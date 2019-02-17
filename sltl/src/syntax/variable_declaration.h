#pragma once

#include "expression.h"
#include "declaration_statement.h"

#include <type.h>

#include <core/semantic.h>
#include <core/qualifier.h>


namespace sltl
{
namespace syntax
{
  class variable_declaration : public declaration_statement
  {
  public:
    variable_declaration(std::wstring&& name, expression::ptr&& initializer);
    variable_declaration(std::wstring&& name, const language::type& type, core::qualifier_storage qualifier, core::semantic_pair semantic);

    bool has_type() const
    {
      return static_cast<bool>(_type);
    }

    bool has_initializer() const
    {
      return static_cast<bool>(_initializer);
    }

    expression::ptr&& move()
    {
      return std::move(_initializer);
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _initializer.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _initializer.get());
    }

    void set_type(const language::type& type)
    {
      if(_initializer)
      {
        throw std::exception();//TODO: exception type and message
      }

      *_type = type;
    }

    virtual language::type get_type() const override
    {
      return (_type ? *_type : _initializer->get_type());
    }

    const core::semantic _semantic;
    const core::semantic_index_t _semantic_index;

    const core::qualifier_storage _qualifier;

  private:
    //TODO: make this std::optional once available (C++17)
    std::unique_ptr<language::type> _type;//TODO: const (ptr or expression or both)?
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
