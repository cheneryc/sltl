#include "language.h"

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::language;
}

bool ns::is_row_vector(const type_dimensions& td)
{
  assert(td.is_vector());
  return td.m() == 1U;
}

bool ns::is_column_vector(const type_dimensions& td)
{
  assert(td.is_vector());
  return td.n() == 1U;
}

bool ns::is_prefix_operator(operator_unary_id id)
{
  return ((id == ns::id_increment_pre) || (id == ns::id_decrement_pre));
}

bool ns::is_postfix_operator(operator_unary_id id)
{
  return ((id == ns::id_increment_post) || (id == ns::id_decrement_post));
}

std::wstring ns::to_type_string(const type& t)
{
  std::wstringstream ss;

  const type_id id = t.get_id();
  const type_dimensions& dimensions = t.get_dimensions();

  if(dimensions.is_void())
  {
    assert(id == id_void);
    assert(dimensions.m() == 0U);
    assert(dimensions.n() == 0U);

    ss << L"void";
  }
  else if(dimensions.is_scalar())
  {
    switch(id)
    {
      case id_float:
        ss << L"float";
        break;
      case id_double:
        ss << L"double";
        break;
      case id_int:
        ss << L"int";
        break;
      case id_uint:
        ss << L"unsigned int";
        break;
      case id_bool:
        ss << L"bool";
        break;
      default:
        assert((id != id_unknown) && (id != id_void));
    }
  }
  else if(dimensions.is_vector())
  {
    switch(id)
    {
      case id_float:
        ss << L"vec";
        break;
      case id_double:
        ss << L"dvec";
        break;
      case id_int:
        ss << L"ivec";
        break;
      case id_uint:
        ss << L"uvec";
        break;
      case id_bool:
        ss << L"bvec";
        break;
      default:
        assert((id != id_unknown) && (id != id_void));
    }

    ss << (is_row_vector(dimensions) ? dimensions.n() : dimensions.m());
  }
  else if(dimensions.is_matrix())
  {
    switch(id)
    {
      case id_float:
        ss << L"mat";
        break;
      case id_double:
        ss << L"dmat";
        break;
      default:
        assert((id == id_float) || (id == id_double));
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

std::wstring ns::to_type_prefix_string(const type& t)
{
  // Variable names can't begin with a numeral so we need a type specific prefix
  std::wstring prefix_string;

  const language::type_id id = t.get_id();
  const language::type_dimensions& dimensions = t.get_dimensions();

  if(dimensions.is_scalar())
  {
    switch(id)
    {
      case id_float:
        prefix_string = L'f';
        break;
      case id_double:
        prefix_string = L'd';
        break;
      case id_int:
        prefix_string = L'i';
        break;
      case id_uint:
        prefix_string = L'u';
        break;
      case id_bool:
        prefix_string = L'b';
        break;
      default:
        assert((id != id_unknown) && (id != id_void));
    }
  }
  else if(dimensions.is_vector())
  {
    assert(id != id_void);
    assert(id != id_unknown);

    prefix_string = L'v';
  }
  else if(dimensions.is_matrix())
  {
    assert((id == id_float) ||
           (id == id_double));

    prefix_string = L'm';
  }
  else
  {
    assert(false);
  }

  return std::move(prefix_string);
}

const wchar_t* ns::to_operator_unary_string(operator_unary_id id)
{
  switch(id)
  {
  case id_increment_pre:
  case id_increment_post:
    return L"++";
   case id_decrement_pre:
   case id_decrement_post:
    return L"--";
  }

  return nullptr;
}

const wchar_t* ns::to_operator_binary_string(operator_binary_id id)
{
  switch(id)
  {
    case id_addition:
      return L"+";
    case id_subtraction:
      return L"-";
    case id_multiplication:
      return L"*";
    case id_division:
      return L"/";
    case id_assignment:
      return L"=";
    case id_assignment_addition:
      return L"+=";
    case id_assignment_subtraction:
      return L"-=";
    case id_assignment_multiplication:
      return L"*=";
    case id_assignment_division:
      return L"/=";
  }

  return nullptr;
}

const wchar_t* ns::to_conditional_string(conditional_id id)
{
  switch(id)
  {
  case id_if:
    return L"if";
  case id_else:
    return L"else";
  case id_else_if:
    return L"else if";
  }

  return nullptr;
}

const wchar_t* ns::to_component_string(type_dimension_t component_idx)
{
  switch(component_idx)
  {
  case 0:
    return L"x";
  case 1:
    return L"y";
  case 2:
    return L"z";
  case 3:
    return L"w";
  }

  return nullptr;
}

const wchar_t* ns::to_keyword_string(keyword_id id)
{
  switch(id)
  {
  case id_return:
    return L"return";
  }

  return nullptr;
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

const wchar_t* ns::to_intrinsic_string(sltl::core::intrinsic id)
{
  // _bi_* -> 'built-in' functions

  switch(id)
  {
  case sltl::core::intrinsic::dot:
    return L"_bi_dot";
  case sltl::core::intrinsic::normalize:
    return L"_bi_normalize";
  }

  return nullptr;
}
