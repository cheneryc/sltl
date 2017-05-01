#include "operator.h"


namespace
{
  namespace ns = sltl::syntax;

  ns::expression::ptr add_parentheses(ns::expression::ptr&& e)
  {
    if(dynamic_cast<ns::operator_binary*>(e.get()))
    {
      return ns::expression::make<ns::parentheses>(std::move(e));
    }
    else
    {
      return std::move(e);
    }
  }
}

ns::operator_unary::operator_unary(sltl::language::operator_unary_id id, expression::ptr&& operand) : super_t(id), _operand(::add_parentheses(std::move(operand)))
{
}

ns::operator_binary::operator_binary(sltl::language::operator_binary_id id, expression::ptr&& lhs, expression::ptr&& rhs) : super_t(id),
  _operand_lhs(::add_parentheses(std::move(lhs))),
  _operand_rhs(::add_parentheses(std::move(rhs)))
{
}
