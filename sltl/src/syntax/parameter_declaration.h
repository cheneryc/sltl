#pragma once

#include "node.h"
#include "list.h"
#include "action.h"
#include "declaration.h"

#include "../language.h"

#include <memory>
#include <algorithm>


namespace sltl
{
namespace syntax
{
  class parameter_declaration : public declaration, public node
  {
  public:
    typedef std::unique_ptr<parameter_declaration> ptr;

    parameter_declaration(std::wstring&& name, const language::type& type) : declaration(std::move(name)), _type(type)
    {
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this);
    }

    language::type get_type() const override
    {
      return _type;
    }

    const language::type _type;
  };

  class parameter_list : public list<parameter_declaration, node>
  {
    typedef list<parameter_declaration, node> super_t;

  public:
    parameter_list() : super_t() {}
    parameter_list(parameter_list&& list) : super_t(std::move(list)) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _list_items.begin(), _list_items.end());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _list_items.begin(), _list_items.end());
    }

    friend bool operator<(const parameter_list& pl1, const parameter_list& pl2)
    {
      auto fn_compare = [](const parameter_declaration::ptr& pd1, const parameter_declaration::ptr& pd2)
      {
        return pd1->get_type() < pd2->get_type();
      };

      return std::lexicographical_compare(pl1.begin(), pl1.end(), pl2.begin(), pl2.end(), fn_compare);
    }

    friend bool operator>(const parameter_list& pl1, const parameter_list& pl2)
    {
      return pl2 < pl1;
    }

    friend bool operator<=(const parameter_list& pl1, const parameter_list& pl2)
    {
      return !(pl1 > pl2);
    }

    friend bool operator>=(const parameter_list& pl1, const parameter_list& pl2)
    {
      return !(pl1 < pl2);
    }
  };
}
}
