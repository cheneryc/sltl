#pragma once

#include "block.h"


namespace sltl
{
namespace syntax
{
  class tree
  {
  public:
    tree() : _root_block(block::global) {}
    tree(tree&& t) : _root_block(std::move(t._root_block)) {}

    void traverse(output& out) const
    {
      _root_block.traverse(out);
    }

  private:
    block _root_block;
  };
}
}
