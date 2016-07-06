#pragma once

#include <type_traits>


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

  template<typename T1, typename T2>
  struct same_const : conditional<std::is_const<T1>::value, const T2, T2>
  {
    // Pointer and reference types are excluded as the resulting type is unlikely to be that expected by the calling code
    // pointer types: if allowed would convert T2* into T2* const rather than const T2*
    // reference types: if allowed would convert T2& into T2& const rather than const T2&
    static_assert(!std::is_pointer<T2>::value && !std::is_reference<T2>::value, "sltl::detail::same_const: Type T2 cannot be a pointer or reference type as these types exhibit surprising behaviour when used with this class.");
  };

  template<typename T, bool = std::is_pointer<T>::value>
  struct same_const_pointer_helper {};

  template<typename T>
  struct same_const_pointer_helper<T, true>
  {
    typedef typename std::add_const<typename std::remove_pointer<T>::type>::type* type;
  };

  template<typename T1, typename T2>
  struct same_const_pointer : conditional<std::is_const<T1>::value, typename same_const_pointer_helper<T2>::type, T2> {};

  template<typename T, bool = std::is_reference<T>::value>
  struct same_const_reference_helper {};

  template<typename T>
  struct same_const_reference_helper<T, true>
  {
    typedef typename std::add_const<typename std::remove_reference<T>::type>::type& type;
  };

  template<typename T1, typename T2>
  struct same_const_reference : conditional<std::is_const<T1>::value, typename same_const_reference_helper<T2>::type, T2> {};
}
}
