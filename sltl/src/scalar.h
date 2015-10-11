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

  template<typename T, size_t D = 1>
  class scalar : public basic<sltl::scalar, T, D>
  {
    static_assert(D == 1, "sltl::scalar: template parameter D must be 1");

  public:
    scalar(proxy&& p) : basic(core::qualifier::make<core::storage_qualifier>(core::qualifier_storage::default), core::semantic_pair::none, p.move()) {}
    scalar(core::qualifier_storage qualifier = core::qualifier_storage::default, core::semantic_pair semantic = core::semantic_pair::none) : basic(core::qualifier::make<core::storage_qualifier>(qualifier), semantic) {}

    scalar(scalar&& s) : scalar(proxy(std::move(s))) {}
    scalar(const scalar& s) : scalar(proxy(s)) {}

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
