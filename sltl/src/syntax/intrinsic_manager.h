#pragma once

#include "intrinsic_declaration.h"

#include <detail/algorithm.h>
#include <detail/scoped_singleton.h>

#include <map>


namespace sltl
{
namespace syntax
{
  class intrinsic_manager
  {
  public:
    const intrinsic_declaration& emplace(core::intrinsic i, parameter_list&& parameters, const language::type& type_return)
    {
      auto id = std::make_unique<intrinsic_declaration>(i, std::move(parameters), type_return);
      auto id_ref = std::cref(*id);

      return *(_intrinsic_map.emplace(id_ref, std::move(id)).first->second);
    }

    std::vector<intrinsic_declaration::ptr> transfer()
    {
      std::vector<intrinsic_declaration::ptr> intrinsics(_intrinsic_map.size());

      detail::for_each_value(
        _intrinsic_map.begin(),
        _intrinsic_map.end(),
        [this, it = intrinsics.begin()](intrinsic_declaration::ptr &intrinsic_dec) mutable
      {
        *(it++) = std::move(intrinsic_dec);
      });

      _intrinsic_map.clear();
      return intrinsics;
    }

  private:
    //TODO: replace the map with a set and use std::set::extract to transfer ownership of the intrinsic declarations (C++17)
    std::map<std::reference_wrapper<const intrinsic_declaration>, intrinsic_declaration::ptr> _intrinsic_map;
  };

  typedef detail::scoped_singleton<intrinsic_manager, detail::scope_t::thread> intrinsic_manager_guard;
}
}
