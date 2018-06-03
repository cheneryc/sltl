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

  template<typename T, template<typename> class Op, typename = std::void_t<>>
  struct detect : std::false_type {};

  template<typename T, template<typename> class Op>
  struct detect<T, Op, std::void_t<Op<T>>> : std::true_type {};
}
}
