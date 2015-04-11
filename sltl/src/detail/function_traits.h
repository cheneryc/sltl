#pragma once

#include "variadic_traits.h"


namespace sltl
{
namespace detail
{
  // The default function_traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct function_traits : public function_traits<decltype(&Fn::operator())>
  {
    // Inherits from the specialization for a pointer-to-member-function (passing
    // the type of the object's function call operator as the template parameter)
  };

  // The specialization for a pointer-to-function
  template<typename R, typename ...A>
  struct function_traits<R(*)(A...)>
  {
    typedef R return_t;

    template<size_t N>
    struct arg
    {
      typedef typename get<N, A...>::type type;
    };
  };

  // The specialization for a pointer-to-member-function
  template<typename R, typename T, typename ...A>
  struct function_traits<R(T::*)(A...) const>
  {
    typedef R return_t;

    template<size_t N>
    struct arg
    {
      typedef typename get<N, A...>::type type;
    };
  };
}
}
