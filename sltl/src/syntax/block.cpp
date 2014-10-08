#include "block.h"
#include "block_manager.h"
#include "../output.h"

#include <algorithm>

#include <cassert>


namespace
{
  namespace ns = sltl::syntax;

  //TODO: it is possible to come up with a much better auto-naming scheme, ideas including:
    //1. convert the id into letters so instead of 1,2,3 etc we get a, b, aa, etc. (MAYBE: scope id's are letters and variables are numbers and prefix?)
    //2. or, take the type into account so that a vector could be v1, v2 etc a float f1, f2
    //3. not all scopes need to append their names to ensure uniqueness e.g.shader and function scopes can avoid appending the scopes name and still be unqiue
  //TODO: move the name generation logic into a separate namespace/file (like language), also need a special prefix (not s) for scopes
  std::wstring create_name(bool is_stack_empty)
  {
    if(is_stack_empty)
    {
      return L"root";
    }
    else
    {
      return ns::get_current_block().get_child_name();
    }
  }
}

ns::block::block(type t) : _t(t), _current_child_id(0), _name(::create_name(block_manager::get()._block_stack.empty()))
{
  block_manager::get()._block_stack.emplace(*this);
}

ns::block::block(block&& b) : _t(b._t), _current_child_id(0), _name(::create_name(block_manager::get()._block_stack.empty())), _children(std::move(b._children))
{
  block_manager::get()._block_stack.emplace(*this);
}

bool ns::block::operator==(const block& rhs) const
{
  // This is a suitable equality operator as scope objects are
  // unique (no two scope objects should be considered equal).
  return (this == &rhs);
}

bool ns::block::operator!=(const block& rhs) const
{
  return !(this->operator==(rhs));
}

void ns::block::add(node::ptr&& node)
{
  _children.push_back(std::move(node));
}

void ns::block::pop()
{
  auto& block_stack = block_manager::get()._block_stack;

  assert(block_stack.size() > 0);
  assert(block_stack.top().get() == *this);

  block_stack.pop();
}

ns::node::ptr ns::block::remove(node* node)
{
  auto it = std::find_if(_children.rbegin(), _children.rend(), [node](const node::ptr& n)
  {
    return (n.get() == node);
  });

  //TODO: fix this - check if assert is valid or not
  //assert(it == _children.rbegin()); // This is not correct for the scalar(scalar&&) constructor as it has just created a new var - itself!
  assert(it != _children.rend());

  node::ptr n = std::move(*it);
  // The call to std::next and base are required to convert the reverse_iterator back to a normal iterator
  _children.erase(std::next(it).base());
  return std::move(n);
}

std::wstring ns::block::get_child_name()
{
  std::wstringstream ss;

  //TODO: benchmark to see if stringstream is necessary here (or just + strings together)
  if(_t == local)
  {
    ss << _name << L"_";
  }

  ss << std::to_wstring(++_current_child_id);
  return ss.str();
}

void ns::block::traverse(sltl::output& out) const
{
  out(*this);

  std::for_each(_children.begin(), _children.end(), [&out](const node::ptr& node)
  {
    // TODO: consider how to fix this hack, all this logic should be in the output object
    const bool is_block = (dynamic_cast<block*>(node.get()) != nullptr);

    if(!is_block)
    {
      out.line_begin();
    }

    node->traverse(out);
    
    if(!is_block)
    {
      out.line_end();
    }
  });

  out(*this, false);
}
