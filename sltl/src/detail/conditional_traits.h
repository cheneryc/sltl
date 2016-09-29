#pragma once

#include <type_traits>


namespace sltl
{
namespace detail
{
  template<typename T>
  using negate = typename std::conditional<T::value, std::false_type, std::true_type>::type;

  enum class enable_if_tag
  {
  };

  static const enable_if_tag default_tag = {};

  template<typename T>
  using enable_if = typename std::enable_if<T::value, enable_if_tag>::type;

  template<typename T>
  using disable_if = enable_if<negate<T>>;
}
}
