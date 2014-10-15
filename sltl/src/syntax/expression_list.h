#pragma once

#include "node.h"
#include "../output.h"

#include <deque>
#include <algorithm>


namespace sltl
{
namespace syntax
{
  class expression_list : public node
  {
  public:
    expression_list() : _expressions() {}
    expression_list(expression_list&& list) : _expressions(std::move(list._expressions)) {}

    expression_list(node::ptr&& n, expression_list&& list) : _expressions(std::move(list._expressions))
    {
      _expressions.push_front(std::move(n));
    }

    virtual void traverse(output& out) const
    {
      std::for_each(_expressions.begin(), _expressions.end(), [this, &out](const node::ptr& node)
      {
        node->traverse(out);
    
        if(node != _expressions.back())
        {
          //TODO: this is a bit of a hack, this logic should be in the output object
          out.comma();
        }
      });
    }

    void add(node::ptr&& node)
    {
      _expressions.push_back(std::move(node));
    }

  private:
    std::deque<node::ptr> _expressions;
  };
}
}
