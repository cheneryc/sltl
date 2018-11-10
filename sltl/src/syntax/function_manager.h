#pragma once

#include "function_definition.h"

#include <detail/comparison.h>
#include <detail/function_traits.h>
#include <detail/scoped_singleton.h>

#include <unordered_map>


namespace
{
  namespace ns = sltl::syntax;

  std::wstring create_name(size_t function_count)
  {
    return L"fn" + std::to_wstring(function_count + 1);
  }
}

namespace sltl
{
namespace syntax
{
  class function_key
  {
  public:
    struct hash
    {
      size_t operator()(const function_key& fn_key) const
      {
        return fn_key.m_impl->hash_code();
      }
    };

    template<typename Fn>
    function_key(Fn fn) : m_impl(std::make_unique<impl<Fn>>(fn)) {}

    friend bool operator==(const function_key& lhs, const function_key& rhs)
    {
      return lhs.m_impl->is_equal(*(rhs.m_impl));
    }

  private:
    struct impl_base
    {
      impl_base(size_t fn_hash) : m_fn_hash(fn_hash)
      {
      }

      virtual bool is_equal(const impl_base& other) const = 0;

      size_t hash_code() const
      {
        return m_fn_hash;
      }

      const size_t m_fn_hash;
    };

    template<typename Fn, bool = detail::is_equality_comparable<Fn>::value>
    struct impl : impl_base
    {
      impl(Fn fn) : impl_base({}), m_fn(fn) {}

      bool is_equal(const impl_base&) const override
      {
        return false; // Callable type is not equality comparable
      }

      Fn m_fn;
    };

    // Partial specialization for equality comparable callable types
    template<typename Fn>
    struct impl<Fn, true> : impl_base
    {
      impl(Fn fn) : impl_base({}), m_fn(fn) {}

      bool is_equal(const impl_base& other) const override
      {
        if(auto* other_cast = dynamic_cast<const impl<Fn>*>(&other))
        {
          return other_cast->m_fn == m_fn; // Call the equality operator on the callable type
        }
        else
        {
          return false;
        }
      }

      Fn m_fn;
    };

    // Partial specialization for function pointers
    template<typename R, typename ...P>
    struct impl<R(*)(P...), true> : impl_base
    {
      impl(R(*fn)(P...)) : impl_base(reinterpret_cast<std::intptr_t>(fn)), m_fn(fn) {}

      bool is_equal(const impl_base& other) const override
      {
        if(auto* other_cast = dynamic_cast<const impl<R(*)(P...)>*>(&other))
        {
          return other_cast->m_fn == m_fn; // Compare the function pointers
        }
        else
        {
          return false;
        }
      }

      R(*m_fn)(P...);
    };

    std::unique_ptr<impl_base> m_impl;
  };

  class function_manager
  {
  public:
    template<typename Fn>
    const function_definition& emplace(Fn fn)
    {
      // Pass an empty unique_ptr so that a function_definition instance is
      // only ever created when the call to emplace results in an insertion
      auto result = _function_map.emplace(fn, std::unique_ptr<function_definition>());

      if(result.second)
      {
        result.first->second = std::make_unique<function_definition>(fn, ::create_name(_function_map.size() - 1U), language::type_helper<typename detail::function_traits<Fn>::return_t>());
      }

      return *(result.first->second);
    }

    template<typename T>
    void move(T& t)
    {
      for(auto it = _function_map.begin(); it != _function_map.end(); ++it)
      {
        t.emplace_back(std::move(it->second));
      }

      _function_map.clear();
    }

  private:
    std::unordered_map<function_key, function_definition::ptr, function_key::hash> _function_map;
  };

  typedef detail::scoped_singleton<function_manager, detail::scope_t::thread> function_manager_guard;
}
}
