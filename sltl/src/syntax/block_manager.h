#pragma once

#include "block.h"

#include <stack>
#include <functional>


namespace sltl
{
namespace syntax
{
  //TODO: Manager (needs a better name?) is a singleton but might actually be better as thread-local?
  class block_manager
  {
  public:
    static block_manager& get();

    block& get_block();

  private:
    // The constructors and destructor of the block class are
    // defined as friends so they can access the _block_stack
    friend block::block(block::type);
    friend block::block(block&&);
    friend void block::pop();

    std::stack<std::reference_wrapper<block>> _block_stack;
  };

  block& get_current_block();
}
}
