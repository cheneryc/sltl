#pragma once

#include "action.h"
#include "expression.h"

#include "../traits.h"


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

    virtual bool apply_action(action& act) override
    {
      return act(*this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return cact(*this);
    }

    const T _t;
  };
}
}
