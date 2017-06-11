#include "io_block.h"
#include "io_block_manager.h"
#include "block_manager.h"
#include "action.h"

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;

  std::wstring create_name(sltl::core::qualifier_storage qualifier)
  {
    std::wstringstream ss(L"io", std::ios::in | std::ios::out | std::ios::ate);

    if (auto postfix = sltl::language::to_qualifier_string(qualifier))
    {
      ss << L"_" << postfix;
    }

    return ss.str();
  }
}

ns::io_block::io_block(sltl::detail::pass_key<io_block_manager>, sltl::core::qualifier_storage qualifier) : block_base(::create_name(qualifier)), _qualifier(qualifier)
{
  block_base* b = set_current_block(this);
  assert(b == nullptr);
}

ns::statement& ns::io_block::add_impl(statement::ptr&&)
{
  throw std::exception();//TODO: exception type and message
}

ns::variable_declaration& ns::io_block::add_variable_declaration(std::wstring&&, expression::ptr&&)
{
  throw std::exception();//TODO: exception type and message
}

ns::variable_declaration& ns::io_block::add_variable_declaration(std::wstring&& name, const sltl::language::type& type, sltl::core::semantic_pair semantic)
{
  if(semantic._semantic == core::semantic::none)
  {
    throw std::exception();//TODO: exception type and message
  }

  return add_variable_declaration_impl(statement::make<variable_declaration>(std::move(name), type, core::qualifier::make<core::storage_qualifier>(_qualifier), semantic));
}

void ns::io_block::pop()
{
  block_base* b = set_current_block(nullptr);
  assert(b == this);
}

bool ns::io_block::apply_action(action& act)
{
  return apply_action_impl(act, *this, _statements.begin(), _statements.end());
}

bool ns::io_block::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this, _statements.begin(), _statements.end());
}
