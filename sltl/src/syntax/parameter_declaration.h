#pragma once

#include "node.h"
#include "list.h"
#include "action.h"
#include "declaration.h"

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

    virtual bool apply_action(action& act) override
    {
      return act(*this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return cact(*this);
    }

    const language::type _type;
  };

  class parameter_list : public list<parameter_declaration, node> {};
}
}
