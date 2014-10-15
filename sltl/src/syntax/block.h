#pragma once

#include "node.h"

#include <string>
#include <vector>


namespace sltl
{
  // Forward declarations - sltl namespace
  class output;

namespace syntax
{
  class block : public node
  {
  public:
    enum type
    {
      local,
      global
    };

    block(type t = local);
    block(block&& b);

    block& operator=(const block&) = delete;

    bool operator==(const block& rhs) const;
    bool operator!=(const block& rhs) const;

    void add(node::ptr&& node);
    void pop();

    //TODO: slightly weird that this takes a raw pointer, maybe lookup via name instead?
    //TODO: note that the user could use std::move so simply the presence of an r-value isn't enough to guarantee that
    // this function can be used, we should also search the tree to see if it is referenced anywhere else.
    node::ptr remove(node* node);

    std::wstring get_child_name();

    virtual void traverse(output& out) const;

  private:
    type _t;

    size_t _current_child_id;
    const std::wstring _name;

    std::vector<node::ptr> _children;
  };
}
}
