#pragma once

#include "expression.h"
#include "../output.h"


namespace sltl
{
namespace syntax
{
  class parentheses : public expression
  {
  public:
    parentheses(expression::ptr&& e) : _e(std::move(e)) {}

    virtual void traverse(output& out) const
    {
      out(*this);
      _e->traverse(out);
      out(*this, false);
    }

  private:
    expression::ptr _e;
  };
}
}
