#include "variable.h"

#include "syntax/block_manager.h"
#include "syntax/temporary.h"
#include "syntax/variable_declaration.h"

#include <cassert>


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(syntax::expression::ptr&& initializer) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(std::move(initializer)))) {}
ns::variable::variable(const language::type& type, core::qualifier::ptr&& qualifier, core::semantic_pair semantic) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(type, std::move(qualifier), semantic))) {}

ns::syntax::expression::ptr ns::variable::make_temporary()
{
  assert(_declaration);
  assert(_declaration->get_ref_count() == 0);

  syntax::expression::ptr exp;

  {
    syntax::variable_declaration& declaration = *_declaration; _declaration = nullptr;

    exp = (declaration.has_type() ?
      syntax::expression::make<syntax::temporary>(declaration.get_type()) :
      syntax::expression::make<syntax::temporary>(declaration.move()));

    syntax::get_current_block().erase(declaration);
  }

  return std::move(exp);
}
