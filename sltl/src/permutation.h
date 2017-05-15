#pragma once

#include "syntax/operator_component_access.h"


namespace sltl
{
  //TODO: static_assert that sizeof Args is >= 1 && <= 4
  //TODO: static_assert that each element in Args.. is >= 0 && < 4
  template<template<typename, size_t> class P, typename T, size_t D, size_t ...Args>
  class permutation
  {
    typedef P<T, D> permutable_t;
    typedef P<T, sizeof...(Args)> permuted_t;
    typedef expression::expression<P, T, D> proxy_in_t;
    typedef expression::expression<P, T, sizeof...(Args)> proxy_out_t;

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

    // Every specialization of the permutation_group class template, including partial specializations, is a friend
    template<template<typename, size_t> class, typename, size_t, size_t>
    friend class permutation_group;

    syntax::component_accessor make_accessor() const
    {
      return syntax::component_accessor::make<syntax::component_access::mode::vector>(Args...);
    }

    permutable_t& _p;//TODO: does this need to be mutable (or should operator proxy_t() const && use const_cast), so const methods can still get a non-const ref to this member? seems to work at the moment without this but could be a compiler issue?
  };

  //TODO: static_assert that Arg is >= 0 && < 4
  // Partial specialization for scalar permutations (NOT scalar operands)
  template<template<typename, size_t> class P, typename T, size_t D, size_t Arg>
  class permutation<P, T, D, Arg>
  {
    typedef P<T, D> permutable_t;
    typedef sltl::scalar<T> permuted_t;
    typedef expression::expression<P, T, D> proxy_in_t;
    typedef expression::expression<sltl::scalar, T> proxy_out_t;

  public:
    operator proxy_out_t() const &
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(_p).move(), make_accessor());
    }

    operator proxy_out_t() const &&
    {
      return permuted_t::make_proxy<syntax::operator_component_access>(proxy_in_t(std::move(_p)).move(), make_accessor());
    }

    //TODO: would creating a make_operator_component_access function overloaded on & and && allow the removal of these overloads?
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

    // Every specialization of the permutation_group class template, including partial specializations, is a friend
    template<template<typename, size_t> class, typename, size_t, size_t>
    friend class permutation_group;

    syntax::component_accessor make_accessor() const
    {
      return syntax::component_accessor::make<syntax::component_access::mode::vector>(Arg);
    }

    permutable_t& _p;//TODO: does this need to be mutable (or should operator proxy_t() const && use const_cast), so const methods can still get a non-const ref to this member? seems to work at the moment without this but could be a compiler issue?
  };

  template<template<typename, size_t> class P, typename T, size_t D, size_t = D>
  class permutation_group;

  // Partial specialization for zero dimensions
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 0>
  {
  protected:
    typedef typename P<T, D> permutable_t;
  };

  // Partial specialization for 1D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 1> : public permutation_group<P, T, D, 0>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 0>(), x(p) {}

    union
    {
      permutation<P, T, D, 0> x;
      permutation<P, T, D, 0, 0> xx;
      permutation<P, T, D, 0, 0, 0> xxx;
      permutation<P, T, D, 0, 0, 0, 0> xxxx;
    };
  };

  // Partial specialization for 2D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 2> : public permutation_group<P, T, D, 1>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 1>(p), y(p) {}

    union
    {
      //TODO: add all the missing permutations
      permutation<P, T, D, 1> y;
      permutation<P, T, D, 1, 1> yy;
      permutation<P, T, D, 1, 1, 1> yyy;
      permutation<P, T, D, 1, 1, 1, 1> yyyy;
    };
  };

  // Partial specialization for 3D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 3> : public permutation_group<P, T, D, 2>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 2>(p), z(p) {}

    union
    {
      //TODO: add all the missing permutations
      permutation<P, T, D, 2> z;
      permutation<P, T, D, 2, 2> zz;
      permutation<P, T, D, 2, 2, 2> zzz;
      permutation<P, T, D, 2, 2, 2, 2> zzzz;
    };
  };

  // Partial specialization for 4D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 4> : public permutation_group<P, T, D, 3>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 3>(p), w(p) {}

    union
    {
      //TODO: add all the missing permutations
      permutation<P, T, D, 3> w;
      permutation<P, T, D, 3, 3> ww;
      permutation<P, T, D, 3, 3, 3> www;
      permutation<P, T, D, 3, 3, 3, 3> wwww;
    };
  };
}
