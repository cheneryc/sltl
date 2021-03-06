#include "glsl_language.h"

#include <syntax/variable_declaration.h>

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::glsl;
}

bool ns::is_variable_built_in(sltl::core::semantic semantic, sltl::core::semantic_index_t semantic_index)
{
  bool is_built_in = false;

  if(semantic == core::semantic::system)
  {
    const auto semantic_system_pair = core::detail::to_semantic_system_pair(semantic_index);

    switch(semantic_system_pair.first)
    {
      // Only a built-in variable if the semantic is position or depth
      // Only a built-in variable if the index is zero
      case core::semantic_system::position:
      case core::semantic_system::depth:
        is_built_in = !semantic_system_pair.second;
        break;
    }
  }

  return is_built_in;
}

bool ns::is_variable_built_in(const sltl::syntax::variable_declaration& vd)
{
  return is_variable_built_in(vd._semantic, vd._semantic_index);
}

std::wstring ns::to_type_string(const sltl::language::type& t)
{
  std::wstringstream ss;

  const language::type_id id = t.get_id();
  const language::type_dimensions& dimensions = t.get_dimensions();

  if(dimensions.is_void())
  {
    assert(id == language::id_void);
    assert(dimensions.m() == 0U);
    assert(dimensions.n() == 0U);

    ss << L"void";
  }
  else if(dimensions.is_scalar())
  {
    switch(id)
    {
      case language::id_float:
        ss << L"float";
        break;
      case language::id_double:
        ss << L"double";
        break;
      case language::id_int:
        ss << L"int";
        break;
      case language::id_uint:
        ss << L"unsigned int";
        break;
      case language::id_bool:
        ss << L"bool";
        break;
      default:
        assert((id != language::id_unknown) && (id != language::id_void));
    }
  }
  else if(dimensions.is_vector())
  {
    switch(id)
    {
      case language::id_float:
        ss << L"vec";
        break;
      case language::id_double:
        ss << L"dvec";
        break;
      case language::id_int:
        ss << L"ivec";
        break;
      case language::id_uint:
        ss << L"uvec";
        break;
      case language::id_bool:
        ss << L"bvec";
        break;
      default:
        assert((id != language::id_unknown) && (id != language::id_void));
    }

    ss << (is_row_vector(dimensions) ? dimensions.n() : dimensions.m());
  }
  else if(dimensions.is_matrix())
  {
    switch(id)
    {
      case language::id_float:
        ss << L"mat";
        break;
      case language::id_double:
        ss << L"dmat";
        break;
      default:
        assert((id == language::id_float) || (id == language::id_double));
    }

    // GLSL has column-major matrices so always output 'nxm'
    ss << dimensions.n() << L'x' << dimensions.m();
  }
  else
  {
    assert(dimensions.is_void() || dimensions.is_scalar() || dimensions.is_vector() || dimensions.is_matrix());
  }

  return ss.str();
}

std::wstring ns::to_built_in_string(sltl::core::semantic semantic, sltl::core::semantic_index_t semantic_index)
{
  assert(is_variable_built_in(semantic, semantic_index));

  const auto semantic_system_pair = core::detail::to_semantic_system_pair(semantic_index);

  //TODO: validation that the built-in is of the correct type and used in the correct shader stage

  std::wstringstream ss;

  switch(semantic_system_pair.first)
  {
  case core::semantic_system::position:
    //TODO: if the variable is vertex shader output then this is gl_Position
    //TODO: if the variable is pixel shader input then this is gl_FragCoord
    //TODO: note that index must be zero for position semantic
    ss << L"gl_Position";
    break;
  case core::semantic_system::depth:
    //TODO: only valid as a fragment shader output
    //TODO: note that index must be zero for depth semantic
    ss << L"gl_FragDepth";
    break;
  }

  // Revisit this once built-in variables other than gl_Position & gl_FragDepth are supported
  assert(semantic_system_pair.second == 0);

  if(semantic_system_pair.second > 0)
  {
    ss << L'[' << semantic_system_pair.second << L']';
  }

  return ss.str();
}

std::wstring ns::to_built_in_string(const sltl::syntax::variable_declaration& vd)
{
  return to_built_in_string(vd._semantic, vd._semantic_index);
}

const wchar_t* ns::to_qualifier_string(sltl::core::qualifier_storage id)
{
  switch(id)
  {
  case sltl::core::qualifier_storage::in:
    return L"in";
  case sltl::core::qualifier_storage::out:
    return L"out";
  case sltl::core::qualifier_storage::uniform:
    return L"uniform";
  }

  return nullptr;
}
