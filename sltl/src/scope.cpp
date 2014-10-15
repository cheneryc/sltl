#include "scope.h"
#include "syntax/block.h"
#include "syntax/block_manager.h"


namespace
{
  namespace ns = sltl;
}

ns::scope::scope(type t) : _t(t)
{
  syntax::block_manager& bm = syntax::block_manager::get();
  syntax::block& b = bm.get_block();

  b.add(syntax::make_node_ptr<syntax::block>());
}

ns::scope::~scope()
{
  syntax::get_current_block().pop();
}
