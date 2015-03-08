#pragma once

#include "action.h"
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
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const language::type _type;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, temporary>::value, bool>::type
    {
      return (act(type) && (type._initializer ? type._initializer->apply_action(act) : true) && act(type, false));
    }

    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
