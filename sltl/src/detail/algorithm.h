#pragma once

#include <algorithm>


namespace sltl
{
namespace detail
{
  template<typename It, typename Fn>
  void for_each_value(It it_begin, It it_end, Fn fn)
  {
    using pair_type = decltype(*it_begin);

    std::for_each(it_begin, it_end, [&fn](pair_type pair)
    {
      fn(pair.second);
    });
  }
}
}
