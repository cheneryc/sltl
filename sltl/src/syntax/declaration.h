#pragma once

#include "statement.h"

#include <string>


namespace sltl
{
namespace syntax
{
  class declaration : public statement
  {
  public:
    const std::wstring _name;

  protected:
    declaration(std::wstring&& name) : statement(), _name(std::move(name)) {}
  };
}
}
