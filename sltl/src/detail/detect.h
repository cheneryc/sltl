#pragma once

#include <type_traits>


namespace sltl
{
namespace detail
{
  template<typename ...T>
  struct make_void
  {
    typedef void type;
  };

  template<typename ...T>
  using void_t = typename make_void<T...>::type;

  template<typename V, template<typename...> class Op, typename ...A>
  struct detect : std::false_type {};

  template<template<typename...> class Op, typename ...A>
  struct detect<void_t<Op<A...>>, Op, A...> : std::true_type {};
}
}
