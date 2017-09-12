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

    const intrinsic_declaration& emplace(core::intrinsic i, parameter_list&& parameters, const language::type& type_return)
    {
      return *(_intrinsic_set.emplace(i, std::move(parameters), type_return).first);
    }

  private:
    std::set<intrinsic_declaration> _intrinsic_set;
  };
}
}
