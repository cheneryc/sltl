#pragma once

#include "block.h"
#include "action.h"

#include "../core/qualifier.h"
#include "../language.h"

#include <algorithm>


namespace sltl
{
namespace syntax
{
  //TODO: the block needs a name based on the shader type (vs, ps etc.) and whether it is in or out
  //TODO: the variable declaration naming algorithm could also do with being tweaked for io_blocks
  class io_block : public block
  {
  public:
    io_block(core::qualifier_storage qualifier) : block(global), _qualifier(qualifier) {}

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _statements.begin(), _statements.end());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _statements.begin(), _statements.end());
    }

    const core::qualifier_storage _qualifier;
  };
}
}
