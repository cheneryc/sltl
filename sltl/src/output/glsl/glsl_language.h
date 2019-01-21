#pragma once

#include <core/qualifier.h>
#include <core/semantic.h>

#include <string>


namespace sltl
{
  // Forward declarations - sltl::language namespace
  namespace language
  {
    class type;
  }

  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class variable_declaration;
  }

namespace glsl
{
  bool is_variable_built_in(core::semantic semantic, core::semantic_index_t semantic_index);
  bool is_variable_built_in(const syntax::variable_declaration& vd);

  std::wstring to_type_string(const language::type& t);

  std::wstring to_built_in_string(core::semantic semantic, core::semantic_index_t semantic_index);
  std::wstring to_built_in_string(const syntax::variable_declaration& vd);

  const wchar_t* to_qualifier_string(core::qualifier_storage id);
}
}
