#pragma once

#include "expression.h"
#include "function_definition.h"

#include <language.h>


namespace sltl
{
namespace syntax
{
  class action;

  class function_call : public expression
  {
  public:
    function_call(const function_definition& fd, expression_list&& args) : _fd(fd), _args(std::move(args))
    {
      const parameter_list& params = fd.get_params();

      if (_args.size() != params.size())
      {
        throw std::exception();//TODO: exception type and message
      }

      auto fn_equal = [](const expression::ptr& exp, const parameter_declaration::ptr& pd)
      {
        return (exp->get_type() == pd->get_type());
      };

      if (!std::equal(_args.begin(), _args.end(), params.begin(), params.end(), fn_equal))
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, &_args);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, &_args);
    }

    virtual language::type get_type() const override
    {
      return _fd.get_type();
    }

    const wchar_t* get_function_name() const
    {
      return _fd._name.c_str();
    }

  private:
    const function_definition& _fd;
    expression_list _args;
  };
}
}
