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
      _functions = s._manager_function->transfer();
      _intrinsics = s._manager_intrinsic->transfer();

      std::tie(_io_block_in, _io_block_out, _io_block_uniform) = s._manager_io_block->transfer();
    }
  };

  class tree_shader : public tree
  {
  public:
    template<typename Fn>
    tree_shader(Fn fn)
    {
      const function_definition& fn_def = syntax::function_manager_guard()->emplace(fn, L"main");

      if(fn_def.get_type() != language::type_helper<void>())
      {
        throw std::exception();//TODO: exception type and message
      }
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

      is_continuing = is_continuing && std::all_of(type._functions.begin(), type._functions.end(), [&act](const function_definition::ptr& fd)
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
