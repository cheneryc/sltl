#pragma once

#include "block.h"
#include "block_manager.h"
#include "expression.h"
#include "declaration_statement.h"

#include "../language.h"


namespace sltl
{
namespace syntax
{
  class variable_declaration : public declaration_statement
  {
  public:
    variable_declaration(const language::type& type) : declaration_statement(get_current_block().get_child_name()), _type(type), _initializer(), _ref_count(0) {}
    variable_declaration(const language::type& type, expression::ptr&& initializer) : declaration_statement(get_current_block().get_child_name()), _type(type), _initializer(std::move(initializer)), _ref_count(0) {}

    bool is_direct_initialized() const
    {
      //TODO: this should be based on the type? For scalar types return false, otherwise true?
      return true;//true - Constructor initializer syntax, false - assignment operator syntax
    }

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
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const language::type _type;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, variable_declaration>::value, bool>::type
    {
      return (act(type) && (type._initializer ? type._initializer->apply_action(act) : true) && act(type, false));
    }

    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
    mutable size_t _ref_count;//TODO: this is not a great design, try and improve it...
  };
}
}
