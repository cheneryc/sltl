#pragma once

#include "expression.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  class constructor_call : public expression
  {
  public:
    constructor_call(const language::type& type, expression_list&& args) : _type(type), _args(std::move(args)) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, &_args);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, &_args);
    }

    const language::type _type;

  private:
    expression_list _args;
  };
}
}
