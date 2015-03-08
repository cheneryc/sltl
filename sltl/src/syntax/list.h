#pragma once

#include "action.h"

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
    void add(typename T::ptr&& item)
    {
      _list_items.push_back(std::move(item));
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

  protected:
    list() : N(), _list_items() {}
    list(list&& l) : N(), _list_items(std::move(l._list_items)) {}

    std::deque<typename T::ptr> _list_items;

  private:
    template<typename A, typename TT>
    static auto apply_action(A& act, TT& type) -> typename std::enable_if<std::is_same<typename std::remove_const<TT>::type, list>::value, bool>::type
    {
      bool is_continuing = true;

      auto it = type._list_items.begin();
      const auto it_end = type._list_items.end();

      if(it != it_end)
      {
        is_continuing = (*it)->apply_action(act);

        while(is_continuing && (++it != it_end))
        {
          list_separator ls;
          is_continuing = act(ls);

          if(is_continuing)
          {
            is_continuing = (*it)->apply_action(act);
          }
        }
      }

      return is_continuing;
    }
  };
}
}
