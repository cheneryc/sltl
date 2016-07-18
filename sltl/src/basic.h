#pragma once

#include "traits.h"
#include "variable.h"

#include "syntax/literal.h"
#include "syntax/operator.h"
#include "syntax/reference.h"
#include "syntax/expression_statement.h"

#include "core/semantic.h"
#include "core/qualifier.h"

#include "detail/conditional_traits.h"
#include "detail/variadic_traits.h"

#include <cassert>


namespace sltl
{
  template<template<typename, size_t...> class V, typename T, size_t ...D>
  class basic : public variable
  {
    static_assert(is_scalar<T>::value, "sltl::basic: Type T is not a valid template parameter type");

  public:
    basic(const basic&) = delete;
    basic& operator=(const basic&) = delete;

    class proxy_base
    {
    public:
      proxy_base(const proxy_base& p) = delete;
      proxy_base& operator=(const proxy_base&) = delete;

      syntax::expression::ptr&& move()
      {
        return std::move(_expression);
      }

      ~proxy_base()
      {
        if(_expression)
        {
          syntax::get_current_block().add<syntax::expression_statement>(std::move(_expression));
        }
      }

    protected:
      proxy_base(syntax::expression::ptr&& e) : _expression(std::move(e)) {}

      syntax::expression::ptr _expression;
    };

    template<bool>
    class proxy_t : public proxy_base
    {
    public:
      proxy_t(syntax::expression::ptr&& e) : proxy_base(std::move(e)) {}

      proxy_t(proxy_t&& p) : proxy_t(std::move(p._expression)) {}
      proxy_t(V<T, D...>&& v) : proxy_t(v.make_reference_or_temporary()) {}
      proxy_t(const V<T, D...>& v) : proxy_t(v.make_reference()) {}

      //TODO: C++11, this function should only be callable for r-value references. Its signature should be proxy_t operator=(proxy_t&& p) &&
      proxy_t operator=(proxy_t&& p)
      {
        // As the proxy is never meant to be an l-value self-assignment shouldn't
        // occur (if it does then it will put the tree in an invalid/corrupt state)
        assert(this != &p);

        return make_proxy<syntax::operator_binary>(language::id_assignment, move(), p.move());
      }
    };

    // Specialization for the one-dimensional case that allows
    // a proxy to be constructed directly from an instance of T
    template<>
    class proxy_t<true> : public proxy_base
    {
    public:
      proxy_t(syntax::expression::ptr&& e) : proxy_base(std::move(e)) {}

      proxy_t(T t) : proxy_t(syntax::expression::make<syntax::literal<T>>(t)) {}
      proxy_t(proxy_t&& p) : proxy_t(std::move(p._expression)) {}
      proxy_t(V<T, D...>&& v) : proxy_t(v.make_reference_or_temporary()) {}
      proxy_t(const V<T, D...>& v) : proxy_t(v.make_reference()) {}

      //TODO: C++11, this function should only be callable for r-value references. Its signature should be proxy_t operator=(proxy_t&& p) &&
      proxy_t operator=(proxy_t&& p)
      {
        // As the proxy is never meant to be an l-value self-assignment shouldn't
        // occur (if it does then it will put the tree in an invalid/corrupt state)
        assert(this != &p);

        return make_proxy<syntax::operator_binary>(language::id_assignment, move(), p.move());
      }
    };

    typedef proxy_t<detail::is_empty<D...>::value> proxy;

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

  protected:
    basic(core::qualifier::ptr&& qualifier, core::semantic_pair semantic) : variable(language::type_helper<T>{D...}, std::move(qualifier), semantic) {}
    basic(core::qualifier::ptr&& qualifier, core::semantic_pair semantic, syntax::expression::ptr&& initializer) : variable(language::type_helper<T>{D...}, std::move(qualifier), semantic, std::move(initializer)) {}

    syntax::expression::ptr make_reference() const
    {
      return syntax::expression::make<syntax::reference>(get_declaration()->inc_ref_count());
    }

    syntax::expression::ptr make_reference_or_temporary()
    {
      if(get_declaration()->get_ref_count() > 0)
      {
        return make_reference();
      }
      else
      {
        return make_temporary();
      }
    }

    template<typename T, typename ...A>
    static proxy make_proxy(A&& ...a)
    {
      return proxy(syntax::expression::make<T>(std::forward<A>(a)...));
    }
  };
}
