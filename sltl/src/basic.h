#pragma once

#include "variable.h"

#include "expression/expression.h"

#include "core/semantic.h"

#include "detail/type_traits.h"


namespace sltl
{
  template<template<typename, size_t...> class V, typename T, size_t ...D>
  class basic : public variable
  {
    static_assert(detail::is_scalar<T>::value, "sltl::basic: Type T is not a valid template parameter type");

  public:
    typedef expression::expression<V, T, D...> proxy;

    // Non-copyable and non-assignable
    basic(const basic&) = delete;
    basic& operator=(const basic&) = delete;

    // helper functions

    template<typename TNode, typename ...A>
    static proxy make_proxy(A&& ...a)
    {
      return proxy(syntax::expression::make<TNode>(std::forward<A>(a)...));
    }

  protected:
    basic(syntax::expression::ptr&& initializer) : variable(create_variable_declaration(std::move(initializer))) {}
    basic(core::semantic_pair semantic) : variable(create_variable_declaration(language::type_helper<V<T, D...>>(), semantic)) {}

    explicit basic(syntax::parameter_declaration* pd) : variable(pd) {}

    friend expression::expression<V, T, D...>;
  };
}
