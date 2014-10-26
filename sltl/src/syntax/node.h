#pragma once


namespace sltl
{
  // Forward declaration - sltl namespace
  class output;

namespace syntax
{
  class node
  {
  public:
    // Make sure all derived types are non-copyable
    node(const node&) = delete;
    node& operator=(const node&) = delete;

    virtual ~node()
    {
    }
    
    virtual void traverse(output& out) const = 0;

  protected:
    node() = default;
  };
}
}
