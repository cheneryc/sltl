#pragma once


namespace sltl
{
namespace detail
{
  // Gets the nth type template parameter from the specified parameter pack
  template<size_t N, typename T, typename ...A>
  struct get : public get<N - 1, A...>
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
  struct get_value : public get_value<N - 1, T, V...>
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
  struct get_param
  {
  };

  template<size_t N, template<typename...> class T, typename ...A>
  struct get_param<N, T<A...>>
  {
    typedef typename get<N, A...>::type type;
  };

  // Gets the nth value template parameter from the specified templated type
  template<size_t N, typename T1, typename T2>
  struct get_value_param
  {
  };

  template<size_t N, typename T1, template<T1...> class T2, T1 ...V>
  struct get_value_param<N, T1, T2<V...>>
  {
    static const T1 value = get_value<N, T1, V...>::value;
  };
}
}
