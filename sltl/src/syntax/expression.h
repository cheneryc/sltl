#pragma once

#include "node.h"
#include "list.h"

#include <type.h>

#include <memory>
#include <utility>


namespace sltl
{
namespace syntax
{
  class expression : public node
  {
  public:
    typedef std::unique_ptr<expression> ptr;

    template<typename T, typename ...A>
    static ptr make(A&&... a)
    {
      return ptr(new T(std::forward<A>(a)...));
    }

    // Returns a copy, rather than reference, as the type may need to be calculated on demand (e.g. operator_binary)
    virtual language::type get_type() const = 0;

  protected:
    expression() = default;
  };

  class expression_list : public list<expression, node>
  {
    typedef list<expression, node> super_t;

  public:
    expression_list() : super_t() {}
    expression_list(expression_list&& list) : super_t(std::move(list)) {}

    expression_list(expression::ptr&& e, expression_list&& list) : super_t(std::move(list))
    {
      _list_items.push_front(std::move(e));
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _list_items.begin(), _list_items.end());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _list_items.begin(), _list_items.end());
    }
  };
}
}
