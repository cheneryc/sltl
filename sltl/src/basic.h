#pragma once

#include "traits.h"
#include "variable.h"

#include "syntax/operator.h"

#include "expression/expression.h"

#include "core/semantic.h"
#include "core/qualifier.h"

#include "detail/conditional_traits.h"


namespace sltl
{
  template<template<typename, size_t...> class V, typename T, size_t ...D>
  class basic : public variable
  {
    static_assert(is_scalar<T>::value, "sltl::basic: Type T is not a valid template parameter type");

  public:
    typedef expression::expression<V, T, D...> proxy;

    // Non-copyable and non-assignable
    basic(const basic&) = delete;
    basic& operator=(const basic&) = delete;

    // unary operator overloads

    friend proxy operator++(proxy&& p)
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: prefix increment and decrement operators are not valid if template parameter T is type bool");

      return make_proxy<syntax::operator_unary>(language::id_increment_pre, p.move());
    }

    friend proxy operator--(proxy&& p)
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: prefix increment and decrement operators are not valid if template parameter T is type bool");

      return make_proxy<syntax::operator_unary>(language::id_decrement_pre, p.move());
    }

    friend proxy operator++(proxy&& p, int)
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: postfix increment and decrement operators are not valid if template parameter T is type bool");

      return make_proxy<syntax::operator_unary>(language::id_increment_post, p.move());
    }

    friend proxy operator--(proxy&& p, int)
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: postfix increment and decrement operators are not valid if template parameter T is type bool");

      return make_proxy<syntax::operator_unary>(language::id_decrement_post, p.move());
    }

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
