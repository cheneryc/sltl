#pragma once

#include "node.h"
#include "list.h"
#include "declaration.h"

#include "../output.h"
#include "../language.h"

#include <memory>


namespace sltl
{
namespace syntax
{
  class parameter_declaration : public declaration, public node
  {
  public:
    typedef std::unique_ptr<parameter_declaration> ptr;

    //TODO: real values for the parameter name and type
    parameter_declaration() : declaration(std::wstring(L"?")), _type(language::type_helper<void>()) {}

    virtual void traverse(output& out) const
    {
      out(*this);
    }

    const language::type _type;
  };

  class parameter_list : public list<parameter_declaration, node> {};
}
}
