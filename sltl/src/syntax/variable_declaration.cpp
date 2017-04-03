#include "variable_declaration.h"

#include "block_base.h"
#include "block_manager.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::variable_declaration::variable_declaration(ns::expression::ptr&& initializer) : declaration_statement(get_current_block().get_child_name()),
  _type(),
  _semantic(core::semantic_pair::none._semantic),
  _semantic_index(core::semantic_pair::none._index),
  _qualifier(core::qualifier::make<core::storage_qualifier>(core::qualifier_storage::default)),
  _initializer(std::move(initializer))
{
  assert(_initializer);
}

ns::variable_declaration::variable_declaration(const sltl::language::type& type, sltl::core::qualifier::ptr&& qualifier, sltl::core::semantic_pair semantic) : declaration_statement(get_current_block().get_child_name()),
  _type(std::make_unique<language::type>(type)),
  _semantic(semantic._semantic),
  _semantic_index(semantic._index),
  _qualifier(std::move(qualifier)),
  _initializer()
{
}
