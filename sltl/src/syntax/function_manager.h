#pragma once

#include "function_definition.h"

#include <detail/function_traits.h>
#include <detail/scoped_singleton.h>

#include <map>


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
    std::map<void*, function_definition::ptr> _function_map;
  };

  typedef detail::scoped_singleton<function_manager, detail::scope_t::thread> function_manager_guard;
}
}
