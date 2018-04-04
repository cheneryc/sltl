#include "block_manager.h"
#include "block.h"

#include <cassert>


namespace sltl
{
namespace syntax
{
  class block_override
  {
    friend bool is_override_active();

    friend block_base& get_current_block();
    friend block_base* set_current_block(block_base*);

    thread_local static block_base* _override;
  };

  thread_local block_base* block_override::_override = nullptr;
}
}

namespace
{
  namespace ns = sltl::syntax;
}

ns::block_manager::~block_manager()
{
  assert(_block_stack.empty());
}

void ns::block_manager::push(sltl::detail::pass_key<block>, block& b)
{
  block_stack_t block_stack_copy = get_block_stack();

  // Ensure the passed block has not already been pushed onto the stack
  while(!block_stack_copy.empty())
  {
    if(block_stack_copy.top().get() == b)
    {
      throw std::exception();//TODO: exception type and message
    }
    else
    {
      block_stack_copy.pop();
    }
  }

  // Ensure that 'local' blocks have a 'global' ancestor
  if((b._t == block::local) && _block_stack.empty())
  {
    throw std::exception();//TODO: exception type and message
  }

  _block_stack.emplace(b);
}

void ns::block_manager::pop(sltl::detail::pass_key<block>, block& b)
{
  if(_block_stack.empty() ||
     _block_stack.top().get() != b)
  {
    throw std::exception();//TODO: exception type and message
  }

  _block_stack.pop();
}

ns::block& ns::block_manager::get_block()
{
  assert(!_block_stack.empty());

  return _block_stack.top();
}

ns::block_manager::block_stack_t ns::block_manager::get_block_stack() const
{
  return _block_stack;
}

bool ns::is_override_active()
{
  return block_override::_override != nullptr;
}

ns::block_base& ns::get_current_block()
{
  return (is_override_active() ? *(block_override::_override) : block_manager_guard()->get_block());
}

ns::block_base* ns::set_current_block(block_base* b)
{
  block_base* const override_prev = block_override::_override;
  block_override::_override = b;

  return override_prev;
}
