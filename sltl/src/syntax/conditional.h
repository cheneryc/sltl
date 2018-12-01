#pragma once

#include "action.h"
#include "statement.h"

#include <language.h>

#include <cassert>


namespace sltl
{
namespace syntax
{
  class conditional : public statement
  {
  public:
    conditional(language::conditional_id id) : _id(id)
    {
      assert(_id == language::id_else);
    }

    conditional(language::conditional_id id, expression::ptr&& condition) : _id(id), _condition(std::move(condition))
    {
      assert((_id == language::id_if) || (_id == language::id_else_if));
    }

    template<typename T, typename ...A>
    T& add(A&&... a)
    {
      return static_cast<T&>(*(_statement = statement::make<T>(std::forward<A>(a)...)));
    }

    template<typename T, typename ...A>
    T& add_else(A&&... a)
    {
      return static_cast<T&>(*(_statement_else = statement::make<T>(std::forward<A>(a)...)));
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const expression* get_condition() const
    {
      return _condition.get();
    }

    const statement* get_statement() const
    {
      return _statement.get();
    }

    const statement* get_statement_else() const
    {
      return _statement_else.get();
    }

    const language::conditional_id _id;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, conditional>::value, bool>::type
    {
      assert(((type._id == language::id_else) && !type._condition) ||
             ((type._id != language::id_else) &&  type._condition));

      const auto act_result = act(type);

      assert(act_result == action_return_t::step_in ||
             act_result == action_return_t::step_over ||
             act_result == action_return_t::stop);

      bool is_continuing = act_result != action_return_t::stop;

      if(act_result == action_return_t::step_in)
      {
        if(type._condition)
        {
          is_continuing = type._condition->apply_action(act);
        }

        assert( type._statement);
        assert(!type._statement_else || (type._id != language::id_else));

        if(type._statement)
        {
          is_continuing = is_continuing && type._statement->apply_action(act);
        }

        if(type._statement_else)
        {
          is_continuing = is_continuing && type._statement_else->apply_action(act);
        }
      }

      auto fn = [&act](T& t)
      {
        return act(t, false);
      };

      return is_continuing && apply_action_impl(fn, type);
    }

    const expression::ptr _condition;

    statement::ptr _statement;
    statement::ptr _statement_else;
  };
}
}
