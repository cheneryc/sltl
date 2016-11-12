#include "block_manager.h"
#include "block.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

void ns::block_manager::push(sltl::detail::pass_key<block>, block& b)
{
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

ns::block_manager& ns::block_manager::get()
{
  static block_manager _manager;
  return _manager;
}

ns::block& ns::get_current_block()
{
  return block_manager::get().get_block();
}
