#pragma once

#include "action.h"
#include "expression.h"

#include <type.h>


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

    virtual language::type get_type() const override
    {
      return _e->get_type();
    }

  private:
    expression::ptr _e;
  };
}
}
