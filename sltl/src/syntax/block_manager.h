#pragma once

#include "../detail/pass_key.h"

#include <stack>
#include <functional>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class block;
  class block_base;

  //TODO: Manager (needs a better name?) is a singleton but might actually be better as thread-local?
  class block_manager
  {
    typedef std::stack<std::reference_wrapper<block>> block_stack_t;

  public:
    void push(detail::pass_key<block>, block& b);
    void pop(detail::pass_key<block>, block& b);

    block& get_block();
    block_stack_t get_block_stack() const;

    static block_manager& get();

  private:
    block_stack_t _block_stack;
  };

  bool is_override_active();

  block_base& get_current_block();
  block_base* set_current_block(block_base* b);
}
}
