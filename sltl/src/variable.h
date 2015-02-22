#pragma once

#include "element.h"

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
    variable(const language::type& type);
    variable(const language::type& type, syntax::expression::ptr&& initializer);

    syntax::expression::ptr make_temporary();

    const syntax::variable_declaration* get_declaration() const
    {
      return _declaration;
    }

  private:
    syntax::variable_declaration* _declaration;
  };
}
