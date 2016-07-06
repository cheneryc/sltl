#pragma once

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
      return apply_action_impl(act, *this, _expression.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _expression.get());
    }

  private:
    expression::ptr _expression;
  };
}
}
