#pragma once

#include "variable_declaration.h"


namespace sltl
{
namespace syntax
{
  class temporary : public expression
  {
  public:
    temporary(variable_declaration&& declaration) : expression(), _id(declaration._id), _initializer(declaration.move())
    {
      syntax::get_current_block().erase(declaration);
    }

    virtual void traverse(output& out) const
    {
      out(*this);

      if(_initializer)
      {
        _initializer->traverse(out);
      }

      out(*this, false);
    }

    const language::type_id _id;

  private:
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
