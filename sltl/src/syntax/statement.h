#pragma once

#include "node.h"

#include <memory>
#include <utility>


namespace sltl
{
namespace syntax
{
  class statement : public node
  {
  public:
    typedef std::unique_ptr<statement> ptr;

    template<typename T, typename ...A>
    static ptr make(A&&... a)
    {
      return ptr(new T(std::forward<A>(a)...));
    }

  protected:
    statement() = default;
  };
}
}
