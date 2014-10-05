#pragma once

#include "node.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  //TODO: maybe have a less generic class e.g. binary_operator, unary_operator etc?
  class operator_base : public node
  {
  };

  class binary_operator : public operator_base
  {
  public:
    binary_operator(language::operator_id id, node::ptr&& lhs, node::ptr&& rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)), _id(id) {}

    virtual void traverse(output& out) const
    {
      //TODO: maybe the parentheses should be added to the syntax tree as a node?
      out.parentheses_begin();
      _lhs->traverse(out);
      out(*this);
      _rhs->traverse(out);
      out.parentheses_end();
    }

    const node::ptr _lhs;
    const node::ptr _rhs;

    const language::operator_id _id;
  };

  //TODO: can combine the binary and assignment operator classes into one?
  class assignment_operator : public operator_base
  {
  public:
    assignment_operator(language::assignment_operator_id id, node::ptr&& lhs, node::ptr&& rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)), _id(id) {}

    virtual void traverse(output& out) const
    {
      _lhs->traverse(out);
      out(*this);
      _rhs->traverse(out);
    }

    const node::ptr _lhs;
    const node::ptr _rhs;

    const language::assignment_operator_id _id;
  };
}
}
