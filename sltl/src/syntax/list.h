#pragma once

#include "../output.h"

#include <deque>
#include <algorithm>


namespace sltl
{
namespace syntax
{
  class list_separator {};

  template<typename T, typename N>
  class list : public N
  {
  public:
    virtual void traverse(output& out) const
    {
      auto it = _list_items.begin();
      const auto it_end = _list_items.end();

      if(it != it_end)
      {
        (*it)->traverse(out);

        std::for_each(++it, it_end, [this, &out](const typename T::ptr& item)
        {
          out(list_separator());
          item->traverse(out);
        });
      }
    }

    void add(typename T::ptr&& item)
    {
      _list_items.push_back(std::move(item));
    }

  protected:
    list() : N(), _list_items() {}
    list(list&& l) : N(), _list_items(std::move(l._list_items)) {}

    std::deque<typename T::ptr> _list_items;
  };
}
}
