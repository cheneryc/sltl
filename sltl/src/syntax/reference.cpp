#include "reference.h"
#include "action.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::reference::reference(const variable_declaration& declaration) : expression(), _declaration(declaration)
{
}

bool ns::reference::apply_action(action& act)
{
  return act(*this);
}

bool ns::reference::apply_action(const_action& cact) const
{
  return cact(*this);
}
