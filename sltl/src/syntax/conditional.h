#pragma once

#include "node.h"
#include "../output.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  class conditional : public node
  {
  public:
    conditional(language::conditional_id id) : _condition(), _id(id) {}
    conditional(language::conditional_id id, node::ptr&& condition) : _condition(std::move(condition)), _id(id) {}

    virtual void traverse(output& out) const
    {
      out(*this);

      if(_condition)
      {
        _condition->traverse(out);
      }

      out(*this, false);
    }

    const node::ptr _condition;
    const language::conditional_id _id;
  };
}
}
