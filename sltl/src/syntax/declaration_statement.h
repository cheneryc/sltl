#pragma once

#include "statement.h"
#include "declaration.h"


namespace sltl
{
namespace syntax
{
  class declaration_statement : public declaration, public statement
  {
  public:
    declaration_statement(std::wstring&& name) : declaration(std::move(name)) {}
  };
}
}
