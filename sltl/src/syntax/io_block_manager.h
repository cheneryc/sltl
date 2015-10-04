#pragma once

#include "io_block.h"
#include "statement.h"

#include "../core./qualifier.h"
#include "../language.h"

#include <tuple>

#include <cassert>


namespace sltl
{
namespace syntax
{
  class io_block_manager
  {
  public:
    static io_block_manager& get()
    {
      static io_block_manager manager;
      return manager;
    }

    io_block* get_io_block(core::qualifier_storage qualifier)
    {
      assert(qualifier != core::qualifier_storage::default);
      assert(qualifier != core::qualifier_storage::uniform);

      return static_cast<io_block*>((qualifier == core::qualifier_storage::in) ? _io_in.get() : _io_out.get());
    }

    io_block& add(core::qualifier_storage qualifier)
    {
      assert(qualifier != core::qualifier_storage::default);
      assert(qualifier != core::qualifier_storage::uniform);

      statement::ptr& io_block_node = ((qualifier == core::qualifier_storage::in) ? _io_in : _io_out);

      //TODO: might need to throw an exception here instead as this could be a user/programmer error
      assert(!io_block_node);

      io_block_node = statement::make<io_block>(qualifier);
      return static_cast<io_block&>(*io_block_node);
    }

    std::tuple<statement::ptr&&, statement::ptr&&> move()
    {
      return std::forward_as_tuple(std::move(_io_in), std::move(_io_out));
    }

  private:
    statement::ptr _io_in;
    statement::ptr _io_out;
  };
}
}
