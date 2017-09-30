#pragma once

#include "intrinsic_declaration.h"

#include <map>


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

    const intrinsic_declaration& emplace(core::intrinsic i, parameter_list&& parameters, const language::type& type_return)
    {
      auto id = std::make_unique<intrinsic_declaration>(i, std::move(parameters), type_return);
      auto id_ref = std::ref(*id);

      return *(_intrinsic_map.emplace(id_ref, std::move(id)).first->second);
    }

    template<typename T>
    void move(T& t)
    {
      for(auto it = _intrinsic_map.begin(); it != _intrinsic_map.end(); ++it)
      {
        t.emplace_back(std::move(it->second));
      }

      _intrinsic_map.clear();
    }

  private:
    //TODO: replace the map with a set and use std::set::extract to transfer ownership of the intrinsic declarations (C++17)
    std::map<std::reference_wrapper<intrinsic_declaration>, intrinsic_declaration::ptr> _intrinsic_map;
  };
}
}
