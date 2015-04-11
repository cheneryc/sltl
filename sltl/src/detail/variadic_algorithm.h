#pragma once


namespace sltl
{
namespace detail
{
  // Does nothing, useful as parameter pack expansion is only allowed in certain
  // contexts (function argument lists are one of the acceptable expansion loci)
  template<typename ...A>
  inline void pass(A&&...) {}
}
}
