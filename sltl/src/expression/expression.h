#pragma once

#include "../syntax/literal.h"
#include "../syntax/expression.h"
#include "../syntax/expression_statement.h"
#include "../syntax/operator.h"

#include <cassert>


namespace sltl
{
namespace expression
{
  template<template<typename, size_t...> class V, typename T, size_t ...D>
  class expression
  {
  public:
    expression(syntax::expression::ptr&& exp) : _exp(std::move(exp)) {}

    expression(expression&& exp) : expression(std::move(exp._exp)) {}
    expression(V<T, D...>&& v) : expression(v.make_reference_or_temporary()) {}
    expression(const V<T, D...>& v) : expression(v.make_reference()) {}

    ~expression()
    {
      if(_exp)
      {
        syntax::get_current_block().add<syntax::expression_statement>(std::move(_exp));
      }
    }

    //TODO: C++11, this function should only be callable for r-value references. Its signature should be proxy_t operator=(proxy_t&& p) &&
    expression operator=(expression&& exp)
    {
      // As the proxy is never meant to be an l-value self-assignment shouldn't
      // occur (if it does then it will put the tree in an invalid/corrupt state)
      assert(this != &exp);

      return expression(syntax::expression::make<syntax::operator_binary>(language::id_assignment, move(), exp.move()));
    }

    // Non-copyable and non-assignable
    expression(const expression&) = delete;
    expression& operator=(const expression&) = delete;

    syntax::expression::ptr&& move()
    {
      return std::move(_exp);
    }

  private:
    syntax::expression::ptr _exp;
  };

  // Partial specialization for scalar types that has a conversion constructor accepting type T
  template<template<typename> class V, typename T>
  class expression<V, T>
  {
  public:
    expression(syntax::expression::ptr&& exp) : _exp(std::move(exp)) {}

    expression(T t) : expression(syntax::expression::make<syntax::literal<T>>(t)) {}
    expression(expression&& exp) : expression(std::move(exp._exp)) {}
    expression(V<T>&& v) : expression(v.make_reference_or_temporary()) {}
    expression(const V<T>& v) : expression(v.make_reference()) {}

    ~expression()
    {
      if(_exp)
      {
        syntax::get_current_block().add<syntax::expression_statement>(std::move(_exp));
      }
    }

    //TODO: C++11, this function should only be callable for r-value references. Its signature should be proxy_t operator=(proxy_t&& p) &&
    expression operator=(expression&& exp)
    {
      // As the proxy is never meant to be an l-value self-assignment shouldn't
      // occur (if it does then it will put the tree in an invalid/corrupt state)
      assert(this != &exp);

      return expression(syntax::expression::make<syntax::operator_binary>(language::id_assignment, move(), exp.move()));
    }

    // Non-copyable and non-assignable
    expression(const expression&) = delete;
    expression& operator=(const expression&) = delete;

    syntax::expression::ptr&& move()
    {
      return std::move(_exp);
    }

  private:
    syntax::expression::ptr _exp;
  };

  template<template<typename, size_t...> class V, typename T, size_t ...D>
  auto as_expression(V<T, D...>&& v) -> expression<V, T, D...>
  {
    return expression<V, T, D...>(std::move(v));
  }

  template<template<typename, size_t...> class V, typename T, size_t ...D>
  auto as_expression(const V<T, D...>& v) -> expression<V, T, D...>
  {
    return expression<V, T, D...>(v);
  }

  template<template<typename, size_t...> class V, typename T, size_t ...D>
  auto as_expression(expression<V, T, D...>&& p) -> expression<V, T, D...>
  {
    return std::move(p);
  }
}
}
