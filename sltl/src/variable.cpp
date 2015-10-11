#include "variable.h"

#include "syntax/block_manager.h"
#include "syntax/temporary.h"
#include "syntax/variable_declaration.h"

#include <cassert>


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(type, std::move(qualifier), semantic))) {}
ns::variable::variable(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic, syntax::expression::ptr&& initializer) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(type, std::move(qualifier), semantic, std::move(initializer)))) {}

ns::syntax::expression::ptr ns::variable::make_temporary()
{
  syntax::variable_declaration* decl_tmp = _declaration; _declaration = nullptr;

  assert(decl_tmp);
  assert(decl_tmp->get_ref_count() == 0);

  return syntax::expression::make<syntax::temporary>(std::move(*decl_tmp));
}
