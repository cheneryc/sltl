#pragma once

#include "node.h"

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
}
}
