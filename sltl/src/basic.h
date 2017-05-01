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

    // binary operator overloads

    proxy operator+=(proxy&& p)
    {
      return make_proxy<syntax::operator_binary>(language::id_assignment_addition, make_reference(), p.move());
    }

    friend proxy operator+(proxy&& lhs, proxy&& rhs)
    {
      return make_proxy<syntax::operator_binary>(language::id_addition, lhs.move(), rhs.move());
    }

    proxy operator-=(proxy&& p)
    {
      return make_proxy<syntax::operator_binary>(language::id_assignment_subtraction, make_reference(), p.move());
    }

    friend proxy operator-(proxy&& lhs, proxy&& rhs)
    {
      return make_proxy<syntax::operator_binary>(language::id_subtraction, lhs.move(), rhs.move());
    }

    // helper functions

    template<typename T, typename ...A>
    static proxy make_proxy(A&& ...a)
    {
      return proxy(syntax::expression::make<T>(std::forward<A>(a)...));
    }

  protected:
    basic(syntax::expression::ptr&& initializer) : variable(std::move(initializer)) {}
    basic(core::semantic_pair semantic) : variable(language::type_helper<V<T, D...>>(), semantic) {}

    friend expression::expression<V, T, D...>;
  };
}
