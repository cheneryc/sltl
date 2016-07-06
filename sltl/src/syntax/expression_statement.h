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
    expression::ptr _expression;
  };
}
}
