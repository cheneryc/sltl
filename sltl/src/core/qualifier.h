#pragma once

#include <memory>


namespace sltl
{
namespace core
{
  enum class qualifier_storage
  {
    none,
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
}
}
