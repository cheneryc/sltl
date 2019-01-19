#pragma once

#include "function_definition.h"

#include <detail/algorithm.h>
#include <detail/reference.h>
#include <detail/comparison.h>
#include <detail/function_traits.h>
#include <detail/scoped_singleton.h>

#include <unordered_map>


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
    function_manager() : _function_depth(0U) {}

    template<typename Fn>
    const function_definition& emplace(Fn fn)
    {
      return emplace(fn, create_name(_function_map.size()));
    }

    template<typename Fn>
    const function_definition& emplace(Fn fn, std::wstring&& name)
    {
      // Pass an empty unique_ptr so that a function_definition instance is
      // only ever created when the call to emplace results in an insertion
      auto result = _function_map.emplace(fn, std::unique_ptr<function_definition>());

      if(result.second)
      {
        ++_function_depth;
        result.first->second = std::make_unique<function_definition>(fn, std::move(name), language::type_helper<typename detail::function_traits<Fn>::return_t>());
        --_function_depth;

        const std::pair<size_t, size_t> depth_id = {
          _function_depth,
          _function_map.size() - 1U
        };

        _function_depth_map.emplace(result.first->second, depth_id);
      }
      else
      {
        std::pair<size_t, size_t>& depth_id = _function_depth_map.at(result.first->second);

        // Assign the depth of this invocation of the function to the map if it is greater than the function's current depth value
        if(_function_depth > depth_id.first)
        {
          depth_id.first = _function_depth;
        }
      }

      return *(result.first->second);
    }

    std::vector<function_definition::ptr> transfer()
    {
      std::vector<function_definition::ptr> functions(_function_map.size());

      detail::for_each_value(
        _function_map.begin(),
        _function_map.end(),
        [this, it = functions.begin()](function_definition::ptr &fn_def) mutable
      {
        const std::pair<size_t, size_t> depth_id = _function_depth_map.at(fn_def);

        fn_def->set_depth(depth_id.first);
        fn_def->set_depth_id(depth_id.second);

        *(it++) = std::move(fn_def); // Transfer ownership of the function definition last to avoid de-referencing an empty unique_ptr
      });

      _function_map.clear();
      _function_depth = 0U;
      _function_depth_map.clear();

      std::sort(functions.begin(), functions.end(), [](const function_definition::ptr &lhs, const function_definition::ptr &rhs)
      {
        const auto lhs_depth_id = std::make_pair(lhs->get_depth(), lhs->get_depth_id());
        const auto rhs_depth_id = std::make_pair(rhs->get_depth(), rhs->get_depth_id());

        return lhs_depth_id > rhs_depth_id;
      });

      return functions;
    }

  private:
    static std::wstring create_name(size_t function_count)
    {
      return L"fn" + std::to_wstring(function_count);
    }

    using function_definition_ptr_ref = std::reference_wrapper<const function_definition::ptr>;

    std::unordered_map<function_key, function_definition::ptr, function_key::hash> _function_map;
    std::unordered_map<function_definition_ptr_ref, std::pair<size_t, size_t>,
      detail::cref_hash<function_definition::ptr>,
      detail::cref_equal_to<function_definition::ptr>> _function_depth_map;

    size_t _function_depth; // Current depth of function 'call stack'
  };

  typedef detail::scoped_singleton<function_manager, detail::scope_t::thread> function_manager_guard;
}
}
