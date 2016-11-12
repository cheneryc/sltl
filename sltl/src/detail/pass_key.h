#pragma once


namespace sltl
{
  namespace detail
  {
    template<typename T>
    class pass_key
    {
      friend T;

      pass_key() {}
      pass_key(const pass_key&) {}
    };
  }
}
