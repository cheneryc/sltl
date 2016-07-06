#include "block.h"
#include "block_manager.h"
#include "action.h"

#include <algorithm>
#include <sstream>

#include <cassert>


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

ns::block::block(type t) : _t(t), _current_child_id(0), _name(::create_name(_t == local))
{
  block_manager::get()._block_stack.emplace(*this);
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

//TODO: make this private and add temporary::temporary as a friend?
void ns::block::erase(const statement& s)
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

void ns::block::pop()
{
  auto& block_stack = block_manager::get()._block_stack;

  assert(block_stack.size() > 0);
  assert(block_stack.top().get() == *this);

  block_stack.pop();
}

std::wstring ns::block::get_child_name()
{
  std::wstringstream ss;

  if(_t == local)
  {
    ss << _name << L"_";
  }

  ss << std::to_wstring(++_current_child_id);
  return ss.str();
}

bool ns::block::apply_action(action& act)
{
  return apply_action_impl(act, *this, _statements.begin(), _statements.end());
}

bool ns::block::apply_action(const_action& cact) const
{
  return apply_action_impl(cact, *this, _statements.begin(), _statements.end());
}
