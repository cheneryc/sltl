#include "expression_statement.h"
#include "action.h"

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
  return apply_action(act, *this);
}

bool ns::expression_statement::apply_action(const_action& cact) const
{
  assert(_expression);
  return apply_action(cact, *this);
}
