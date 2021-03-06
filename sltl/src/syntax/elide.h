#pragma once

#include "expression.h"


namespace sltl
{
  // Forward declarations - sltl::language namespace
  namespace language
  {
    class type;
  }

namespace syntax
{
  expression::ptr&& elide(expression::ptr&& exp, const language::type& type);
}
}
