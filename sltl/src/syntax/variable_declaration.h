#pragma once

#include "block.h"
#include "block_manager.h"
#include "expression.h"
#include "declaration_statement.h"

#include "../language.h"

#include "../core/semantic.h"
#include "../core/qualifier.h"


namespace sltl
{
namespace syntax
{
  class variable_declaration : public declaration_statement
  {
  public:
    variable_declaration(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic) : declaration_statement(get_current_block().get_child_name()),
      _type(type),
      _semantic(semantic._semantic),
      _semantic_index(semantic._index),
      _qualifier(std::move(qualifier)),
      _initializer(),
      _ref_count(0) {}

    variable_declaration(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic, expression::ptr&& initializer) : declaration_statement(get_current_block().get_child_name()),
      _type(type),
      _semantic(semantic._semantic),
      _semantic_index(semantic._index),
      _qualifier(std::move(qualifier)),
      _initializer(std::move(initializer)),
      _ref_count(0) {}

    bool has_initializer() const
    {
      return static_cast<bool>(_initializer);
    }

    expression::ptr&& move()
    {
      return std::move(_initializer);
    }

    size_t get_ref_count() const
    {
      return _ref_count;
    }

    const variable_declaration& inc_ref_count() const
    {
      ++_ref_count; return *this;
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _initializer.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _initializer.get());
    }

    const core::qualifier& get_qualifier() const
    {
      return *_qualifier;
    }

    const language::type _type;

    const core::semantic _semantic;
    const core::semantic_index_t _semantic_index;

  private:
    core::qualifier::ptr _qualifier;//TODO: const (ptr or expression or both)?
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
    mutable size_t _ref_count;//TODO: this is not a great design, try and improve it...
  };
}
}
