#include "parameter_declaration.h"

#include <sstream>


namespace
{
  namespace ns = sltl::syntax;
}

ns::parameter_declaration::parameter_declaration(std::wstring&& name, const sltl::language::type& type, sltl::core::qualifier_param qualifier) : declaration(std::move(name)), _qualifier(qualifier), _type(type)
{
}

bool ns::parameter_declaration::apply_action(action& act)
{
  return apply_action_impl(act, *this);
}

bool ns::parameter_declaration::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this);
}

sltl::language::type ns::parameter_declaration::get_type() const
{
  return _type;
}
