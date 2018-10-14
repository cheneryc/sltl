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

  if(vd->_qualifier == sltl::core::qualifier_storage::none)
  {
    return *(ns::get_current_block().variable_info_find(vd->_name));
  }
  else if(auto io_block = ns::io_block_manager_guard()->get_io_block(vd->_qualifier))
  {
    return *(io_block->variable_info_find(vd->_name));
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }
}
