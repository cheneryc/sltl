#pragma once

#include "syntax/action.h"
#include "syntax/tree.h"
#include "syntax/block_manager.h"

#include "io/io.h"

#include "core/shader_stage.h"

#include "detail/function_traits.h"
#include "detail/conditional_traits.h"


namespace sltl
{
  namespace detail
  {
    template<typename T>
    using has_get_result_op = decltype(std::declval<T>().get_result());

    template<typename T>
    using has_get_result = detect<T, has_get_result_op>;
  }

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
      return apply_action<Fn, is_error>(std::forward<T>(t)...);
    }

    template<typename Fn, bool is_error = true, typename ...T>
    auto apply_action(T&& ...t) -> typename std::enable_if<detail::negate<detail::has_get_result<Fn>>::value>::type
    {
      static_assert(std::is_base_of<syntax::action, Fn>::value, "sltl::shader::apply_action: template parameter Fn must derive from sltl::syntax::action");

      apply_action_impl<Fn>(*this, is_error, std::forward<T>(t)...);
    }

    template<typename Fn, bool is_error = true, typename ...T>
    auto apply_action(T&& ...t) const -> typename std::enable_if<detail::negate<detail::has_get_result<Fn>>::value>::type
    {
      static_assert(std::is_base_of<syntax::const_action, Fn>::value, "sltl::shader::apply_action: template parameter Fn must derive from sltl::syntax::const_action");

      apply_action_impl<Fn>(*this, is_error, std::forward<T>(t)...);
    }

    template<typename Fn, bool is_error = true, typename ...T, typename R = detail::function_traits<decltype(&Fn::get_result)>::return_t>
    R apply_action(T&& ...t)
    {
      static_assert(std::is_base_of<syntax::action_result<R>, Fn>::value, "sltl::shader::apply_action: template parameter Fn must derive from syntax::action_result<R>");

      return apply_action_impl<Fn>(*this, is_error, std::forward<T>(t)...).get_result();
    }

    template<typename Fn, bool is_error = true, typename ...T, typename R = detail::function_traits<decltype(&Fn::get_result)>::return_t>
    R apply_action(T&& ...t) const
    {
      static_assert(std::is_base_of<syntax::const_action_result<R>, Fn>::value, "sltl::shader::apply_action: template parameter Fn must derive from syntax::const_action_result<R>");

      return apply_action_impl<Fn>(*this, is_error, std::forward<T>(t)...).get_result();
    }

    const core::shader_stage _stage;

  private:
    template<typename Fn, typename S, typename ...T>
    static auto apply_action_impl(S& s, bool is_error, T&& ...t) -> typename std::enable_if<std::is_same<typename std::remove_const<S>::type, shader>::value, Fn>::type
    {
      Fn fn(s._stage, std::forward<T>(t)...);

      if(!s._tree->apply_action(fn) && is_error)
      {
        throw std::exception();//TODO: better exception type and message?
      }

      return std::move(fn);
    }

    syntax::tree_base::ptr _tree;
  };

  typedef shader::tag<core::shader_stage::vertex>   shader_tag_vertex;
  typedef shader::tag<core::shader_stage::geometry> shader_tag_geometry;
  typedef shader::tag<core::shader_stage::fragment> shader_tag_fragment;

  // The default shader traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct shader_traits
  {
    // Use the pointer-to-member-function specialization with the type
    // of the object's function call operator as the template parameter
    static const core::shader_stage _stage = shader_traits<decltype(&Fn::operator())>::_stage;
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
  //TODO: constrain Fn using std::enable_if and std::is_callable (once available) (C++17)
  template<core::shader_stage S, typename TTree, typename Fn>
  shader make_shader(Fn fn)
  {
    //TODO: the lifetime of all manager classes (block, io_block, function) should be controlled by this function, ideally via RAII idiom
    return shader(S, syntax::tree::make<TTree>(fn));
  }

  // Used to create a shader from a callable type when the first parameter is a shader type tag
  //TODO: constrain Fn using std::enable_if and std::is_callable (once available) (C++17)
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
