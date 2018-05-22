#pragma once

#include "action.h"

#include <deque>


namespace sltl
{
namespace syntax
{
  template<typename T, typename N>
  class list : public N
  {
    static_assert(std::is_base_of<node, T>::value, "sltl::syntax::list: template parameter T must derive from sltl::syntax::node");
    static_assert(std::is_base_of<node, N>::value, "sltl::syntax::list: template parameter N must derive from sltl::syntax::node");

  public:
    void add(typename T::ptr&& item)
    {
      _list_items.push_back(std::move(item));
    }

    typename std::deque<typename T::ptr>::const_iterator begin() const
    {
      return _list_items.begin();
    }

    typename std::deque<typename T::ptr>::const_iterator end() const
    {
      return _list_items.end();
    }

    size_t size() const
    {
      return _list_items.size();
    }

  protected:
    list() : N(), _list_items() {}
    list(list&& l) : N(), _list_items(std::move(l._list_items)) {}

    std::deque<typename T::ptr> _list_items;
  };
}
}
