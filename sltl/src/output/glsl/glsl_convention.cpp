#include "glsl_convention.h"
#include "glsl_language.h"

#include <syntax/variable_declaration.h>

#include <language.h>

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::glsl;
}

std::wstring ns::to_type_prefix_string(const sltl::language::type& t)
{
  // Variable names can't begin with a numeral so we need a type specific prefix
  std::wstring prefix_string;

  const language::type_id id = t.get_id();
  const language::type_dimensions& dimensions = t.get_dimensions();

  if(dimensions.is_scalar())
  {
    switch(id)
    {
      case language::id_float:
        prefix_string = L'f';
        break;
      case language::id_double:
        prefix_string = L'd';
        break;
      case language::id_int:
        prefix_string = L'i';
        break;
      case language::id_uint:
        prefix_string = L'u';
        break;
      case language::id_bool:
        prefix_string = L'b';
        break;
      default:
        assert((id != language::id_unknown) && (id != language::id_void));
    }
  }
  else if(dimensions.is_vector())
  {
    assert(id != language::id_void);
    assert(id != language::id_unknown);

    prefix_string = L'v';
  }
  else if(dimensions.is_matrix())
  {
    assert((id == language::id_float) ||
           (id == language::id_double));

    prefix_string = L'm';
  }
  else
  {
    assert(false);
  }

  return std::move(prefix_string);
}

const wchar_t* ns::to_qualifier_prefix_string(sltl::core::qualifier_storage id)
{
  switch(id)
  {
  case sltl::core::qualifier_storage::in:
    return L"i";
  case sltl::core::qualifier_storage::out:
    return L"o";
  case sltl::core::qualifier_storage::uniform:
    return L"u";
  }

  return nullptr;
}

const wchar_t* ns::to_parameter_prefix_string(sltl::core::qualifier_param id)
{
  switch(id)
  {
    case sltl::core::qualifier_param::in:
    case sltl::core::qualifier_param::inout:
    case sltl::core::qualifier_param::out:
      return L"p";
  }

  return nullptr;
}

std::wstring ns::get_variable_name(const sltl::syntax::variable_declaration& vd)
{
  std::wstringstream ss;

  if(is_variable_built_in(vd))
  {
    ss << to_built_in_string(vd);
  }
  else
  {
    // Prepend the storage qualifier to the variable name to ensure it is globally unique
    if(auto qualifier = to_qualifier_prefix_string(vd._qualifier))
    {
      ss << qualifier << L'_';
    }

    ss << to_type_prefix_string(vd.get_type()) << vd._name;
  }

  return ss.str();
}
