#pragma once


namespace sltl
{
namespace detail
{
  template<bool, typename T1, typename T2>
  struct conditional
  {
    typedef T1 type;
  };

  template<typename T1, typename T2>
  struct conditional<false, T1, T2>
  {
    typedef T2 type;
  };

  template<typename T>
  using negate = typename conditional<T::value, std::false_type, std::true_type>::type;
}
}
