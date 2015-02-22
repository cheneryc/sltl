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

    virtual void traverse(output& out) const
    {
      out(*this);
      _expression->traverse(out);
      out(*this, false);
    }

  private:
    expression::ptr _expression;
  };
}
}
