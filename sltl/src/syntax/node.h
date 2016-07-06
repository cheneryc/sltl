#pragma once

#include "../detail/conditional_traits.h"

#include <algorithm>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class node
  {
  public:
    // Make sure all derived types are non-copyable
    node(const node&) = delete;
    node& operator=(const node&) = delete;

    virtual ~node()
    {
    }

    virtual bool apply_action(action& act) = 0;
    virtual bool apply_action(const_action& cact) const = 0;

  protected:
    node() = default;
  };

  template<typename A, typename T>
  bool apply_action_impl(A& act, T& type)
  {
    return (act(type) != action_return_t::stop);
  }

  template<typename A, typename T>
  bool apply_action_impl(A& act, T& type, typename detail::same_const_pointer<T, node*>::type child)
  {
    const auto return_act = act(type);
    bool return_val = return_act != action_return_t::stop;

    if(return_act == action_return_t::step_in && child)
    {
      return_val = child->apply_action(act);
    }

    return (return_val && (act(type, false) != action_return_t::stop));
  }

  template<typename A, typename T, typename I>
  bool apply_action_impl(A& act, T& type, I it_begin, I it_end)
  {
    const auto return_act = act(type);
    bool return_val = return_act != action_return_t::stop;

    if(return_act == action_return_t::step_in)
    {
      return_val = std::all_of(it_begin, it_end, [&act](typename detail::same_const_reference<T, statement::ptr&>::type n){ return n->apply_action(act); });
    }

    return (return_val && (act(type, false) != action_return_t::stop));
  }
}
}
