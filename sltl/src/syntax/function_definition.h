#pragma once

#include "node.h"
#include "block.h"
#include "block_guard.h"
#include "block_manager.h"
#include "elide.h"
#include "action.h"
#include "declaration.h"
#include "parameter_declaration.h"
#include "return_statement.h"

#include <language.h>

#include <detail/function_traits.h>


namespace sltl
{
namespace syntax
{
  class function_definition : public declaration, public node
  {
  public:
    typedef std::unique_ptr<function_definition> ptr;

    template<typename Fn>
    function_definition(Fn fn, std::wstring&& name, const language::type& type_return) : declaration(std::move(name)), _type_return(type_return), _function_body(block::global), _depth(), _depth_id()
    {
      block_guard(_function_body, [this, &fn](){ call_fn(fn); });
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    void set_type(const language::type& type)
    {
      _type_return = type;
    }

    virtual language::type get_type() const override
    {
      return _type_return;
    }

    void set_depth(size_t depth)
    {
      _depth = depth;
    }

    size_t get_depth() const
    {
      return _depth;
    }

    void set_depth_id(size_t depth_id)
    {
      _depth_id = depth_id;
    }

    size_t get_depth_id() const
    {
      return _depth_id;
    }

    const parameter_list& get_params() const
    {
      return _parameters;
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, function_definition>::value, bool>::type
    {
      return (apply_action_impl(act, type, &(type._parameters)) && type._function_body.apply_action(act));
    }

    template<size_t>
    struct tag {};

    // Template parameters:
    // Fn - function object type
    // R  - return type
    // P  - parameter types
    // D  - parameter declarations (syntax::parameter_declaration pointers)

    // Parameter declarations have been created - calls the function object by constructing temporary argument instances (void return type)
    template<typename Fn, typename ...P, typename ...D>
    static auto invoke_void_impl(tag<detail::function_traits<Fn>::param_count>, Fn fn, parameter_list&, D&& ...decls) -> void
    {
      fn(P(std::forward<D>(decls))...);
    }

    // Recursive function that creates a parameter_declaration for the function's nth parameter (void return type)
    template<typename Fn, typename ...P, typename ...D, size_t N>
    static auto invoke_void_impl(tag<N>, Fn fn, parameter_list& parameters, D&& ...decls) -> typename std::enable_if<detail::is_function_param_index<Fn, N>(), void>::type
    {
      typedef typename detail::get<N, P...>::type param_t;

      parameter_declaration::ptr decl = std::make_unique<parameter_declaration>(std::to_wstring(N + 1), language::type_helper<param_t>(), core::qualifier_param::in);
      parameter_declaration* ptr_decl = decl.get();

      parameters.add(std::move(decl));

      invoke_void_impl<Fn, P...>(tag<N + 1>(), fn, parameters, std::forward<D>(decls)..., ptr_decl);
    }

    // Parameter declarations have been created - calls the function object by constructing temporary argument instances
    template<typename Fn, typename R, typename ...P, typename ...D>
    static auto invoke_impl(tag<detail::function_traits<Fn>::param_count>, Fn fn, parameter_list&, D&& ...decls) -> expression::ptr
    {
      return typename R::proxy(fn(P(std::forward<D>(decls))...)).move();
    }

    // Recursive function that creates a parameter_declaration for the function's nth parameter
    template<typename Fn, typename R, typename ...P, typename ...D, size_t N>
    static auto invoke_impl(tag<N>, Fn fn, parameter_list& parameters, D&& ...decls) -> typename std::enable_if<detail::is_function_param_index<Fn, N>(), expression::ptr>::type
    {
      typedef typename detail::get<N, P...>::type param_t;

      parameter_declaration::ptr decl = std::make_unique<parameter_declaration>(std::to_wstring(N + 1), language::type_helper<param_t>(), core::qualifier_param::in);
      parameter_declaration* ptr_decl = decl.get();

      parameters.add(std::move(decl));

      return invoke_impl<Fn, R, P...>(tag<N + 1>(), fn, parameters, std::forward<D>(decls)..., ptr_decl);
    }

    template<typename R, typename TT>
    struct function_invoker;

    template<template<typename ...> class TT, typename ...P>
    struct function_invoker<void, TT<P...>>
    {
      template<typename Fn>
      static void invoke(Fn fn, parameter_list& parameters)
      {
        invoke_void_impl<Fn, P...>(tag<0>(), fn, parameters);
      }
    };

    template<template<typename ...> class TT, typename ...P, typename R>
    struct function_invoker<R, TT<P...>>
    {
      template<typename Fn>
      static expression::ptr invoke(Fn fn, parameter_list& parameters)
      {
        return invoke_impl<Fn, R, P...>(tag<0>(), fn, parameters);
      }
    };

    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if< std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      function_invoker<void, typename detail::function_traits<Fn>::params_t>::invoke(fn, _parameters);
    }

    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if<!std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      typedef typename detail::function_traits<Fn>::params_t params_t;
      typedef typename detail::function_traits<Fn>::return_t return_t;

      syntax::get_current_block().add<return_statement>(elide(function_invoker<return_t, params_t>::invoke(fn, _parameters), _type_return));
    }

    language::type _type_return;
    parameter_list _parameters;

    block _function_body;

    size_t _depth;
    size_t _depth_id;
  };
}
}
