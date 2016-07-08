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
      return apply_action_impl(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this);
    }

    const language::type _type;
  };

  class parameter_list : public list<parameter_declaration, node>
  {
  public:
    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _list_items.begin(), _list_items.end());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _list_items.begin(), _list_items.end());
    }
  };
}
}
