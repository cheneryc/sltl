#pragma once

#include <string>


namespace sltl
{
namespace syntax
{
  class declaration
  {
  public:
    // Make sure all derived types are non-copyable
    declaration(const declaration&) = delete;
    declaration& operator=(const declaration&) = delete;

    virtual ~declaration()
    {
    }

    const std::wstring _name;

  protected:
    declaration(std::wstring&& name) : _name(std::move(name)) {}
  };
}
}
