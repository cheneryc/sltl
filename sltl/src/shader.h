#pragma once

#include "syntax/action.h"
#include "syntax/tree.h"
#include "syntax/block_manager.h"

#include "io/io.h"

#include "core/shader_stage.h"

#include "detail/function_traits.h"


namespace sltl
{
  class shader
  {
  public:
    template<core::shader_stage>
    struct tag {};

    shader(shader&& s) : _stage(s._stage), _tree(std::move(s._tree)) {}
    shader(core::shader_stage stage, syntax::tree_base::ptr&& tree) : _stage(stage), _tree(std::move(tree)) {}

    template<typename Fn, bool is_error = true, typename ...T>
    std::wstring str(T&& ...t) const
    {
      static_assert(std::is_base_of<syntax::const_action_result<std::wstring>, Fn>::value, "sltl::shader::str: template parameter Fn must derive from sltl::syntax::const_action_result<std::wstring>");
      //TODO: static assert that the type Fn is callable? Something similar to std::is_function, but works for functors & lambdas

      Fn fn(_stage, std::forward<T>(t)...);

      if(!_tree->apply_action(fn) && is_error)
      {
        throw std::exception();//TODO: better exception type and message?
      }

      return fn.get_result();
    }

    const core::shader_stage _stage;

  private:
    syntax::tree_base::ptr _tree;
  };

  typedef shader::tag<core::shader_stage::vertex>   shader_tag_vertex;
  typedef shader::tag<core::shader_stage::geometry> shader_tag_geometry;
  typedef shader::tag<core::shader_stage::fragment> shader_tag_fragment;

  // The default shader traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct shader_traits : shader_traits<decltype(&Fn::operator())>
  {
    // Inherits _stage from the specialization for a pointer-to-member-function (passing
    // the type of the object's function call operator as the template parameter)
  };

  // The shader traits specialization for a pointer-to-function
  template<typename R, template<core::shader_stage> class A, typename I, core::shader_stage S>
  struct shader_traits<R(*)(A<S>, I)>
  {
    static const core::shader_stage _stage = S;
  };

  // The shader traits specialization for a pointer-to-member-function
  template<typename R, typename T, template<core::shader_stage> class A, typename I, core::shader_stage S>
  struct shader_traits<R(T::*)(A<S>, I) const>
  {
    static const core::shader_stage _stage = S;
  };

  // Used to create a shader from a callable type without a shader type tag parameter
  template<core::shader_stage S, typename TTree, typename Fn>
  shader make_shader(Fn fn)
  {
    return shader(S, syntax::tree::make<TTree>(fn));
  }

  // Used to create a shader from a callable type when the first parameter is a shader type tag
  template<typename Fn>
  shader make_shader(Fn fn)
  {
    auto fn_wrap = [fn]()
    {
      fn(detail::function_traits<Fn>::arg<0>::type(),
         detail::function_traits<Fn>::arg<1>::type(core::qualifier_storage::in));
    };

    // Infer the type of shader to be created from the shader type tag parameter.
    return make_shader<shader_traits<Fn>::_stage, syntax::tree>(fn_wrap);
  }

  //TODO: the program functor must support vertex and fragment but not necessarily geometry. How to get this to compile?
  //TODO: the return type would be a type called program, not shader
  template<typename Fn>
  void make_program(Fn fn)
  {
  }

  template<typename Fn>
  shader make_test(Fn fn)
  {
    // Explicitly specify the 'fragment' tree type
    return make_shader<core::shader_stage::test, syntax::tree_fragment>(fn);
  }
}
