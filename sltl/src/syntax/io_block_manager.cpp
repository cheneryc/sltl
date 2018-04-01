#include "io_block_manager.h"
#include "io_block.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

ns::io_block* ns::io_block_manager::get_io_block(sltl::core::qualifier_storage qualifier)
{
  assert(qualifier != core::qualifier_storage::default);

  return static_cast<io_block*>(get_io_block_node(qualifier).get());
}

ns::statement::ptr& ns::io_block_manager::get_io_block_node(sltl::core::qualifier_storage qualifier)
{
  switch(qualifier)
  {
    case core::qualifier_storage::in:
      return _io_in;
      break;
    case core::qualifier_storage::out:
      return _io_out;
      break;
    case core::qualifier_storage::uniform:
      return _io_uniform;
      break;
    default:
      throw std::exception();//TODO: exception type and message
  }
}

ns::io_block& ns::io_block_manager::add(sltl::core::qualifier_storage qualifier)
{
  assert(qualifier != core::qualifier_storage::default);

  statement::ptr& io_block_node = get_io_block_node(qualifier);

  if(io_block_node)
  {
    throw std::exception();//TODO: exception type and message
  }
  else
  {
    io_block_node.reset(new io_block({}, qualifier));
  }

  return static_cast<io_block&>(*io_block_node);
}

std::tuple<ns::statement::ptr&&, ns::statement::ptr&&, ns::statement::ptr&&> ns::io_block_manager::move()
{
  return std::forward_as_tuple(std::move(_io_in), std::move(_io_out), std::move(_io_uniform));
}
