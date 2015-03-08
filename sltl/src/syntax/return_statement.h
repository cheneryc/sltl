#pragma once

#include "action.h"
#include "statement.h"
#include "expression.h"


namespace sltl
{
namespace syntax
{
  class return_statement : public statement
  {
  public:
    return_statement(expression::ptr&& e) : _expression(std::move(e)) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, return_statement>::value, bool>::type
    {
      return (act(type) && type._expression->apply_action(act) && act(type, false));
    }

    expression::ptr _expression;
  };
}
}
