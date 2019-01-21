#pragma once

#include "action.h"
#include "expression.h"
#include "parentheses.h"

#include <type.h>


namespace sltl
{
namespace syntax
{
  class operator_base : public expression
  {
  protected:
    operator_base() = default;
  };

  template<typename E>
  class operator_base_id : public operator_base
  {
    static_assert(std::is_enum<E>::value, "sltl::syntax::operator_base_id: template parameter E must be an enumeration type");

  public:
    const E _operator_id;

  protected:
    operator_base_id(E operator_id) : _operator_id(operator_id) {}
  };

  class operator_unary : public operator_base_id<language::operator_unary_id>
  {
    typedef operator_base_id<language::operator_unary_id> super_t;

  public:
    operator_unary(language::operator_unary_id id, expression::ptr&& operand);

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _operand.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _operand.get());
    }

    void reset(expression::ptr&& operand)
    {
      _operand = std::move(operand);
    }

    virtual language::type get_type() const override
    {
      return _operand->get_type();
    }

    expression::ptr _operand;
  };

  class operator_binary : public operator_base_id<language::operator_binary_id>
  {
    typedef operator_base_id<language::operator_binary_id> super_t;

  public:
    operator_binary(language::operator_binary_id id, expression::ptr&& lhs, expression::ptr&& rhs);

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    void swap_operands()
    {
      _operand_lhs.swap(_operand_rhs);
    }

    virtual language::type get_type() const override
    {
      const language::type type_lhs = _operand_lhs->get_type();
      const language::type type_rhs = _operand_rhs->get_type();

      assert(type_lhs.get_id() == type_rhs.get_id());
      assert(type_lhs.get_dimensions() == type_rhs.get_dimensions() || is_operator_asymmetric(_operator_id));

      //TODO: re-instate somehow as this currently incorrectly fires during the type dimension swapping action
      //assert(type_lhs.get_dimensions().n() == type_rhs.get_dimensions().m());

      language::type_id id = language::id_unknown;

      switch(_operator_id)
      {
        case language::id_lt:
        case language::id_lt_eq:
        case language::id_gt:
        case language::id_gt_eq:
          return language::type_helper<bool>();
        case language::id_scalar_vector_multiplication:
        case language::id_scalar_vector_division:
        case language::id_scalar_matrix_multiplication:
        case language::id_scalar_matrix_division:
          return type_rhs;
        case language::id_vector_scalar_multiplication:
        case language::id_vector_scalar_division:
        case language::id_matrix_scalar_multiplication:
        case language::id_matrix_scalar_division:
          return type_lhs;
        case language::id_element_wise_eq:
        case language::id_element_wise_ne:
        case language::id_element_wise_lt:
        case language::id_element_wise_lt_eq:
        case language::id_element_wise_gt:
        case language::id_element_wise_gt_eq:
          id = language::id_bool;
        default:
          // This determines the return type for vector-matrix and matrix-matrix multiplication but should
          // work fine for all other currently supported operators, as their operand types should be equal
          return language::type(id != language::id_unknown ? id : type_lhs.get_id(), type_lhs.get_dimensions().m(), type_rhs.get_dimensions().n());
      }
    }

    expression::ptr _operand_lhs;
    expression::ptr _operand_rhs;

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
