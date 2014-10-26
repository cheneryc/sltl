#pragma once

#include <type_traits>


namespace sltl
{
  template<typename T>
  struct is_integer
  {
    static const bool value = (std::is_same<int, T>::value || std::is_same<unsigned int, T>::value);
  };

  template<typename T>
  struct is_real
  {
    // Don't use std::is_floating_point as it accepts long double types
    static const bool value = (std::is_same<float, T>::value || std::is_same<double, T>::value);
  };

  template<typename T>
  struct is_scalar
  {
    static const bool value = (is_integer<T>::value || is_real<T>::value || std::is_same<bool, T>::value);
  };
}