#pragma once

#include "function_definition.h"

#include "detail/function_traits.h"

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
  //TODO: Manager (needs a better name?) is a singleton but might actually be better as thread-local?
  class function_manager
  {
  public:
    static function_manager& get()
    {
      static function_manager _manager;
      return _manager;
    }

    template<typename Fn>
    const function_definition* get_definition(Fn fn)
    {
      auto it = _function_map.find(fn);

      if(it != _function_map.end())
      {
        return it->second.get();
      }
      else
      {
        return nullptr;
      }
    }

    template<typename Fn>
    function_definition* add(Fn fn)
    {
      auto result = _function_map.emplace(fn, std::make_unique<function_definition>(fn, ::create_name(_function_map.size()), language::type_helper<typename detail::function_traits<Fn>::return_t>()));

      if(result.second)
      {
        return result.first->second.get();
      }
      else
      {
        return nullptr;
      }
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
}
}
