#pragma once


namespace sltl
{
namespace detail
{
  template<typename T1, typename T2>
  bool less(const T1& t1_lhs, const T2& t2_lhs, const T1& t1_rhs, const T2& t2_rhs)
  {
    return t1_lhs < t1_rhs || t1_lhs == t1_rhs && t2_lhs < t2_rhs;
  }
}
}
