#pragma once


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class node
  {
  public:
    // Make sure all derived types are non-copyable
    node(const node&) = delete;
    node& operator=(const node&) = delete;

    virtual ~node()
    {
    }

    virtual bool apply_action(action& act) = 0;
    virtual bool apply_action(const_action& cact) const = 0;

  protected:
    node() = default;
  };
}
}
