#pragma once

#include "expression.h"
#include "../output.h"

#include <deque>
#include <algorithm>


namespace sltl
{
namespace syntax
{
  class expression_list : public expression
  {
  public:
    expression_list() : _expressions() {}
    expression_list(expression_list&& list) : _expressions(std::move(list._expressions)) {}

    expression_list(expression::ptr&& e, expression_list&& list) : _expressions(std::move(list._expressions))
    {
      _expressions.push_front(std::move(e));
    }

    virtual void traverse(output& out) const
    {
      std::for_each(_expressions.begin(), _expressions.end(), [this, &out](const expression::ptr& e)
      {
        e->traverse(out);
    
        if(e != _expressions.back())
        {
          //TODO: this is a bit of a hack, this logic should be in the output object
          //TODO: maybe the solution is to temporarily create a comma/separator object and pass that to the output object?
          out.comma();
        }
      });
    }

    void add(expression::ptr&& e)
    {
      _expressions.push_back(std::move(e));
    }

  private:
    std::deque<expression::ptr> _expressions;
  };
}
}
