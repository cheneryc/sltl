#pragma once

#include "syntax/tree.h"
#include "output.h"


namespace sltl
{
  class shader
  {
  public:
    enum type
    {
      vertex,
      geometry,
      fragment
    };

    shader(shader&& s) : _t(s._t), _tree(std::move(s._tree)) {}
    shader(type t, syntax::tree&& tree) : _t(t), _tree(std::move(tree)) {}

    template<typename Fn, typename ...T>
    std::wstring str(T&& ...t) const
    {
      static_assert(std::is_base_of<output, Fn>::value, "Template parameter Fn must derive from sltl::output");
      //TODO: static assert that the type Fn is callable? Something similar to std::is_function, but works for functors & lambdas

      Fn fn(std::forward<T>(t)...);
      _tree.traverse(fn);
      return fn.str();
    }

  private:
    type _t;
    syntax::tree _tree;
  };

  //TODO: create a similar make_test function for use with unit test?
  template<typename Fn>
  shader make_shader(shader::type t, Fn fn)
  {
    syntax::tree tree;

    auto result = fn();//TODO: do something with the returned value?

    // Remove the root block from the block stack. The stack should be empty after this.
    syntax::block_manager::get().get_block().pop();

    return shader(t, std::move(tree));
  }
}
