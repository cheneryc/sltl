#pragma once

#include "element.h"
#include "language.h"


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class variable_declaration;
  }

  class variable : public element
  {
  protected:
    variable(language::type_id id);

    //TODO: check for duplicate names being declared in the same scope?
    //void set_name(const std::wstring& name);

    //TODO: change this to a reference rather than a pointer?
    syntax::variable_declaration* _vd;
  };
}
