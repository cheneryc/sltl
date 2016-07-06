#pragma once

#include "action.h"
#include "expression.h"
#include "function_definition.h"


namespace sltl
{
namespace syntax
{
  class function_call : public expression
  {
  public:
    function_call(const function_definition& fd) : _fd(fd) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, nullptr);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, nullptr);
    }

    const wchar_t* get_function_name() const
    {
      return _fd._name.c_str();
    }

  private:
    const function_definition& _fd;
  };
}
}
