#pragma once

#include "expression.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;
  class declaration;

  class reference : public expression
  {
  public:
    reference(const declaration& declaration);

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

    virtual language::type get_type() const override;

    //TODO: store the name of the variable and look it up on demand from a variable_map stored in each block
    const declaration& _declaration;
  };
}
}
