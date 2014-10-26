#include "reference.h"
#include "../output.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::reference::reference(const variable_declaration& declaration) : expression(), _declaration(declaration)
{
}

void ns::reference::traverse(sltl::output& out) const
{
  out(*this);
}
