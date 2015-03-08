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
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const wchar_t* get_function_name() const
    {
      return _fd._name.c_str();
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, function_call>::value, bool>::type
    {
      return (act(type) && act(type, false));
    }

    const function_definition& _fd;
  };
}
}
