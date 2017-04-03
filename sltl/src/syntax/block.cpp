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

ns::variable_declaration& ns::block::add_variable_declaration(expression::ptr&& initializer)
{
  return add_variable_declaration_impl(statement::make<variable_declaration>(std::move(initializer)));
}

ns::variable_declaration& ns::block::add_variable_declaration(const sltl::language::type& type, sltl::core::semantic_pair semantic)
{
  if(semantic._semantic != core::semantic::none)
  {
    throw std::exception();//TODO: exception type and message
  }

  return add_variable_declaration_impl(statement::make<variable_declaration>(type, core::qualifier::make<core::storage_qualifier>(core::qualifier_storage::default), semantic));
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

ns::variable_info* ns::block::variable_info_find(const std::wstring& name)
{
  return variable_info_find(name, block_manager::get().get_block_stack());
}

ns::variable_info* ns::block::variable_info_find(const std::wstring& name, std::stack<std::reference_wrapper<block>> block_stack)
{
  if(block_stack.empty() || (block_stack.top().get() != *this))
  {
    throw std::exception();//TODO: exception type and message
  }

  variable_info* vi = block_base::variable_info_find(name);

  if(!vi)
  {
    block_stack.pop();

    if(!block_stack.empty())
    {
      auto& parent_block = block_stack.top().get();
      vi  = parent_block.variable_info_find(name, std::move(block_stack));
    }
  }

  return vi;
}

bool ns::block::apply_action(action& act)
{
  return apply_action_impl(act, *this, _statements.begin(), _statements.end());
}

bool ns::block::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this, _statements.begin(), _statements.end());
}
