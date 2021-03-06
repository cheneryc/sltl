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

  template<typename T, template<typename> class Op, typename = void_t<>>
  struct detect : std::false_type {};

  template<typename T, template<typename> class Op>
  struct detect<T, Op, void_t<Op<T>>> : std::true_type {};
}
}
