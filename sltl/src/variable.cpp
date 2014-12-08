#include "variable.h"

#include "syntax/block_manager.h"
#include "syntax/variable_declaration.h"


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(const language::type& type) : _declaration(syntax::get_current_block().add<syntax::variable_declaration>(type)) {}
ns::variable::variable(const language::type& type, syntax::expression::ptr&& initializer) : _declaration(syntax::get_current_block().add<syntax::variable_declaration>(type, std::move(initializer))) {}
