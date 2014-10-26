#pragma once

#include "element.h"


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class block;
  }

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

    template<typename S>
    scope(type t, S& s) : _t(t), _b(s.add<syntax::block>()) {}

    ~scope();

    scope(const scope&) = delete;
    scope& operator=(const scope&) = delete;

  private:
    type _t;
    syntax::block& _b;
  };
}
