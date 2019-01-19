#pragma once

#include <functional>


namespace sltl
{
namespace detail
{
  template<typename T>
  struct ref_hash
  {
    size_t operator()(const std::reference_wrapper<T>& ref) const
    {
      return std::hash<T>()(ref.get());
    }
  };

  template<typename T>
  struct cref_hash
  {
    size_t operator()(const std::reference_wrapper<const T>& ref) const
    {
      return std::hash<T>()(ref.get());
    }
  };

  template<typename T>
  struct ref_equal_to
  {
    size_t operator()(const std::reference_wrapper<T>& lhs, const std::reference_wrapper<T>& rhs) const
    {
      return lhs.get() == rhs.get();
    }
  };

  template<typename T>
  struct cref_equal_to
  {
    size_t operator()(const std::reference_wrapper<const T>& lhs, const std::reference_wrapper<const T>& rhs) const
    {
      return lhs.get() == rhs.get();
    }
  };
}
}
