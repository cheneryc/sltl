#pragma once

#include "statement.h"
#include "expression.h"


namespace sltl
{
namespace syntax
{
  class expression_statement : public statement
  {
  public:
    expression_statement(expression::ptr&& e);

    virtual void traverse(output& out) const;

  private:
    expression::ptr _expression;
  };
}
}
