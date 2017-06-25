#pragma once

#include "syntax/operator_component_access.h"


namespace sltl
{
  template<template<typename, size_t> class P, typename T, size_t D, size_t ...Args>
  class permutation
  {
    static_assert((sizeof...(Args) >= 2) && (sizeof...(Args) <= 4), "sltl::permutation: class may only have 2, 3 or 4 template index arguments");

    typedef P<T, D> permutable_t;
    typedef P<T, sizeof...(Args)> permuted_t;
    typedef expression::expression<P, T, D> proxy_in_t;
    typedef expression::expression<P, T, sizeof...(Args)> proxy_out_t;

    // Every specialization of the permutation_group class template, including partial specializations, is a friend
    template<template<typename, size_t> class, typename, size_t, size_t>
    friend class permutation_group;

  public:
    operator proxy_out_t() const &
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(_p).move(), make_accessor());
    }

    operator proxy_out_t() const &&
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(std::move(_p)).move(), make_accessor());
    }

    proxy_out_t operator=(proxy_out_t&& p) &
    {
      return permuted_t::make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(proxy_in_t(_p).move(), make_accessor()), p.move());
    }

    proxy_out_t operator=(proxy_out_t&& p) &&
    {
      return permuted_t::make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(proxy_in_t(std::move(_p)).move(), make_accessor()), p.move());
    }

    proxy_out_t operator=(permutation&& p)
    {
      return this->operator=(static_cast<proxy_out_t>(std::move(p)));
    }

    proxy_out_t operator=(const permutation& p)
    {
      return this->operator=(static_cast<proxy_out_t>(p));
    }

  private:
    explicit permutation(permutable_t& p) : _p(p) {}

    syntax::component_accessor::ptr make_accessor() const
    {
      return syntax::component_accessor::make<syntax::component_accessor::mode::vector>(Args...);
    }

    permutable_t& _p;
  };

  // Partial specialization for scalar permutations (NOT scalar operands)
  template<template<typename, size_t> class P, typename T, size_t D, size_t Arg>
  class permutation<P, T, D, Arg>
  {
    typedef P<T, D> permutable_t;
    typedef sltl::scalar<T> permuted_t;
    typedef expression::expression<P, T, D> proxy_in_t;
    typedef expression::expression<sltl::scalar, T> proxy_out_t;

    // Every specialization of the permutation_group class template, including partial specializations, is a friend
    template<template<typename, size_t> class, typename, size_t, size_t>
    friend class permutation_group;

  public:
    operator proxy_out_t() const &
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(_p).move(), make_accessor());
    }

    operator proxy_out_t() const &&
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(std::move(_p)).move(), make_accessor());
    }

    proxy_out_t operator=(proxy_out_t&& p) &
    {
      return permuted_t::make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(proxy_in_t(_p).move(), make_accessor()), p.move());
    }

    proxy_out_t operator=(proxy_out_t&& p) &&
    {
      return permuted_t::make_proxy<syntax::operator_binary>(language::id_assignment, syntax::expression::make<syntax::operator_component_access>(proxy_in_t(std::move(_p)).move(), make_accessor()), p.move());
    }

    proxy_out_t operator=(permutation&& p)
    {
      return this->operator=(static_cast<proxy_out_t>(std::move(p)));
    }

    proxy_out_t operator=(const permutation& p)
    {
      return this->operator=(static_cast<proxy_out_t>(p));
    }

  private:
    explicit permutation(permutable_t& p) : _p(p) {}

    syntax::component_accessor::ptr make_accessor() const
    {
      return syntax::component_accessor::make<syntax::component_accessor::mode::vector>(Arg);
    }

    permutable_t& _p;
  };
}
