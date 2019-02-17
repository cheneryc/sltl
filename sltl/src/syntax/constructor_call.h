#pragma once

#include "expression.h"

#include <type.h>


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

    void set_type(const language::type& type)
    {
      _type = type;
    }

    virtual language::type get_type() const override
    {
      return _type;
    }

  private:
    language::type _type;
    expression_list _args;
  };
}
}
