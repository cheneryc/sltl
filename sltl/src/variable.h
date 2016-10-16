#pragma once

#include "element.h"

#include "syntax/expression.h"

#include "core/semantic.h"
#include "core/qualifier.h"


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
    class type;
  }

  class variable : public element
  {
  public:
    variable(const variable&) = delete;
    variable& operator=(const variable&) = delete;

  protected:
    variable(syntax::expression::ptr&& initializer);
    variable(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic);

    syntax::expression::ptr make_temporary();

    const syntax::variable_declaration* get_declaration() const
    {
      return _declaration;
    }

  private:
    syntax::variable_declaration* _declaration;
  };
}
