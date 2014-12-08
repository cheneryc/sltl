#pragma once

#include "element.h"
#include "language.h"
#include "syntax/expression.h"


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class variable_declaration;
  }

  class variable : public element
  {
  public:
    variable(const variable&) = delete;
    variable& operator=(const variable&) = delete;

  protected:
    variable(const language::type& type);
    variable(const language::type& type, syntax::expression::ptr&& initializer);

    //TODO: check for duplicate names being declared in the same scope?
    //void set_name(const std::wstring& name);

    // This isn't const so the variable declaration's initializer
    // can potentially be moved into a syntax::temporary object
    syntax::variable_declaration& _declaration;
  };
}
