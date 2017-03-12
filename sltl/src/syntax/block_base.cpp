#include "block_base.h"

#include <algorithm>


namespace
{
  namespace ns = sltl::syntax;
}

ns::block_base::block_base(std::wstring&& name) : _current_child_id(0), _name(std::move(name))
{
}

std::wstring ns::block_base::get_child_name()
{
  return std::to_wstring(++_current_child_id);
}

//TODO: make this private and add temporary::temporary as a friend?
void ns::block_base::erase(const statement& s)
{
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

void ns::block_base::variable_info_add(const std::wstring& name)
{
  auto result = _variable_map.emplace(name, variable_info());

  if(!(result.second))
  {
    throw std::exception();//TODO: exception type and message
  }
}

ns::variable_info& ns::block_base::variable_info_find(const std::wstring& name)
{
  return _variable_map.at(name);
}
