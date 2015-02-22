#include "language.h"

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::language;
}

std::wstring ns::to_type_string(const type& t)
{
  std::wstring type_string;

  if(t._components > 1)
  {
    switch(t._id)
    {
    case id_float:
      type_string = L"vec";
      break;
    case id_double:
      type_string = L"dvec";
      break;
    case id_int:
      type_string = L"ivec";
      break;
    case id_uint:
      type_string = L"uvec";
      break;
    case id_bool:
      type_string = L"bvec";
      break;
    default:
      assert((t._id != id_unknown) && (t._id != id_void));
    }

    type_string += std::to_wstring(t._components);
  }
  else
  {
    assert(t._components != 0);

    switch(t._id)
    {
    case id_void:
      type_string = L"void";
      break;
    case id_float:
      type_string = L"float";
      break;
    case id_double:
      type_string = L"double";
      break;
    case id_int:
      type_string = L"int";
      break;
    case id_uint:
      type_string = L"unsigned int";
      break;
    case id_bool:
      type_string = L"bool";
      break;
    default:
      assert(t._id != id_unknown);
    }
  }

  return std::move(type_string);
}

std::wstring ns::to_prefix_string(const type& t)
{
  // Variable names can't begin with a numeral so we need a type specific prefix
  std::wstring prefix_string;

  if(t._components > 1)
  {
    assert(t._id != id_void);
    assert(t._id != id_unknown);

    prefix_string = L'v';
  }
  else
  {
    assert(t._components != 0);

    switch(t._id)
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
      assert((t._id != id_unknown) && (t._id != id_void));
    }
  }

  return std::move(prefix_string);
}

const wchar_t* ns::to_operator_string(operator_id id)
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
  }

  return nullptr;
}

const wchar_t* ns::to_assignment_operator_string(assignment_operator_id id)
{
  switch(id)
  {
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

const wchar_t* ns::to_keyword_string(keyword_id id)
{
  switch(id)
  {
  case id_return:
    return L"return";
  }

  return nullptr;
}
