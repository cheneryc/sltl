#pragma once

#include "byte.h"

//TODO: move some 'detail' functionality to a 'sltl_core' library
#include "../../../sltl/src/detail/detect.h"
#include "../../../sltl/src/detail/assert.h"
#include "../../../sltl/src/detail/priority.h"
#include "../../../sltl/src/detail/variadic_traits.h"
#include "../../../sltl/src/detail/variadic_algorithm.h"

#include <new>
#include <utility>
#include <exception>

#include <cassert>


namespace sltl
{
namespace api
{
namespace detail
{
  // Note that std::is_constructible doesn't seem to have the exact desired behaviour (accepts implicit conversions that result in various warnings)
  template<typename T, typename ...A>
  using is_constructible_op = decltype(T{ std::declval<A>()... });

  template<typename T, typename ...A>
  using is_constructible = sltl::detail::detect<void, is_constructible_op, T, A...>;

  struct variant_index
  {
    variant_index(size_t index) : _index(index)
    {
    }

    const size_t _index;
  };

  template<typename T>
  struct variant_tag
  {
  };

  // N - storage size (in bytes)
  // A - storage alignment
  template<size_t A, size_t N>
  struct variant_storage
  {
    alignas(A) byte _storage[N];
  };

  template<typename ...T>
  struct variant_union : variant_storage<sltl::detail::max<alignof(T)...>::value, sltl::detail::max<sizeof(T)...>::value>
  {
  };

  template<typename ...T>
  class variant final : private variant_union<T...>
  {
    template<size_t N>
    struct variant_param
    {
      typedef typename sltl::detail::get<N, T...>::type type;
    };

  public:
    variant(variant_index var_idx) : _index(var_idx._index)
    {
      assert(var_idx._index < sizeof...(T));
    }

    ~variant()
    {
      assert(!_is_constructed);
    }

    template<typename ...A>
    void construct(A&& ...a)
    {
      visit(construct_functor(), std::forward<A>(a)...);
    }

    template<typename TCon, typename ...A>
    auto construct_type(A&& ...a) -> TCon*
    {
#ifdef _DEBUG
      constexpr static size_t index_type = sltl::detail::get_index<TCon, T...>::value;
#endif

      assert(!_is_constructed);
      assert(_index == index_type);

      return construct_type_impl(variant_tag<TCon>(), sltl::detail::priority<2U>(), std::forward<A>(a)...);
    }

    void destruct()
    {
      visit(destruct_functor());
    }

    template<typename TDes>
    void destruct_type()
    {
#ifdef _DEBUG
      constexpr static size_t index_type = sltl::detail::get_index<TDes, T...>::value;
#endif

      assert(_is_constructed);
      assert(_index == index_type);

      static_cast<TDes*>(get_ptr())->~TDes();

#ifdef _DEBUG
      _is_constructed = false;
#endif
    }

    template<typename Fn, typename ...A>
    void visit(Fn fn, A&& ...a)
    {
      visit_impl(fn, _index, std::forward<A>(a)...);
    }

    template<typename TGet>
    const TGet& get_type() const
    {
#ifdef _DEBUG
      constexpr static size_t index_type = sltl::detail::get_index<TGet, T...>::value;
#endif

      assert(_is_constructed);
      assert(_index == index_type);

      return *static_cast<const TGet*>(get_ptr());
    }

  private:
    struct destruct_functor
    {
      template<typename TTag>
      void operator()(variant& var, variant_tag<TTag>)
      {
        var.destruct_type<TTag>();
      }
    };

    struct construct_functor
    {
      template<typename TTag, typename ...A>
      void operator()(variant& var, variant_tag<TTag>, A&& ...a)
      {
        var.construct_type<TTag>(std::forward<A>(a)...);
      }
    };

    template<typename TCon, typename ...A>
    auto construct_type_impl(variant_tag<TCon>, sltl::detail::priority<2U>, A&& ...a) -> typename std::enable_if<is_constructible<TCon, A...>::value, TCon*>::type
    {
      TCon* const typed_ptr = new(get_ptr()) TCon{std::forward<A>(a)...};

#ifdef _DEBUG
      _is_constructed = true;
#endif

      return typed_ptr;
    }

    template<typename TCon, typename ...A>
    auto construct_type_impl(variant_tag<TCon>, sltl::detail::priority<1U>, A&& ...a) -> typename std::enable_if<sltl::detail::any<is_constructible<T, A...>...>::value, TCon*>::type
    {
      throw std::exception();//TODO: exception type and message
    }

    template<typename TCon, typename ...A>
    auto construct_type_impl(variant_tag<TCon>, sltl::detail::priority<0U>, A&& ...a) -> TCon*
    {
      static_assert(sltl::detail::fail<TCon>::value, "sltl::api::variant::construct_type: specified argument types cannot be used to construct any of the variant's class template parameters");
    }

    template<typename Fn, size_t N, typename ...A>
    static void visit_type_impl(variant& var, Fn fn, A&& ...a)
    {
      fn(var, variant_tag<typename variant_param<N>::type>(), std::forward<A>(a)...);
    }

    template<typename Fn, size_t ...Ns, typename ...A>
    void visit_impl(Fn fn, size_t index, std::index_sequence<Ns...>, A&& ...a)
    {
      using visit_fn_t = void(variant&, Fn, A&& ...a);

      static constexpr visit_fn_t* fn_ptrs[] = {
          &visit_type_impl<Fn, Ns, A...>...
      };

      fn_ptrs[index](*this, fn, std::forward<A>(a)...);
    }

    template<typename Fn, typename ...A>
    void visit_impl(Fn fn, size_t index, A&& ...a)
    {
      visit_impl(fn, index, std::make_index_sequence<sizeof...(T)>(), std::forward<A>(a)...);
    }

    void* get_ptr()
    {
      return _storage;
    }

    const void* get_ptr() const
    {
      return _storage;
    }

    const size_t _index;

#ifdef _DEBUG
    bool _is_constructed = false;
#endif
  };
}
}
}
