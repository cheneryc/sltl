#pragma once

#include <cstddef>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class variable_declaration;

  class variable_info
  {
  public:
    variable_info() : _ref_count(0) {}

    void inc_ref()
    {
      ++_ref_count;
    }

    void dec_ref()
    {
      --_ref_count;
    }

    size_t get_ref() const
    {
      return _ref_count;
    }

  private:
    size_t _ref_count;
  };

  variable_info& get_variable_info(const variable_declaration* vd);
}
}
