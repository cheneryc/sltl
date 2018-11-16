#pragma once

#include "basic.h"
#include "scalar.h"
#include "permutation_group.h"

#include "syntax/intrinsic_call.h"
#include "syntax/constructor_call.h"

#include "detail/variadic_traits.h"
#include "detail/conditional_traits.h"


namespace sltl
{
  template<typename T, size_t D>
  class vector : public basic<sltl::vector, T, D>, public permutation_group<sltl::vector, sltl::scalar, T, D>
  {
    static_assert((D >=2) && (D <= 4), "sltl::vector: template parameter D is only valid for values of 2, 3 and 4");

    typedef basic<sltl::vector, T, D> super_t;

  public:
    typedef typename super_t::proxy proxy;

    vector(proxy&& p) : super_t(p.move()), permutation_group<sltl::vector, sltl::scalar, T, D>(*this) {}
    vector(core::semantic_pair semantic = core::semantic_pair::none) : super_t(semantic), permutation_group<sltl::vector, sltl::scalar, T, D>(*this) {}

    vector(vector&& v) : vector(proxy(std::move(v))) {}
    vector(const vector& v) : vector(proxy(v)) {}

    explicit vector(syntax::parameter_declaration* pd) : super_t(pd), permutation_group<sltl::vector, sltl::scalar, T, D>(*this) {}

    // The TArg argument stops this conflicting with the default constructor
    // The disable_if is necessary to stop conflicts with the copy constructor. Ensuring is_empty is false is a good enough requirement as sltl doesn't allow 1D vectors
    template<typename TArg, typename ...A, detail::disable_if<detail::is_empty<A...>> = detail::default_tag>
    explicit vector(TArg&& t, A&&... a) : vector(proxy(syntax::expression::make<syntax::constructor_call>(language::type_helper<vector>(), unpack<D>(std::forward<TArg>(t), std::forward<A>(a)...)))) {}

    template<size_t DP, size_t ...Args, detail::enable_if<std::integral_constant<bool, D == sizeof...(Args)>> = detail::default_tag>
    vector(permutation<sltl::vector, sltl::scalar, T, DP, Args...>&& p) : vector(std::move(p).operator proxy()) {}

    template<size_t DP, size_t ...Args, detail::enable_if<std::integral_constant<bool, D == sizeof...(Args)>> = detail::default_tag>
    vector(const permutation<sltl::vector, sltl::scalar, T, DP, Args...>& p) : vector(p.operator proxy()) {}

    proxy operator=(proxy&& p)
    {
      //TODO: this could be called on an r-value? Should use make_reference_or_temporary instead?
      return super_t::template make_proxy<syntax::operator_binary>(language::id_assignment, variable::make_reference(), p.move());
    }

    proxy operator=(vector&& v)
    {
      return this->operator=(proxy(std::move(v)));
    }

    proxy operator=(const vector& v)
    {
      return this->operator=(proxy(v));
    }

    template<size_t DP, size_t ...Args>
    auto operator=(permutation<sltl::vector, sltl::scalar, T, DP, Args...>&& p) -> typename std::enable_if<D == sizeof...(Args), proxy>::type
    {
      return this->operator=(std::move(p).operator proxy());
    }

    template<size_t DP, size_t ...Args>
    auto operator=(const permutation<sltl::vector, sltl::scalar, T, DP, Args...>& p) -> typename std::enable_if<D == sizeof...(Args), proxy>::type
    {
      return this->operator=(p.operator proxy());
    }

  private:
    template<int N, typename ...A>
    syntax::expression_list unpack(typename scalar<T>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 1>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename vector<T, 2>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 2>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename vector<T, 3>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 3>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename vector<T, 4>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 4>(std::forward<A>(a)...));
    }

    template<int N>
    syntax::expression_list unpack()
    {
      static_assert(!(N > 0), "sltl::vector: too few vector components provided as arguments");
      static_assert(!(N < 0), "sltl::vector: too many vector components provided as arguments");

      return syntax::expression_list();
    }
  };

  namespace detail
  {
    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_dot(expression::expression<V, T, D...>&& x, expression::expression<V, T, D...>&& y) -> expression::expression<scalar, T>
    {
      static_assert(is_real<T>::value, "sltl::dot: template parameter T must be float or double");
      static_assert(sizeof...(D) < 2, "sltl::dot: only valid for scalar or vector arguments");

      return expression::expression<scalar, T>(syntax::call_intrinsic_dot(x.move(), y.move()));
    }

    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_normalize(expression::expression<V, T, D...>&& t) -> expression::expression<V, T, D...>
    {
      static_assert(is_real<T>::value, "sltl::normalize: template parameter T must be float or double");
      static_assert(sizeof...(D) < 2, "sltl::normalize: only valid for scalar or vector arguments");

      return expression::expression<V, T, D...>(syntax::call_intrinsic_normalize(t.move()));
    }

    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_clamp(expression::expression<V, T, D...>&& t, expression::expression<V, T, D...>&& min, expression::expression<V, T, D...>&& max) -> expression::expression<V, T, D...>
    {
      static_assert(is_real<T>::value, "sltl::clamp: template parameter T must be float or double");
      static_assert(sizeof...(D) < 2, "sltl::clamp: only valid for scalar or vector arguments");

      return expression::expression<V, T, D...>(syntax::call_intrinsic_clamp(t.move(), min.move(), max.move()));
    }

    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_lerp(expression::expression<V, T, D...>&& x, expression::expression<V, T, D...>&& y, expression::expression<scalar, T>&& s) -> expression::expression<V, T, D...>
    {
      static_assert(is_real<T>::value, "sltl::lerp: template parameter T must be float or double");
      static_assert(sizeof...(D) < 2, "sltl::lerp: only valid for scalar or vector arguments");

      return expression::expression<V, T, D...>(syntax::call_intrinsic_lerp(x.move(), y.move(), s.move()));
    }

    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_pow(expression::expression<V, T, D...>&& x, expression::expression<V, T, D...>&& y) -> expression::expression<V, T, D...>
    {
      static_assert(is_real<T>::value, "sltl::pow: template parameter T must be float or double");
      static_assert(sizeof...(D) < 2, "sltl::pow: only valid for scalar or vector arguments");

      return expression::expression<V, T, D...>(syntax::call_intrinsic_pow(x.move(), y.move()));
    }
  }

  template<typename T1, typename T2>
  auto dot(T1&& x, T2&& y) -> decltype(detail::as_proxy_dot(expression::as_expression(std::forward<T1>(x)), expression::as_expression(std::forward<T2>(y))))
  {
    auto x_proxy = expression::as_expression(std::forward<T1>(x));
    auto y_proxy = expression::as_expression(std::forward<T2>(y));

    return detail::as_proxy_dot(std::move(x_proxy), std::move(y_proxy));
  }

  template<typename T>
  auto normalize(T&& t) -> decltype(detail::as_proxy_normalize(expression::as_expression(std::forward<T>(t))))
  {
    return detail::as_proxy_normalize(expression::as_expression(std::forward<T>(t)));
  }

  template<typename T, typename TMin, typename TMax>
  auto clamp(T&& t, TMin&& min, TMax&& max) -> decltype(detail::as_proxy_clamp(expression::as_expression(std::forward<T>(t)), expression::as_expression(std::forward<TMin>(min)), expression::as_expression(std::forward<TMax>(max))))
  {
    auto min_proxy = expression::as_expression(std::forward<TMin>(min));
    auto max_proxy = expression::as_expression(std::forward<TMax>(max));

    return detail::as_proxy_clamp(expression::as_expression(std::forward<T>(t)), std::move(min_proxy), std::move(max_proxy));
  }

  template<typename T1, typename T2, typename T3>
  auto lerp(T1&& x, T2&& y, T3&& s) -> decltype(detail::as_proxy_lerp(expression::as_expression(std::forward<T1>(x)), expression::as_expression(std::forward<T2>(y)), expression::as_expression(std::forward<T3>(s))))
  {
    auto x_proxy = expression::as_expression(std::forward<T1>(x));
    auto y_proxy = expression::as_expression(std::forward<T2>(y));
    auto s_proxy = expression::as_expression(std::forward<T3>(s));

    return detail::as_proxy_lerp(std::move(x_proxy), std::move(y_proxy), std::move(s_proxy));
  }

  template<typename T1, typename T2>
  auto pow(T1&& x, T2&& y) -> decltype(detail::as_proxy_pow(expression::as_expression(std::forward<T1>(x)), expression::as_expression(std::forward<T2>(y))))
  {
    auto x_proxy = expression::as_expression(std::forward<T1>(x));
    auto y_proxy = expression::as_expression(std::forward<T2>(y));

    return detail::as_proxy_pow(std::move(x_proxy), std::move(y_proxy));
  }
}
