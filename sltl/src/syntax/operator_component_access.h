#pragma once

#include "operator.h"
#include "action.h"
#include "component_accessor.h"

#include <language.h>


namespace sltl
{
namespace syntax
{
  class operator_component_access : public operator_base
  {
  public:
    operator_component_access(expression::ptr&& operand, component_accessor::ptr&& accessor) : _operand(std::move(operand)), _accessor(std::move(accessor)), _type(_accessor->get_type(_operand->get_type()))
    {
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _operand.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _operand.get());
    }

    virtual language::type get_type() const override
    {
      return _type;
    }

  private:
    expression::ptr _operand;

  public:
    const component_accessor::ptr _accessor;

  private:
    const language::type _type;
  };
}
}
