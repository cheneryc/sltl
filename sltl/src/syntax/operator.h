#pragma once

#include "action.h"
#include "expression.h"
#include "parentheses.h"

#include "../language.h"


namespace sltl
{
namespace syntax
{
  class operator_base : public expression
  {
  protected:
    static expression::ptr add_parentheses(expression::ptr&& e)
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

  template<typename E>
  class operator_base_id : public operator_base
  {
    static_assert(std::is_enum<E>::value, "sltl::syntax::operator_base_id: template parameter E must be an enumeration type.");

  public:
    const E _operator_id;

  protected:
    operator_base_id(E operator_id) : _operator_id(operator_id) {}
  };

  class operator_unary : public operator_base_id<language::operator_unary_id>
  {
    typedef operator_base_id<language::operator_unary_id> super_t;

  public:
    operator_unary(language::operator_unary_id id, expression::ptr&& operand) : super_t(id), _operand(add_parentheses(std::move(operand))) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _operand.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _operand.get());
    }

    const expression::ptr _operand;
  };

  class operator_binary : public operator_base_id<language::operator_binary_id>
  {
    typedef operator_base_id<language::operator_binary_id> super_t;

  public:
    operator_binary(language::operator_binary_id id, expression::ptr&& lhs, expression::ptr&& rhs) : super_t(id),
      _operand_lhs(add_parentheses(std::move(lhs))),
      _operand_rhs(add_parentheses(std::move(rhs))) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    const expression::ptr _operand_lhs;
    const expression::ptr _operand_rhs;

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, operator_binary>::value, bool>::type
    {
      expression* op_lhs = type._operand_lhs.get();
      expression* op_rhs = type._operand_rhs.get();
      expression* ops[] = { op_lhs, op_rhs };

      assert(op_lhs);
      assert(op_rhs);

      return apply_action_impl(act, type, std::begin(ops), std::end(ops));
    }
  };
}
}
