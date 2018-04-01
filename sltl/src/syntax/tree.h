#pragma once

#include "block.h"
#include "block_guard.h"
#include "block_manager.h"
#include "io_block.h"
#include "io_block_manager.h"
#include "function_manager.h"
#include "function_definition.h"
#include "intrinsic_manager.h"
#include "intrinsic_declaration.h"

#include <algorithm>


namespace sltl
{
namespace syntax
{
  class tree
  {
  public:
    typedef std::unique_ptr<tree> ptr;

    // Make sure all derived types are non-copyable
    tree(const tree&) = delete;
    tree& operator=(const tree&) = delete;

    virtual ~tree() = default;

    template<typename T, typename ...A>
    static ptr make(A&&... a)
    {
      ptr tree;

      {
        state s;

        tree = std::make_unique<T>(std::forward<A>(a)...);
        tree->move(s);
      }

      return tree;
    }

    virtual bool apply_action(action& act) = 0;
    virtual bool apply_action(const_action& cact) const = 0;

  protected:
    tree() = default;

    //TODO: the order of the collection is very important. Maybe assign functions a generation (0, 1, 2 etc.) and use a set instead of a vector?
    std::vector<function_definition::ptr> _functions;

    statement::ptr _io_block_in;
    statement::ptr _io_block_out;
    statement::ptr _io_block_uniform;

    std::vector<intrinsic_declaration::ptr> _intrinsics;

  private:
    struct state final
    {
      block_manager_guard     _manager_block;
      io_block_manager_guard  _manager_io_block;
      function_manager_guard  _manager_function;
      intrinsic_manager_guard _manager_intrinsic;
    };

    void move(state& s)
    {
      s._manager_function->move(_functions);
      s._manager_intrinsic->move(_intrinsics);

      std::tie(_io_block_in, _io_block_out, _io_block_uniform) = s._manager_io_block->move();
    }
  };

  class tree_shader : public tree
  {
  public:
    template<typename Fn>
    tree_shader(Fn fn)
    {
      _functions.emplace_back(std::make_unique<function_definition>(fn, L"main", language::type_helper<void>()));
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
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, tree_shader>::value, bool>::type
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
  };

  class tree_test : public tree
  {
  public:
    template<typename Fn>
    tree_test(Fn fn) : _root_block(block::global)
    {
      block_guard(_root_block, fn);
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
