#pragma once

#include "expression.h"
#include "function_definition.h"

#include "../output.h"


namespace sltl
{
namespace syntax
{
  class function_call : public expression
  {
  public:
    function_call(const function_definition& fd) : _fd(fd) {}

    virtual void traverse(output& out) const
    {
      out(*this);
      out(*this, false);
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
