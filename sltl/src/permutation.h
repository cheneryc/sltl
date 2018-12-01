#pragma once

#include "syntax/operator_component_access.h"


namespace sltl
{
  namespace detail
  {
    template<size_t ...Args>
    struct is_permutation_scalar
    {
      static const bool value = (sizeof...(Args) > 1U);
    };
  }

  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D, size_t ...Args>
  class permutation
  {
    static_assert((sizeof...(Args) >= 1) && (sizeof...(Args) <= 4), "sltl::permutation: class may only have 1, 2, 3 or 4 template index arguments");

    typedef std::conditional_t<detail::is_permutation_scalar<Args...>::value, P<T, sizeof...(Args)>, S<T>> permutation_t;
    typedef std::conditional_t<detail::is_permutation_scalar<Args...>::value, expression::expression<P, T, sizeof...(Args)>, expression::expression<S, T>> permutation_proxy;

    // Every specialization of the permutation_group class template, including partial specializations, is a friend
    template<template<typename, size_t> class, template<typename> class, typename, size_t, size_t>
    friend class permutation_group;

  public:
    operator permutation_proxy() const &
    {
      return permutation_t::template make_proxy<syntax::operator_component_access>(expression::as_expression(_p).move(), make_accessor());
    }

    operator permutation_proxy() const &&
    {
      return permutation_t::template make_proxy<syntax::operator_component_access>(expression::as_expression(std::move(_p)).move(), make_accessor());
    }

    permutation_proxy operator=(permutation_proxy&& p) &
    {
      return permutation_t::template make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(expression::as_expression(_p).move(), make_accessor()), p.move());
    }

    permutation_proxy operator=(permutation_proxy&& p) &&
    {
      return permutation_t::template make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(expression::as_expression(std::move(_p)).move(), make_accessor()), p.move());
    }

    permutation_proxy operator=(permutation&& p)
    {
      return this->operator=(std::move(p).operator permutation_proxy());
    }

    permutation_proxy operator=(const permutation& p)
    {
      return this->operator=(p.operator permutation_proxy());
    }

  private:
    explicit permutation(P<T, D>& p) : _p(p) {}

    syntax::component_accessor::ptr make_accessor() const
    {
      return syntax::component_accessor::make<syntax::component_accessor::mode::vector>(Args...);
    }

    P<T, D>& _p;
  };
}
