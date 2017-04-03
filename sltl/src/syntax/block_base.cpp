#include "block_base.h"

#include <algorithm>


namespace
{
  namespace ns = sltl::syntax;
}

ns::block_base::block_base(std::wstring&& name) : _current_child_id(0), _name(std::move(name))
{
}

ns::statement& ns::block_base::add_impl(statement::ptr&& s)
{
  _statements.emplace_back(std::move(s));
  return *_statements.back();
}

ns::variable_declaration& ns::block_base::add_variable_declaration_impl(statement::ptr&& s)
{
  _statements.emplace_back(std::move(s));

  auto& vd_statement = _statements.back();
  auto& vd = static_cast<variable_declaration&>(*vd_statement);

  auto result = _variable_map.emplace(vd._name, variable_info());

  if(!(result.second))
  {
    throw std::exception();//TODO: exception type and message
  }

  return vd;
}

void ns::block_base::erase(const statement& s)
{
  // Remove the associated variable_info data if the erased statement is a variable_declaration
  if(auto vd = dynamic_cast<const variable_declaration*>(&s))
  {
    _variable_map.erase(vd->_name);
  }

  auto it = std::find_if(_statements.rbegin(), _statements.rend(), [&s](const statement::ptr& s_find)
  {
    return (s_find.get() == &s);
  });

  // The statement to be removed should be the most recently created (this
  // function is only meant to be used by the syntax::temporary constructor)
  assert(it == _statements.rbegin());
  assert(it != _statements.rend());

  // The call to std::next and base are required to convert the reverse_iterator back to a normal iterator
  _statements.erase(std::next(it).base());
}

ns::variable_info* ns::block_base::variable_info_find(const std::wstring& name)
{
  auto it = _variable_map.find(name);

  if(it != _variable_map.end())
  {
    return &(it->second);
  }
  else
  {
    return nullptr;
  }
}

const std::wstring& ns::block_base::get_name() const
{
  return _name;
}

std::wstring ns::block_base::get_child_name()
{
  return std::to_wstring(++_current_child_id);
}

bool ns::block_base::operator==(const block_base& rhs) const
{
  // This is a suitable equality operator as block objects are
  // unique (no two block objects should be considered equal).
  return (this == &rhs);
}

bool ns::block_base::operator!=(const block_base& rhs) const
{
  return !(this->operator==(rhs));
}
