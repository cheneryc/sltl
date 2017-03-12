#include "block.h"
#include "block_manager.h"
#include "action.h"

#include <sstream>


namespace
{
  namespace ns = sltl::syntax;

  //TODO: it is possible to come up with a much better auto-naming scheme, ideas including:
    //1. convert the id into letters so instead of 1,2,3 etc we get a, b, aa, etc. (MAYBE: scope id's are letters and variables are numbers and prefix?)
    //2. or, take the type into account so that a vector could be v1, v2 etc a float f1, f2
    //3. not all scopes need to append their names to ensure uniqueness e.g.shader and function scopes can avoid appending the scopes name and still be unqiue
  //TODO: move the name generation logic into a separate namespace/file (like language), also need a special prefix (not s) for scopes
  std::wstring create_name(bool is_local)
  {
    return (is_local ? ns::get_current_block().get_child_name() : L"root");
  }
}

ns::block::block(type t) : block_base(::create_name(t == local)), _t(t)
{
  block_manager::get().push({}, *this);
}

bool ns::block::operator==(const block& rhs) const
{
  // This is a suitable equality operator as block objects are
  // unique (no two block objects should be considered equal).
  return (this == &rhs);
}

bool ns::block::operator!=(const block& rhs) const
{
  return !(this->operator==(rhs));
}

void ns::block::pop()
{
  block_manager::get().pop({}, *this);
}

std::wstring ns::block::get_child_name()
{
  std::wstringstream ss;

  if(_t == local)
  {
    ss << get_name() << L"_";
  }

  ss << block_base::get_child_name();
  return ss.str();
}

ns::variable_info& ns::block::variable_info_find(const std::wstring& name)
{
  return variable_info_find(name, block_manager::get().get_block_stack());
}

ns::variable_info& ns::block::variable_info_find(const std::wstring& name, std::stack<std::reference_wrapper<block>> block_stack)
{
  auto it = _variable_map.find(name);

  if(it != _variable_map.end())
  {
    return it->second;
  }
  else
  {
    if(block_stack.empty() || (block_stack.top().get() != *this))
    {
      throw std::exception();//TODO: exception type and message
    }

    block_stack.pop();

    if(block_stack.empty())
    {
      throw std::exception();//TODO: exception type and message
    }

    auto& parent_block = block_stack.top().get();
    auto& parent_vi = parent_block.variable_info_find(name, std::move(block_stack));

    return parent_vi;
  }
}

bool ns::block::apply_action(action& act)
{
  return apply_action_impl(act, *this, _statements.begin(), _statements.end());
}

bool ns::block::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this, _statements.begin(), _statements.end());
}
