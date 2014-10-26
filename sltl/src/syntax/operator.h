#pragma once

#include "statement.h"
#include "parentheses.h"
#include "../output.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  //TODO: maybe have a less generic class e.g. binary_operator, unary_operator etc?
  class operator_base : public expression
  {
  protected:
    operator_base(expression::ptr&& lhs, expression::ptr&& rhs) : expression(),
      _lhs(add_parentheses(std::move(lhs))),
      _rhs(add_parentheses(std::move(rhs))) {}

    const expression::ptr _lhs;
    const expression::ptr _rhs;

  private:
    expression::ptr add_parentheses(expression::ptr&& e)
    {
      if(dynamic_cast<operator_base*>(e.get()))
      {
        return expression::make<parentheses>(std::move(e));
      }
      else
      {
        return std::move(e);
      }
    }
  };

  class binary_operator : public operator_base
  {
  public:
    binary_operator(language::operator_id id, expression::ptr&& lhs, expression::ptr&& rhs) : operator_base(std::move(lhs), std::move(rhs)), _id(id) {}

    virtual void traverse(output& out) const
    {
      _lhs->traverse(out);
      out(*this);
      _rhs->traverse(out);
    }

    const language::operator_id _id;
  };

  //TODO: can combine the binary and assignment operator classes into one?
  class assignment_operator : public operator_base
  {
  public:
    assignment_operator(language::assignment_operator_id id, expression::ptr&& lhs, expression::ptr&& rhs) : operator_base(std::move(lhs), std::move(rhs)), _id(id) {}

    virtual void traverse(output& out) const
    {
      _lhs->traverse(out);
      out(*this);
      _rhs->traverse(out);
    }

    const language::assignment_operator_id _id;
  };
}
}
