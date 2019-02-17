#include "operator.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::operator_unary::operator_unary(sltl::language::operator_unary_id id, expression::ptr&& operand) : super_t(id), _operand(std::move(operand))
{
}

ns::operator_binary::operator_binary(sltl::language::operator_binary_id id, expression::ptr&& lhs, expression::ptr&& rhs) : super_t(id),
  _operand_lhs(std::move(lhs)),
  _operand_rhs(std::move(rhs))
{
}
