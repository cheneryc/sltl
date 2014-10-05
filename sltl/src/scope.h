#pragma once

#include "element.h"


namespace sltl
{
  class scope : public element
  {
  public:
    enum type
    {
      block,
      loop,
      function,
      shader
    };

    scope(type t);
    ~scope();

  private:
    type _t;
  };
}
