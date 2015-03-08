#pragma once

#include "action.h"
#include "expression.h"


namespace sltl
{
namespace syntax
{
  class parentheses : public expression
  {
  public:
    parentheses(expression::ptr&& e) : _e(std::move(e)) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, parentheses>::value, bool>::type
    {
      return (act(type) && (type._e ? type._e->apply_action(act) : true) && act(type, false));
    }

    expression::ptr _e;
  };
}
}
