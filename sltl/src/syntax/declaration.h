#pragma once

#include <language.h>

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

    // Returns a copy, rather than reference, as the type may need to be calculated on demand (e.g. operator_binary)
    virtual language::type get_type() const = 0;

    const std::wstring _name;

  protected:
    declaration(std::wstring&& name) : _name(std::move(name)) {}
  };
}
}
