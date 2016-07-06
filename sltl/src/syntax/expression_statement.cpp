#include "expression_statement.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

ns::expression_statement::expression_statement(expression::ptr&& e) : _expression(std::move(e))
{
}

bool ns::expression_statement::apply_action(action& act)
{
  assert(_expression);
  return apply_action_impl(act, *this, _expression.get());
}

bool ns::expression_statement::apply_action(const_action& cact) const
{
  assert(_expression);
  return apply_action_impl(cact, *this, _expression.get());
}
