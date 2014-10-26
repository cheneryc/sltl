#pragma once

#include "block.h"
#include "block_manager.h"
#include "expression.h"
#include "declaration.h"

#include "../output.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  class variable_declaration : public declaration
  {
  public:
    variable_declaration(language::type_id id) : declaration(get_current_block().get_child_name()), _id(id), _initializer() {}
    variable_declaration(language::type_id id, expression::ptr&& initializer) : declaration(get_current_block().get_child_name()), _id(id), _initializer(std::move(initializer)) {}

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

    virtual void traverse(output& out) const
    {
      out(*this);

      if(_initializer)
      {
        _initializer->traverse(out);
      }

      out(*this, false);
    }

    const language::type_id _id;

  private:
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
