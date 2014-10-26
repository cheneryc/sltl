#include "expression_statement.h"
#include "../output.h"

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;
}

ns::expression_statement::expression_statement(expression::ptr&& e) : _expression(std::move(e))
{
}

void ns::expression_statement::traverse(sltl::output& out) const
{
  assert(_expression);

  out(*this);
  _expression->traverse(out);
  out(*this, false);
}
