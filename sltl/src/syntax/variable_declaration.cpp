#include "variable_declaration.h"

#include "block_base.h"
#include "block_manager.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::variable_declaration::variable_declaration(std::wstring&& name, ns::expression::ptr&& initializer) : declaration_statement(std::move(name)),
  _semantic(core::semantic_pair::none._semantic),
  _semantic_index(core::semantic_pair::none._index),
  _qualifier(core::qualifier_storage::none),
  _type(),
  _initializer(std::move(initializer))
{
  assert(_initializer);
}

ns::variable_declaration::variable_declaration(std::wstring&& name, const sltl::language::type& type, sltl::core::qualifier_storage qualifier, sltl::core::semantic_pair semantic) : declaration_statement(std::move(name)),
  _semantic(semantic._semantic),
  _semantic_index(semantic._index),
  _qualifier(qualifier),
  _type(std::make_unique<language::type>(type)),
  _initializer()
{
}
