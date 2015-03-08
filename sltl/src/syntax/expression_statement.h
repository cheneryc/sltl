#pragma once

#include "statement.h"
#include "expression.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class expression_statement : public statement
  {
  public:
    expression_statement(expression::ptr&& e);

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, expression_statement>::value, bool>::type
    {
      return (act(type) && type._expression->apply_action(act) && act(type, false));
    }

    expression::ptr _expression;
  };
}
}
