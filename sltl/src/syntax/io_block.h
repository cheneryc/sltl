#pragma once

#include "block_base.h"

#include "../core/qualifier.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class io_block : public block_base
  {
  public:
    io_block(core::qualifier_storage qualifier);

    void pop();

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

    const core::qualifier_storage _qualifier;
  };
}
}
