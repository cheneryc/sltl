#pragma once

#include "variadic_traits.h"


namespace sltl
{
namespace detail
{
  template<typename ...P>
  struct function_params {};

  // The default function_traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct function_traits : function_traits<decltype(&Fn::operator())>
  {
    // Inherits from the specialization for a pointer-to-member-function (passing
    // the type of the object's function call operator as the template parameter)
  };

  // The specialization for a pointer-to-function
  template<typename R, typename ...P>
  struct function_traits<R(*)(P...)>
  {
    typedef R return_t;

    template<size_t N>
    struct param
    {
      typedef typename get<N, P...>::type type;
    };

    typedef function_params<P...> params_t;

    static const size_t param_count = sizeof...(P);
  };

  // The specialization for a pointer-to-member-function
  template<typename R, typename T, typename ...P>
  struct function_traits<R(T::*)(P...) const>
  {
    typedef R return_t;

    template<size_t N>
    struct param
    {
      typedef typename get<N, P...>::type type;
    };

    typedef function_params<P...> params_t;

    static const size_t param_count = sizeof...(P);
  };

  template<typename Fn, size_t N>
  constexpr bool is_function_param_index()
  {
    return N < function_traits<Fn>::param_count;
  }
}
}
