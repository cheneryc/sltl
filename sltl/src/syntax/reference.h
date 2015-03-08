#pragma once

#include "expression.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;
  class variable_declaration;

  class reference : public expression
  {
  public:
    reference(const variable_declaration& declaration);

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

    const variable_declaration& _declaration;
  };
}
}
