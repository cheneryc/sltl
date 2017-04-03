#include "variable.h"

#include "syntax/block.h"
#include "syntax/block_manager.h"
#include "syntax/reference.h"
#include "syntax/temporary.h"
#include "syntax/variable_info.h"
#include "syntax/variable_declaration.h"

#include <cassert>


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(syntax::expression::ptr&& initializer) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(std::move(initializer)))) {}
ns::variable::variable(const language::type& type, core::semantic_pair semantic) : _declaration(&(syntax::get_current_block().add<syntax::variable_declaration>(type, semantic))) {}

ns::syntax::expression::ptr ns::variable::make_reference() const
{
  get_variable_info(_declaration).inc_ref();

  return syntax::expression::make<syntax::reference>(*_declaration);
}

ns::syntax::expression::ptr ns::variable::make_reference_or_temporary()
{
  if(get_variable_info(_declaration).get_ref() > 0)
  {
    return make_reference();
  }
  else
  {
    return make_temporary();
  }
}

ns::syntax::expression::ptr ns::variable::make_temporary()
{
  assert(get_variable_info(_declaration).get_ref() == 0);

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
