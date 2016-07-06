#pragma once

#include "block.h"
#include "function_manager.h"
#include "function_definition.h"

#include <algorithm>


namespace sltl
{
namespace syntax
{
  class tree_base
  {
  public:
    typedef std::unique_ptr<tree_base> ptr;

    // Make sure all derived types are non-copyable
    tree_base(const tree_base&) = delete;
    tree_base& operator=(const tree_base&) = delete;

    virtual ~tree_base()
    {
    }

    template<typename T, typename ...A>
    static ptr make(A&&... a)
    {
      return ptr(new T(std::forward<A>(a)...));
    }

    virtual bool apply_action(const_action& cact) const = 0;

  protected:
    tree_base() = default;
  };

  //TODO: rename this as tree_shader and the base class to just tree
  class tree : public tree_base
  {
  public:
    template<typename Fn>
    tree(Fn fn)
    {
      //TODO: is there a good way for the function_manager to create/own the main function definition?
      _functions.emplace_back(std::make_unique<function_definition>(fn, L"main", language::type_helper<void>()));
      function_manager::get().move(_functions);

      std::tie(_io_block_in, _io_block_out, _io_block_uniform) = io_block_manager::get().move();
    }

    virtual bool apply_action(const_action& cact) const override
    {
      bool is_continuing = true;

      if(_io_block_uniform)
      {
        is_continuing = _io_block_uniform->apply_action(cact);
      }

      if(_io_block_in && is_continuing)
      {
        is_continuing = _io_block_in->apply_action(cact);
      }

      if(_io_block_out && is_continuing)
      {
        is_continuing = _io_block_out->apply_action(cact);
      }

      // Traverse in reverse order so the main function is processed last
      return is_continuing && std::all_of(_functions.rbegin(), _functions.rend(), [&cact](const function_definition::ptr& fd)
      {
        return fd->apply_action(cact);
      });
    }

  private:
    //TODO: the order of the collection might be very important? Maybe use a set instead of a vector?
    std::vector<function_definition::ptr> _functions;

    statement::ptr _io_block_in;
    statement::ptr _io_block_out;
    statement::ptr _io_block_uniform;
  };

  class tree_fragment : public tree_base
  {
  public:
    template<typename Fn>
    tree_fragment(Fn fn) : _root_block(block::global)
    {
      fn();

      //TODO: whatabout function_definitions now potentially stored in the function_manager?
      //TODO: clear the map function_manager's map or this will leak/affect the next shader created?
      //TODO: what about potentially using io_block definitions in fragments?

      // Remove the root block from the block stack. The stack should be empty after this.
      _root_block.pop();
    }

    virtual bool apply_action(const_action& cact) const
    {
      return _root_block.apply_action(cact);
    }

  private:
    block _root_block;
  };
}
}
