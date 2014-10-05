#pragma once

#include <memory>


namespace sltl
{
  // Foward declaration - sltl namespace
  class output;

namespace syntax
{
  class node
  {
  public:
    typedef std::unique_ptr<node> ptr;

    virtual ~node()
    {
    }
    
    virtual void traverse(output& out) const = 0;
  };
}
}
