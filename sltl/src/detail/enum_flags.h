#pragma once

#include <type_traits>


namespace sltl
{
namespace detail
{
  template<typename E>
  class enum_flags
  {
    typedef typename std::underlying_type<E>::type underlying_t;

  public:
    enum_flags(E e) : _u(static_cast<underlying_t>(e)) {}

    bool operator==(const enum_flags<E>& rhs) const
    {
      return (_u == rhs._u);
    }

    friend bool operator==(const enum_flags<E>& lhs, const enum_flags<E>& rhs)
    {
      return lhs.operator==(rhs);
    }

    enum_flags<E>& operator|=(const enum_flags<E>& rhs)
    {
      _u |= rhs._u;
      return *this;
    }

    friend enum_flags<E> operator|(const enum_flags<E>& lhs, const enum_flags<E>& rhs)
    {
      enum_flags flags(lhs);

      flags |= rhs;
      return flags;
    }

    enum_flags<E>& operator&=(const enum_flags<E>& rhs)
    {
      _u &= rhs._u;
      return *this;
    }

    friend enum_flags<E> operator&(const enum_flags<E>& lhs, const enum_flags<E>& rhs)
    {
      enum_flags flags(lhs);

      flags &= rhs;
      return flags;
    }

    template<E e>
    bool has_flag() const
    {
      return (_u & static_cast<underlying_t>(e)) != 0U;
    }

    static_assert(std::is_enum<E>::value, "sltl::detail::enum_flags: template parameter E must be an enumeration type.");

  private:
    underlying_t _u;
  };
}
}
