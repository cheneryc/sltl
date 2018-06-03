#include "block_base.h"
#include "block_manager.h"

#include <algorithm>


namespace
{
  namespace ns = sltl::syntax;
}

ns::block_base::block_base(std::wstring&& name) : _current_child_id(0), _name(std::move(name)) {}
ns::block_base::block_base(const wchar_t* name) : block_base(name ? name : ns::get_current_block().get_child_name()) {}

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
    auto it = _variable_map.find(vd->_name);

    assert(it != _variable_map.end());
    assert(it->second.get_ref() == 0);

    _variable_map.erase(it);
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }

  auto it = std::find_if(_statements.rbegin(), _statements.rend(), [&s](const statement::ptr& s_find)
  {
    return (s_find.get() == &s);
  });

  if(it != _statements.rend())
  {
    // Calls to std::next and base are required to convert
    // the reverse_iterator back into a 'forward' iterator
    _statements.erase(std::next(it).base());
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }
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
