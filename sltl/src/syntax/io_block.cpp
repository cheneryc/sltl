#include "io_block.h"
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

ns::io_block::io_block(sltl::core::qualifier_storage qualifier) : block_base(::create_name(qualifier)), _qualifier(qualifier)
{
  block_base* b = set_current_block(this);
  assert(b == nullptr);
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
