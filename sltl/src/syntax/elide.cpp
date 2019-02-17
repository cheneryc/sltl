#include "elide.h"

#include "action.h"
#include "temporary.h"

#include <type.h>

#include <cassert>


namespace
{
  using namespace sltl;
  using namespace sltl::syntax;

  class elide_action : public action
  {
  public:
    elide_action(const language::type& type) : _type(type),
      _action_state(state_unknown),
      _action_target(nullptr) {}

    virtual action_return_t operator()(temporary& t, bool is_start = true) override
    {
      action_return_t return_val = action_return_t::stop;

      if(is_start)
      {
        assert(((_action_state == state_unknown)  && !_action_target) ||
               ((_action_state == state_matching) &&  _action_target));

        const language::type type = t.get_type();

        if(_type == type)
        {
          if(t.has_initializer())
          {
            return_val = action_return_t::step_in;

            _action_state = state_matching;
            _action_target = &t;
          }
          else
          {
            _action_state = ((_action_state == state_matching) ? state_omitted : state_different);
          }
        }
        else
        {
          assert(_type == type);

          // The syntax tree is in an invalid state, do not perform any elision
          _action_state = state_different;
          _action_target = nullptr;
        }
      }

      return return_val;
    }

    expression::ptr&& get_result(expression::ptr&& exp)
    {
      assert(((_action_state == state_omitted)   &&  _action_target) ||
             ((_action_state == state_different) && !_action_target));

      if(_action_state == state_omitted)
      {
        exp = _action_target->move();// Destruction of elided temporary nodes happens during this assignment
      }

      return std::move(exp);
    }

    const language::type _type;

  protected:
    virtual action_return_t get_default(bool) override
    {
      assert(((_action_state == state_unknown)  && !_action_target) ||
             ((_action_state == state_matching) &&  _action_target));

      _action_state = ((_action_state == state_matching) ?
        state_omitted :
        state_different);

      return action_return_t::stop;
    }

  private:
    enum
    {
      state_unknown,
      state_matching,
      state_different,
      state_omitted
    } _action_state;

    temporary* _action_target;
  };

  namespace ns = sltl::syntax;
}

ns::expression::ptr&& ns::elide(expression::ptr&& exp, const language::type& type)
{
  elide_action elider(type);

  exp->apply_action(elider);
  exp = elider.get_result(std::move(exp));

  return std::move(exp);
}
