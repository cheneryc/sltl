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

  template<typename T, typename ...A>
  node::ptr make_node_ptr(A&&... a)
  {
    return std::unique_ptr<node>(new T(std::forward<A>(a)...));
  }
}
}
