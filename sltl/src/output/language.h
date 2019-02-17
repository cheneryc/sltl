#pragma once

#include <type.h>


namespace sltl
{
  const wchar_t* to_operator_unary_string(language::operator_unary_id id);
  const wchar_t* to_operator_binary_string(language::operator_binary_id id);
  const wchar_t* to_conditional_string(language::conditional_id id);
  const wchar_t* to_component_string(language::type_dimension_t component_idx);
  const wchar_t* to_keyword_string(language::keyword_id id);
}
