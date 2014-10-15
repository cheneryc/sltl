#pragma once

#include "basic.h"

#include "syntax/variable_declaration.h"


namespace sltl
{
  //TODO: what about constant scalar variables??
  //eg.
  // shader::const_scalar<float> my_const = 1.0f;
  // my_const += /*expression*/; // Error, this is not allowed
  //
  // alternatively:
  // sltl::scalar<const float> my_const = 1.0f;
  //
  // this would:
  //1. //use partial template specialization to remove non-const operators etc.
  //2. require a 'specifier' field adding to variable declaration etc.

  template<typename T>
  struct scalar_id
  {
    static const language::type_id value = language::id_unknown;
  };

  template<>
  struct scalar_id<float>
  {
    static const language::type_id value = language::id_float;
  };

  template<>
  struct scalar_id<double>
  {
    static const language::type_id value = language::id_double;
  };

  template<>
  struct scalar_id<int>
  {
    static const language::type_id value = language::id_int;
  };

  template<>
  struct scalar_id<unsigned int>
  {
    static const language::type_id value = language::id_uint;
  };

  template<>
  struct scalar_id<bool>
  {
    static const language::type_id value = language::id_bool;
  };

  template<typename T, size_t D = 1>
  class scalar : public basic<sltl::scalar, T, D>
  {
    static_assert(D == 1, "sltl::scalar: template parameter D must be 1");

  public:
    scalar() : basic(scalar_id<T>::value) {}
    scalar(scalar&& s) : scalar(proxy(std::move(s))) {}
    scalar(const scalar& s) : scalar(proxy(s)) {}

    scalar(proxy&& p) : scalar()
    {
      _vd->add(p.move());
    }

    //TODO: see if the assignment opertators can be moved into basic and
    //then used to mask the compiler defaults created for scalar/vector
    proxy operator=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment, make_reference(), p.move());
    }

    proxy operator=(scalar&& s)
    {
      return this->operator=(proxy(std::move(s)));
    }

    proxy operator=(const scalar& s)
    {
      return this->operator=(proxy(s));
    }
  };
}
