#pragma once

#include "expression.h"

#include "../traits.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  template<typename T>
  class literal : public expression
  {
    static_assert(is_scalar<T>::value, "sltl::literal: Type T is not a valid template parameter type");

  public:
    literal(T t) : _t(t), _type(language::type_helper<T>()) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this);
    }

    virtual language::type get_type() const override
    {
      return _type;
    }

    const T _t;

  private:
    const language::type _type;
  };
}
}
