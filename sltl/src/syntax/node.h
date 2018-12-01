#pragma once

#include "action.h"

#include <vector>
#include <algorithm>

#include <cassert>


namespace sltl
{
namespace syntax
{
  class node
  {
  public:
    // Make sure all derived types are non-copyable
    node(const node&) = delete;
    node& operator=(const node&) = delete;

    virtual ~node() = default;

    virtual bool apply_action(action& act) = 0;
    virtual bool apply_action(const_action& cact) const = 0;

  protected:
    node() = default;
  };

  template<typename A, typename T>
  bool apply_action_impl(A& act, T& type)
  {
    const auto act_result = act(type);

    assert(act_result == action_return_t::step_out ||
           act_result == action_return_t::stop);

    return act_result != action_return_t::stop;
  }

  template<typename A, typename T, typename N>
  bool apply_action_impl(A& act, T& type, N* child)
  {
    std::vector<N*> children;

    if(child)
    {
      children.push_back(child);
    }

    return apply_action_impl(act, type, children.begin(), children.end());
  }

  template<typename A, typename T, typename I>
  bool apply_action_impl(A& act, T& type, I it_begin, I it_end)
  {
    const auto act_result = act(type);

    assert(act_result == action_return_t::step_in ||
           act_result == action_return_t::step_over ||
           act_result == action_return_t::stop);

    bool is_continuing = act_result != action_return_t::stop;

    if(act_result == action_return_t::step_in)
    {
      is_continuing = std::all_of(it_begin, it_end, [&act](typename std::iterator_traits<I>::reference child){ return child->apply_action(act); });
    }

    auto fn = [&act](T& t)
    {
      return act(t, false);
    };

    return is_continuing && apply_action_impl(fn, type);
  }
}
}
