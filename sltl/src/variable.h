#pragma once

#include "element.h"

#include "syntax/expression.h"

#include "core/semantic.h"


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class declaration;
    class variable_declaration;
    class parameter_declaration;
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
    explicit variable(syntax::variable_declaration* vd);
    explicit variable(syntax::parameter_declaration* pd);

    syntax::expression::ptr make_reference() const;
    syntax::expression::ptr make_reference_or_temporary();
    syntax::expression::ptr make_temporary();

  private:
    syntax::declaration* _declaration;
  };

  syntax::variable_declaration* create_variable_declaration(syntax::expression::ptr&& initializer);
  syntax::variable_declaration* create_variable_declaration(const language::type& type, core::semantic_pair semantic);
}
