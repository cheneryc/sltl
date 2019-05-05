#pragma once

#include "byte.h"


namespace sltl
{
namespace api
{
namespace detail
{
  class blob_view final
  {
  public:
    blob_view(byte* const bytes, size_t byte_length) :
      _byte_length(byte_length),
      _bytes(bytes)
    {
    }

    byte* get_bytes()
    {
      return _bytes;
    }

    const byte* get_bytes() const
    {
      return _bytes;
    }

    const size_t _byte_length;

  private:
    byte* const _bytes;
  };
}
}
}
