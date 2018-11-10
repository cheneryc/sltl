#pragma once

#include <type_traits>


namespace sltl
{
namespace detail
{
  template<typename T, template<typename> class Op, typename = std::void_t<>>
  struct detect : std::false_type {};

  template<typename T, template<typename> class Op>
  struct detect<T, Op, std::void_t<Op<T>>> : std::true_type {};
}
}
