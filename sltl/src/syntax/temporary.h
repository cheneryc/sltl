#pragma once

#include "expression.h"
#include "variable_declaration.h"


namespace sltl
{
namespace syntax
{
  class temporary : public expression
  {
  public:
    temporary(variable_declaration&& declaration) : expression(), _type(declaration._type), _initializer(declaration.move())
    {
      syntax::get_current_block().erase(declaration);
    }

    expression::ptr&& move()
    {
      return std::move(_initializer);
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _initializer.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _initializer.get());
    }

    const language::type _type;

  private:
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
