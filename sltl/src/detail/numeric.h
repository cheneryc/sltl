#pragma once

#include <type_traits>

#include <cmath>


namespace sltl
{
namespace detail
{
  template<typename T>
  auto has_fractional_part(T t) -> typename std::enable_if<std::is_floating_point<T>::value, bool>::type
  {
    T value_integral;
    T value_fractional = std::modf(t, &value_integral);

    return (value_fractional != T());
  }
}
}
