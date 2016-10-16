#include "elide.h"

#include "action.h"
#include "temporary.h"
#include "reference.h"
#include "variable_declaration.h"

#include "../language.h"

#include <cassert>


namespace
{
  using namespace sltl;
  using namespace sltl::syntax;

  class elide_action : public action
  {
  public:
    elide_action(const language::type& type) : _type(type), _action_state(unknown), _action_target(nullptr) {}

    virtual action_return_t operator()(temporary& t, bool is_start = true) override
    {
      if(is_start)
      {
        return do_action(t.get_type(), [&]
        {
          _action_state = matching;
          _action_target = &t;

          // The elide action only continues when the expression is a temporary node of matching type
          return action_return_t::step_in;
        });
      }
      else
      {
        return action_return_t::stop;
      }
    }

    virtual action_return_t operator()(syntax::reference& r) override
    {
      return do_action(r._declaration.get_type(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    expression::ptr&& get_result(expression::ptr&& exp)
    {
      assert(_action_state != unknown);

      if(_action_state == omitted)
      {
        return _action_target->move();
      }
      else
      {
        return std::move(exp);
      }
    }

    const language::type _type;

  protected:
    virtual action_return_t operator()(float) override
    {
      return do_action(language::type_helper<float>(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    virtual action_return_t operator()(double) override
    {
      return do_action(language::type_helper<double>(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    virtual action_return_t operator()(int) override
    {
      return do_action(language::type_helper<int>(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    virtual action_return_t operator()(unsigned int) override
    {
      return do_action(language::type_helper<unsigned int>(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    virtual action_return_t operator()(bool) override
    {
      return do_action(language::type_helper<bool>(), [&]{ _action_state = ((_action_state == matching) ? omitted : different); return action_return_t::stop; });
    }

    virtual action_return_t get_default(bool is_start) override
    {
      assert(is_start);

      _action_state = different;
      _action_target = nullptr;

      return action_return_t::stop;
    }

  private:
    template<typename Fn>
    action_return_t do_action(const language::type& type, Fn fn)
    {
      assert(((_action_state == unknown)  && !_action_target) ||
             ((_action_state == matching) &&  _action_target));

      action_return_t is_continuing = action_return_t::stop;

      if(_type == type)
      {
        is_continuing = fn();
      }
      else
      {
        _action_state = different;
        _action_target = nullptr;
      }

      return is_continuing;
    }

    enum
    {
      unknown,
      matching,
      different,
      omitted
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

  return std::move(exp);//TODO: check the elided node's destructor(s) runs when elision takes place
}
