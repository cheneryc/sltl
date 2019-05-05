#pragma once

#include "blob_view.h"

#include <memory>


namespace sltl
{
namespace api
{
namespace detail
{
  class blob final
  {
  public:
    blob(size_t byte_length) :
      _byte_length(byte_length),
      _byte_buffer(std::make_unique<byte[]>(byte_length))
    {
    }

    operator blob_view() const
    {
      return {
        _byte_buffer.get(),
        _byte_length
      };
    }

    const size_t _byte_length;

  private:
    std::unique_ptr<byte[]> _byte_buffer;
  };
}
}
}
