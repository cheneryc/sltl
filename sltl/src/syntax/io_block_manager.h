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

      return static_cast<io_block*>(get_block_node(qualifier).get());
    }

    io_block& add(core::qualifier_storage qualifier)
    {
      assert(qualifier != core::qualifier_storage::default);

      statement::ptr& io_block_node = get_block_node(qualifier);

      //TODO: might need to throw an exception here instead as this could be a user/programmer error
      assert(!io_block_node);

      io_block_node = statement::make<io_block>(qualifier);
      return static_cast<io_block&>(*io_block_node);
    }

    std::tuple<statement::ptr&&, statement::ptr&&, statement::ptr&&> move()
    {
      return std::forward_as_tuple(std::move(_io_in), std::move(_io_out), std::move(_io_uniform));
    }

  private:
    statement::ptr& get_block_node(core::qualifier_storage qualifier)
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

    statement::ptr _io_in;
    statement::ptr _io_out;
    statement::ptr _io_uniform;
  };
}
}
