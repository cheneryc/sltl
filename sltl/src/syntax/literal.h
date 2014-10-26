#pragma once

#include "expression.h"

#include "../traits.h"
#include "../output.h"


namespace sltl
{
namespace syntax
{
  template<typename T>
  class literal : public expression
  {
    static_assert(is_scalar<T>::value, "sltl::literal: Type T is not a valid template parameter type");

  public:
    literal(T t) : _t(t) {}

    virtual void traverse(output& out) const
    {
      out(*this);
    }

    const T _t;
  };
}
}
