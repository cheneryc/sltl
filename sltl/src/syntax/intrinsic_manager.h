#pragma once

#include "intrinsic_declaration.h"

#include <set>


namespace sltl
{
namespace syntax
{
  class intrinsic_manager
  {
  public:
    static intrinsic_manager& get()
    {
      static intrinsic_manager _manager;
      return _manager;
    }

    const intrinsic_declaration* get_definition(const intrinsic_declaration& definition)
    {
      auto it = _intrinsic_set.find(definition);

      if(it != _intrinsic_set.end())
      {
        return &(*it);
      }
      else
      {
        return nullptr;
      }
    }

    const intrinsic_declaration* add(intrinsic_declaration&& definition)
    {
      auto result = _intrinsic_set.insert(std::move(definition));

      if(result.second)
      {
        return &(*result.first);
      }
      else
      {
        return nullptr;
      }
    }

  private:
    std::set<intrinsic_declaration> _intrinsic_set;
  };
}
}
