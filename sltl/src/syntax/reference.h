#pragma once

#include "expression.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class variable_declaration;

  class reference : public expression
  {
  public:
    reference(const variable_declaration& declaration);

    virtual void traverse(output& out) const;

    const variable_declaration& _declaration;
  };
}
}
