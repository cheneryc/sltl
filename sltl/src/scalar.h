#pragma once

#include "basic.h"
#include "permutation.h"

#include "syntax/variable_declaration.h"


namespace sltl
{
  //TODO: what about constant scalar variables??
  //eg.
  // shader::const_scalar<float> my_const = 1.0f;
  // my_const += /*expression*/; // Error, this is not allowed
  //
  // alternatively:
  // sltl::scalar<const float> my_const = 1.0f;
  //
  // this would:
  //1. //use partial template specialization to remove non-const operators etc.
  //2. require a 'specifier' field adding to variable declaration etc.

  template<typename T>
  class scalar : public basic<sltl::scalar, T>
  {
    typedef basic<sltl::scalar, T> super_t;

  public:
    typedef typename super_t::proxy proxy;

    scalar(proxy&& p) : super_t(p.move()) {}
    scalar(core::semantic_pair semantic = core::semantic_pair::none) : super_t(semantic) {}

    scalar(T t) : scalar(proxy(t)) {}

    scalar(scalar&& s) : scalar(proxy(std::move(s))) {}
    scalar(const scalar& s) : scalar(proxy(s)) {}

    explicit scalar(syntax::parameter_declaration* pd) : super_t(pd) {}

    template<template<typename, size_t> class V, size_t DP, size_t Arg>
    scalar(permutation<V, sltl::scalar, T, DP, Arg>&& p) : scalar(std::move(p).operator proxy()) {}

    template<template<typename, size_t> class V, size_t DP, size_t Arg>
    scalar(const permutation<V, sltl::scalar, T, DP, Arg>& p) : scalar(p.operator proxy()) {}

    proxy operator=(proxy&& p)
    {
      return super_t::template make_proxy<syntax::operator_binary>(language::id_assignment, variable::make_reference(), p.move());
    }

    proxy operator=(T t)
    {
      return this->operator=(proxy(t));
    }

    proxy operator=(scalar&& s)
    {
      return this->operator=(proxy(std::move(s)));
    }

    proxy operator=(const scalar& s)
    {
      return this->operator=(proxy(s));
    }

    template<template<typename, size_t> class V, size_t DP, size_t Arg>
    proxy operator=(permutation<V, sltl::scalar, T, DP, Arg>&& p)
    {
      return this->operator=(std::move(p).operator proxy());
    }

    template<template<typename, size_t> class V, size_t DP, size_t Arg>
    proxy operator=(const permutation<V, sltl::scalar, T, DP, Arg>& p)
    {
      return this->operator=(p.operator proxy());
    }

    //TODO: disable comparision operators for scalar<bool>

    friend expression::expression<sltl::scalar, bool> operator<(proxy&& p1, proxy&& p2)
    {
      return basic<sltl::scalar, bool>::make_proxy<syntax::operator_binary>(language::id_lt, p1.move(), p2.move());
    }

    friend expression::expression<sltl::scalar, bool> operator>(proxy&& p1, proxy&& p2)
    {
      return basic<sltl::scalar, bool>::make_proxy<syntax::operator_binary>(language::id_gt, p1.move(), p2.move());
    }

    friend expression::expression<sltl::scalar, bool> operator<=(proxy&& p1, proxy&& p2)
    {
      return basic<sltl::scalar, bool>::make_proxy<syntax::operator_binary>(language::id_lt_eq, p1.move(), p2.move());
    }

    friend expression::expression<sltl::scalar, bool> operator>=(proxy&& p1, proxy&& p2)
    {
      return basic<sltl::scalar, bool>::make_proxy<syntax::operator_binary>(language::id_gt_eq, p1.move(), p2.move());
    }
  };

  namespace expression
  {
    template<typename T>
    auto as_expression(T t) -> typename std::enable_if<detail::is_scalar<T>::value, expression<sltl::scalar, T>>::type
    {
      return expression<sltl::scalar, T>(t);
    }
  }
}
