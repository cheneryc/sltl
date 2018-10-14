#pragma once

#include "element.h"

#include "syntax/block.h"


namespace sltl
{
  class scope : public element
  {
  public:
    scope();

    template<typename S>
    scope(S& s) : _b(s.template add<syntax::block>(syntax::block::local))
    {
      _b.push();
    }

    scope(const scope&) = delete;
    scope& operator=(const scope&) = delete;

    ~scope();

  private:
    syntax::block& _b;
  };
}
