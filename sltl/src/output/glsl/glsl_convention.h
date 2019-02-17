#pragma once

#include <core/qualifier.h>

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
  std::wstring to_type_prefix_string(const language::type& t);

  const wchar_t* to_qualifier_prefix_string(core::qualifier_storage id);
  const wchar_t* to_parameter_prefix_string(core::qualifier_param id);

  std::wstring get_variable_name(const syntax::variable_declaration& vd);
}
}
