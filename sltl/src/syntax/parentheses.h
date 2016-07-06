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
      return apply_action_impl(act, *this, _e.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _e.get());
    }

  private:
    expression::ptr _e;
  };
}
}
