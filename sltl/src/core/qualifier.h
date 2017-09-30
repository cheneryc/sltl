#pragma once

#include <memory>


namespace sltl
{
namespace core
{
  enum class qualifier_storage
  {
    default,
    in,
    out,
    uniform
  };

  enum class qualifier_param
  {
    in,
    inout,
    out
  };

  class qualifier
  {
  public:
    // Non-copyable and non-assignable
    qualifier(const qualifier&) = delete;
    qualifier& operator=(const qualifier&) = delete;

    typedef std::unique_ptr<qualifier> ptr;

    template<typename T, typename ...A>
    static ptr make(A&&... a)
    {
      return ptr(new T(std::forward<A>(a)...));
    }

  protected:
    qualifier() = default;
  };

  class storage_qualifier : public qualifier
  {
  public:
    storage_qualifier(qualifier_storage value) : _value(value) {}

    const qualifier_storage _value;
  };

  class param_qualifier : public qualifier
  {
  public:
    param_qualifier(qualifier_param value) : _value(value) {}

    const qualifier_param _value;
  };
}
}
