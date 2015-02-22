#pragma once

#include "node.h"
#include "list.h"

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

  protected:
    expression() = default;
  };

  class expression_list : public list<expression, expression>
  {
    typedef list<expression, expression> super_t;

  public:
    expression_list() : super_t() {}
    expression_list(expression_list&& list) : super_t(std::move(list)) {}

    expression_list(expression::ptr&& e, expression_list&& list) : super_t(std::move(list))
    {
      _list_items.push_front(std::move(e));
    }
  };
}
}
