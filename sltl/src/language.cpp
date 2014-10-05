#include "language.h"

#include <cassert>


namespace
{
  namespace ns = sltl::language;
}

const wchar_t* ns::to_type_string(type_id id)
{
  switch(id)
  {
  case id_float:
    return L"float";
  case id_double:
    return L"double";
  case id_int:
    return L"int";
  case id_uint:
    return L"unsigned int";
  case id_bool:
    return L"bool";
  default:
    assert(id != id_unknown);
  }

  return nullptr;
}

const wchar_t* ns::to_prefix_string(type_id id)
{
  // Variable names can't begin with a numeral so we need a type specific prefix

  switch(id)
  {
  case id_float:
    return L"f";
  case id_double:
    return L"d";
  case id_int:
    return L"i";
  case id_uint:
    return L"u";
  case id_bool:
    return L"b";
  default:
    assert(id != id_unknown);
  }

  return nullptr;
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
  }

  return nullptr;
}
