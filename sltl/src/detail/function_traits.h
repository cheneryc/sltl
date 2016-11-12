#pragma once

#include "variadic_traits.h"


namespace sltl
{
namespace detail
{
  template<typename ...A>
  struct function_args
  {
  };

  // The default function_traits class. Used by lambdas and functors (but not function pointers).
  template<typename Fn>
  struct function_traits : function_traits<decltype(&Fn::operator())>
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

    typedef function_args<A...> args;
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

    typedef function_args<A...> args;
  };

  template<typename T, template<typename> class Op, typename = std::void_t<>>
  struct detect : std::false_type {};

  template<typename T, template<typename> class Op>
  struct detect<T, Op, std::void_t<Op<T>>> : std::true_type {};
}
}
