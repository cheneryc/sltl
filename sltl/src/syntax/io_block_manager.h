#pragma once

#include "statement.h"

#include "../core./qualifier.h"
#include "../detail/scoped_singleton.h"

#include <tuple>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class io_block;

  class io_block_manager
  {
  public:
    io_block* get_io_block(core::qualifier_storage qualifier);

    io_block& add(core::qualifier_storage qualifier);

    //TODO: the returned tuple shouldn't be of type rvalue reference?
    std::tuple<statement::ptr&&, statement::ptr&&, statement::ptr&&> move();

  private:
    statement::ptr& get_io_block_node(core::qualifier_storage qualifier);

    statement::ptr _io_in;
    statement::ptr _io_out;
    statement::ptr _io_uniform;
  };

  typedef detail::scoped_singleton<io_block_manager, detail::scope_t::thread> io_block_manager_guard;
}
}
