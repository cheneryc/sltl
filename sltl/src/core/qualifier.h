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
}
}
