#pragma once

#include "assert.h"

#include <type_traits>


namespace sltl
{
namespace detail
{
  // Gets the nth type template parameter from the specified parameter pack
  template<size_t N, typename T, typename ...A>
  struct get : get<N - 1, A...>
  {
    static_assert(sizeof...(A) >= N, "sltl::detail::get: the index parameter N is greater than the number of type template parameters");
  };

  template<typename T, typename ...A>
  struct get<0, T, A...>
  {
    typedef T type;
  };

  // Gets the nth value template parameter from the specified parameter pack
  template<size_t N, typename T, T V1, T ...V>
  struct get_value : get_value<N - 1, T, V...>
  {
    static_assert(sizeof...(V) >= N, "sltl::detail::get_value: the index parameter N is greater than the number of value template parameters");
  };

  template<typename T, T V1, T ...V>
  struct get_value<0, T, V1, V...>
  {
    static const T value = V1;
  };

  // Gets the nth type template parameter from the specified templated type
  template<size_t N, typename T>
  struct get_param;

  template<size_t N, template<typename...> class T, typename ...A>
  struct get_param<N, T<A...>>
  {
    typedef typename get<N, A...>::type type;
  };

  // Gets the nth value template parameter from the specified templated type
  template<size_t N, typename T1, typename T2>
  struct get_value_param;

  template<size_t N, typename T1, template<T1...> class T2, T1 ...V>
  struct get_value_param<N, T1, T2<V...>>
  {
    static const T1 value = get_value<N, T1, V...>::value;
  };

  template<typename T, typename ...A>
  struct get_index;

  template<typename T>
  struct get_index<T>
  {
    static_assert(fail<T>::value, "sltl::detail::get_index: template parameter T doesn't match any of the variadic template parameters");
  };

  template<typename T, typename ...A>
  struct get_index<T, T, A...>
  {
    static const size_t value = 0U;
  };

  template<typename T, typename T2, typename ...A>
  struct get_index<T, T2, A...>
  {
    static const size_t value = get_index<T, A...>::value + 1U; // Increment the value by one each time this specialization is used to get the correct index
  };

  template<typename ...A>
  struct is_empty
  {
    static const bool value = false;
  };

  template<>
  struct is_empty<>
  {
    static const bool value = true;
  };

  template<size_t ...D>
  struct is_empty_values
  {
    static const bool value = false;
  };

  template<>
  struct is_empty_values<>
  {
    static const bool value = true;
  };
}
}
