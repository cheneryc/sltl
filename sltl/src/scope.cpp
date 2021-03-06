#include "scope.h"

#include "syntax/block.h"
#include "syntax/block_manager.h"


namespace
{
  namespace ns = sltl;
}

ns::scope::scope() : scope(syntax::get_current_block()) {}

ns::scope::~scope()
{
  _b.pop();
}
