#include "variable.h"

#include "syntax/block_manager.h"
#include "syntax/variable_declaration.h"


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(language::type_id id) : _declaration(syntax::get_current_block().add<syntax::variable_declaration>(id)) {}
ns::variable::variable(language::type_id id, syntax::expression::ptr&& initializer) : _declaration(syntax::get_current_block().add<syntax::variable_declaration>(id, std::move(initializer))) {}
