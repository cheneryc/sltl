#pragma once

#include "block_base.h"

#include <utility>


namespace sltl
{
namespace syntax
{
  class block_guard final
  {
  public:
    block_guard(block_base& b) : _block(b)
    {
      _block.push();
    }

    template<typename Fn, typename ...T>
    block_guard(block_base& b, Fn fn, T&& ...t) : block_guard(b)
    {
      fn(std::forward<T>(t)...);
    }

    ~block_guard()
    {
      _block.pop();
    }

    // Non-copyable, non-movable and non-assignable
    block_guard(block_guard&&) = delete;
    block_guard(const block_guard&) = delete;
    block_guard& operator=(block_guard&&) = delete;
    block_guard& operator=(const block_guard&) = delete;

  private:
    block_base& _block;
  };
}
}
