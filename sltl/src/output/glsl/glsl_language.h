#pragma once

#include <language.h>

#include <core/qualifier.h>
#include <core/semantic.h>

#include <string>


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class variable_declaration;
  }

namespace glsl
{
  bool is_variable_built_in(core::semantic semantic, core::semantic_index_t semantic_index);
  bool is_variable_built_in(const syntax::variable_declaration& vd);

  std::wstring to_built_in_string(core::semantic semantic, core::semantic_index_t semantic_index);
  std::wstring to_built_in_string(const syntax::variable_declaration& vd);

  std::wstring to_type_string(const language::type& t);

  const wchar_t* to_operator_unary_string(language::operator_unary_id id);
  const wchar_t* to_operator_binary_string(language::operator_binary_id id);
  const wchar_t* to_conditional_string(language::conditional_id id);
  const wchar_t* to_component_string(language::type_dimension_t component_idx);
  const wchar_t* to_keyword_string(language::keyword_id id);
  const wchar_t* to_qualifier_string(core::qualifier_storage id);
}
}
