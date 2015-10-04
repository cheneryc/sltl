#pragma once

#include "element.h"

#include "core/qualifier.h"

#include "syntax/expression.h"


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class variable_declaration;
  }

  // Forward declarations - sltl::language namespace
  namespace language
  {
    struct type;
  }

  class variable : public element
  {
  public:
    variable(const variable&) = delete;
    variable& operator=(const variable&) = delete;

  protected:
    variable(const language::type& type, core::qualifier::ptr&& qualifier);
    variable(const language::type& type, core::qualifier::ptr&& qualifier, syntax::expression::ptr&& initializer);

    syntax::expression::ptr make_temporary();

    const syntax::variable_declaration* get_declaration() const
    {
      return _declaration;
    }

  private:
    syntax::variable_declaration* _declaration;
  };
}
