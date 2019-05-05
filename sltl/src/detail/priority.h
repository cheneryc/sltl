#pragma once


namespace sltl
{
namespace detail
{
  // Used as a function parameter to 'rank' function overloads by order of precedence. A function overload
  // with a priority parameter whose template parameter N is greater than another has the higher priority.
  template<size_t N>
  struct priority : priority<N - 1U>
  {
  };

  template<>
  struct priority<0U>
  {
  };
}
}
