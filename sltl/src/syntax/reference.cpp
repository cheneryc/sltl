#include "reference.h"
#include "variable_declaration.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::reference::reference(const declaration& declaration) : _declaration(declaration) {}

bool ns::reference::apply_action(action& act)
{
  return apply_action_impl(act, *this);
}

bool ns::reference::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this);
}

sltl::language::type ns::reference::get_type() const
{
  return _declaration.get_type();
}
