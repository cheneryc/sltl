#pragma once

#include "block.h"
#include "action.h"
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
    io_block(language::qualifier_id id) : block(global), _id(id) {}

    virtual bool apply_action(action& act) override
    {
      return (act(*this) && std::all_of(_statements.begin(), _statements.end(), [&act](statement::ptr& s){ return s->apply_action(act); }) && act(*this, false));
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return (cact(*this) && std::all_of(_statements.begin(), _statements.end(), [&cact](const statement::ptr& s){ return s->apply_action(cact); }) && cact(*this, false));
    }

    const language::qualifier_id _id;
  };
}
}
