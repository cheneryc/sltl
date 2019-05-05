#pragma once

#include <algorithm>


namespace sltl
{
namespace detail
{
  // Does nothing, useful as parameter pack expansion is only allowed in certain
  // contexts (function argument lists are one of the acceptable expansion loci)
  template<typename ...A>
  inline void pass(A&&...) {}

  template<typename ...A>
  struct variadic_guard
  {
  };

  template<typename ...A>
  struct all : std::true_type
  {
  };

  template<typename T, typename ...A>
  struct all<T, A...> : std::conditional<T::value, all<A...>, std::false_type>::type
  {
  };

  template<typename ...A>
  struct any : std::false_type
  {
  };

  template<typename T, typename ...A>
  struct any<T, A...> : std::conditional<T::value, std::true_type, any<A...>>::type
  {
  };

  template<size_t ...Ns>
  struct max_impl
  {
  };

  template<size_t N, size_t ...Ns>
  struct max_impl<N, Ns...>
  {
    static const size_t value = std::max(N, max_impl<Ns...>::value);
  };

  template<>
  struct max_impl<>
  {
    static const size_t value = 0U;
  };

  template<size_t ...N>
  struct max : max_impl<N...>
  {
    static_assert(sizeof...(N) > 0U, "sltl::detail::max: variadic template parameter must contain at least one element");
  };

  template<size_t ...Ns>
  struct min_impl
  {
  };

  template<size_t N, size_t ...Ns>
  struct min_impl<N, Ns...>
  {
    static const size_t value = std::min(N, min_impl<Ns...>::value);
  };

  template<>
  struct min_impl<>
  {
    static const size_t value = 0U;
  };

  template<size_t ...N>
  struct min : min_impl<N...>
  {
    static_assert(sizeof...(N) > 0U, "sltl::detail::min: variadic template parameter must contain at least one element");
  };
}
}
