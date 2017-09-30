#pragma once

#include "block.h"
#include "function_manager.h"
#include "function_definition.h"
#include "intrinsic_manager.h"
#include "intrinsic_declaration.h"

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

    virtual bool apply_action(action& act) = 0;
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
      _functions.emplace_back(std::make_unique<function_definition>(fn, L"main", language::type_helper<void>()));

      function_manager::get().move(_functions);
      intrinsic_manager::get().move(_intrinsics);

      std::tie(_io_block_in, _io_block_out, _io_block_uniform) = io_block_manager::get().move();
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, tree>::value, bool>::type
    {
      bool is_continuing = true;

      if(type._io_block_uniform)
      {
        is_continuing = type._io_block_uniform->apply_action(act);
      }

      if(type._io_block_in && is_continuing)
      {
        is_continuing = type._io_block_in->apply_action(act);
      }

      if(type._io_block_out && is_continuing)
      {
        is_continuing = type._io_block_out->apply_action(act);
      }

      is_continuing = is_continuing && std::all_of(type._intrinsics.begin(), type._intrinsics.end(), [&act](const intrinsic_declaration::ptr& id)
      {
        return id->apply_action(act);
      });

      // Traverse in reverse order so the main function is processed last
      is_continuing = is_continuing && std::all_of(type._functions.rbegin(), type._functions.rend(), [&act](const function_definition::ptr& fd)
      {
        return fd->apply_action(act);
      });

      return is_continuing;
    }

    //TODO: the order of the collection is very important. Maybe assign functions a generation (0, 1, 2 etc.) and use a set instead of a vector?
    std::vector<function_definition::ptr> _functions;

    statement::ptr _io_block_in;
    statement::ptr _io_block_out;
    statement::ptr _io_block_uniform;

    std::vector<intrinsic_declaration::ptr> _intrinsics;
  };

  class tree_fragment : public tree_base
  {
  public:
    template<typename Fn>
    tree_fragment(Fn fn) : _root_block(block::global)
    {
      fn();

      // Remove the root block from the block stack. The stack should be empty after this.
      _root_block.pop();
    }

    virtual bool apply_action(action& act) override
    {
      return _root_block.apply_action(act);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return _root_block.apply_action(cact);
    }

  private:
    block _root_block;
  };
}
}
