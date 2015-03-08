#pragma once

#include "action.h"
#include "expression.h"
#include "parentheses.h"

#include "../language.h"


namespace sltl
{
namespace syntax
{
  //TODO: maybe have a less generic class e.g. binary_operator, unary_operator etc?
  class operator_base : public expression
  {
  protected:
    operator_base(expression::ptr&& lhs, expression::ptr&& rhs) : expression(),
      _lhs(add_parentheses(std::move(lhs))),
      _rhs(add_parentheses(std::move(rhs))) {}

    const expression::ptr _lhs;
    const expression::ptr _rhs;

  private:
    expression::ptr add_parentheses(expression::ptr&& e)
    {
      if(dynamic_cast<operator_base*>(e.get()))
      {
        return expression::make<parentheses>(std::move(e));
      }
      else
      {
        return std::move(e);
      }
    }
  };

  class binary_operator : public operator_base
  {
  public:
    binary_operator(language::operator_id id, expression::ptr&& lhs, expression::ptr&& rhs) : operator_base(std::move(lhs), std::move(rhs)), _id(id) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const language::operator_id _id;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, binary_operator>::value, bool>::type
    {
      return (type._lhs->apply_action(act) && act(type) && type._rhs->apply_action(act));
    }
  };

  //TODO: can combine the binary and assignment operator classes into one?
  class assignment_operator : public operator_base
  {
  public:
    assignment_operator(language::assignment_operator_id id, expression::ptr&& lhs, expression::ptr&& rhs) : operator_base(std::move(lhs), std::move(rhs)), _id(id) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const language::assignment_operator_id _id;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, assignment_operator>::value, bool>::type
    {
      return (type._lhs->apply_action(act) && act(type) && type._rhs->apply_action(act));
    }
  };
}
}
