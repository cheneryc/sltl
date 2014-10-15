#include "variable.h"

#include "syntax/block_manager.h"
#include "syntax/variable_declaration.h"


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(language::type_id id) : _vd(nullptr)
{
  auto vd = std::make_unique<syntax::variable_declaration>(id);
  _vd = vd.get();
  syntax::get_current_block().add(std::move(vd));
}
