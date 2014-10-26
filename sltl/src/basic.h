#pragma once

#include "traits.h"
#include "variable.h"

#include "syntax/literal.h"
#include "syntax/operator.h"
#include "syntax/reference.h"
#include "syntax/temporary.h"
#include "syntax/expression_statement.h"


namespace sltl
{
  template<template<typename, size_t> class V, typename T, size_t D>
  class basic : public variable
  {
    static_assert(is_scalar<T>::value, "sltl::basic: Type T is not a valid template parameter type");

  public:
    basic(const basic&) = delete;
    basic& operator=(const basic&) = delete;

    class proxy
    {
    public:
      proxy(syntax::expression::ptr&& e) : _expression(std::move(e)) {}

      //TODO: this is probably not suitable for vector?
      proxy(T t) : proxy(syntax::expression::make<syntax::literal<T>>(t)) {}
      proxy(proxy&& p) : proxy(std::move(p._expression)) {}
      proxy(V<T, D>&& v) : proxy(syntax::expression::make<syntax::temporary>(std::move(v._declaration))) {}
      proxy(const V<T, D>& v) : proxy(v.make_reference()) {}

      ~proxy()
      {
        if(_expression)
        {
          syntax::get_current_block().add<syntax::expression_statement>(std::move(_expression));
        }
      }

      syntax::expression::ptr&& move()
      {
        return std::move(_expression);
      }

      proxy(const proxy& p) = delete;
      proxy& operator=(const proxy&) = delete;

    private:
      syntax::expression::ptr _expression;
    };

    proxy operator+=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment_addition, make_reference(), p.move());
    }

    friend proxy operator+(proxy&& lhs, proxy&& rhs)
    {
      return make_proxy<syntax::binary_operator>(language::id_addition, lhs.move(), rhs.move());
    }

    proxy operator-=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment_subtraction, make_reference(), p.move());
    }

    friend proxy operator-(proxy&& lhs, proxy&& rhs)
    {
      return make_proxy<syntax::binary_operator>(language::id_subtraction, lhs.move(), rhs.move());
    }

    syntax::expression::ptr make_reference() const
    {
      return syntax::expression::make<syntax::reference>(_declaration);
    }

  protected:
    basic(language::type_id id) : variable(id) {}
    basic(language::type_id id, syntax::expression::ptr&& initializer) : variable(id, std::move(initializer)) {}

    template<typename T, typename ...A>
    static proxy make_proxy(A&& ...a)
    {
      return proxy(syntax::expression::make<T>(std::forward<A>(a)...));
    }
  };
}
