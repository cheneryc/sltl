#pragma once

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
      return apply_action_impl(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this);
    }

    const T _t;
  };
}
}
