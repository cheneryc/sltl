#include "block_manager.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

ns::block_manager& ns::block_manager::get()
{
  static block_manager _manager;
  return _manager;
}

ns::block& ns::block_manager::get_block()
{
  assert(!_block_stack.empty());

  return _block_stack.top();
}

ns::block& ns::get_current_block()
{
  return block_manager::get().get_block();
}
