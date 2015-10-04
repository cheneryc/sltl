#pragma once

#include "output.h"

#include "syntax/tree.h"
#include "syntax/block_manager.h"

#include "io/io.h"

#include "detail/function_traits.h"


namespace sltl
{
  class shader
  {
  public:
    enum type
    {
      vertex,
      geometry,
      fragment,
      test
    };

    template<type>
    struct tag {};

    shader(shader&& s) : _t(s._t), _tree(std::move(s._tree)) {}
    shader(type t, syntax::tree_base::ptr&& tree) : _t(t), _tree(std::move(tree)) {}

    template<typename Fn, typename ...T>
    std::wstring str(T&& ...t) const
    {
      static_assert(std::is_base_of<output, Fn>::value, "Template parameter Fn must derive from sltl::output");
      //TODO: static assert that the type Fn is callable? Something similar to std::is_function, but works for functors & lambdas

      Fn fn(std::forward<T>(t)...);
      
      if(!_tree->apply_action(fn))
      {
        throw std::exception();//TODO: better exception type and message?
      }

      return fn.str();
    }

    const type _t;

  private:
    syntax::tree_base::ptr _tree;
  };

  typedef shader::tag<shader::vertex>   shader_tag_vertex;
  typedef shader::tag<shader::geometry> shader_tag_geometry;
  typedef shader::tag<shader::fragment> shader_tag_fragment;

  // The default shader traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct shader_traits : public shader_traits<decltype(&Fn::operator())>
  {
    // Inherits _type from the specialization for a pointer-to-member-function (passing
    // the type of the object's function call operator as the template parameter)
  };

  // The shader traits specialization for a pointer-to-function
  template<typename R, template<shader::type> class A, typename I, shader::type S>
  struct shader_traits<R(*)(A<S>, I)>
  {
    static const shader::type _type = S;
  };

  // The shader traits specialization for a pointer-to-member-function
  template<typename R, typename T, template<shader::type> class A, typename I, shader::type S>
  struct shader_traits<R(T::*)(A<S>, I) const>
  {
    static const shader::type _type = S;
  };

  // Used to create a shader from a callable type without a shader type tag parameter
  template<shader::type S, typename TTree, typename Fn>
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
    return make_shader<shader_traits<Fn>::_type, syntax::tree>(fn_wrap);
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
    return make_shader<shader::test, syntax::tree_fragment>(fn);
  }
}
