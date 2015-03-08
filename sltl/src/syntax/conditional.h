#pragma once

#include "action.h"
#include "statement.h"
#include "parentheses.h"
#include "../language.h"

#include <cassert>


namespace sltl
{
namespace syntax
{
  //TODO: rename this class selection?
  class conditional : public statement
  {
  public:
    conditional(language::conditional_id id) : _id(id)
    {
      assert(_id == language::id_else);
    }

    conditional(language::conditional_id id, expression::ptr&& condition) : _id(id), _condition(expression::make<parentheses>(std::move(condition)))
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

    const language::conditional_id _id;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, conditional>::value, bool>::type
    {
      bool is_continuing = act(type);

      assert(((type._id == language::id_else) && !type._condition) ||
             ((type._id != language::id_else) &&  type._condition));

      if(is_continuing && type._condition)
      {
        is_continuing = type._condition->apply_action(act);
      }

      is_continuing = act(type, false);

      if(is_continuing)
      {
        assert( type._statement);
        assert(!type._statement_else || (type._id != language::id_else));

        is_continuing = type._statement->apply_action(act);

        if(is_continuing && type._statement_else)
        {
          is_continuing = type._statement_else->apply_action(act);
        }
      }

      return is_continuing;
    }

    const expression::ptr _condition;

    statement::ptr  _statement;
    statement::ptr  _statement_else;
  };
}
}
