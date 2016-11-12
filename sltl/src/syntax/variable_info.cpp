#include "variable_info.h"

#include "block.h"
#include "block_manager.h"
#include "io_block.h"
#include "io_block_manager.h"
#include "variable_declaration.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

ns::variable_info& ns::get_variable_info(const variable_declaration* vd)
{
  assert(vd);

  const auto qualifier_storage = static_cast<const sltl::core::storage_qualifier&>(vd->get_qualifier())._value;

  if(qualifier_storage == sltl::core::qualifier_storage::default)
  {
    return ns::get_current_block().variable_info_find(vd->_name);
  }
  else if(auto io_block = ns::io_block_manager::get().get_io_block(qualifier_storage))
  {
    return io_block->variable_info_find(vd->_name);
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }
}
