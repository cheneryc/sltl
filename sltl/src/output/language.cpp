#include "language.h"


namespace
{
  namespace ns = sltl;
}

const wchar_t* ns::to_operator_unary_string(language::operator_unary_id id)
{
  switch(id)
  {
  case language::id_increment_pre:
  case language::id_increment_post:
    return L"++";
   case language::id_decrement_pre:
   case language::id_decrement_post:
    return L"--";
  }

  return nullptr;
}

const wchar_t* ns::to_operator_binary_string(language::operator_binary_id id)
{
  switch(id)
  {
    case language::id_addition:
      return L"+";
    case language::id_subtraction:
      return L"-";
    case language::id_multiplication:
      return L"*";
    case language::id_division:
      return L"/";
    case language::id_element_wise_addition:
      return L"+";
    case language::id_element_wise_subtraction:
      return L"-";
    case language::id_element_wise_multiplication:
      return L"*";
    case language::id_element_wise_division:
      return L"/";
    case language::id_assignment:
      return L"=";
    case language::id_assignment_addition:
      return L"+=";
    case language::id_assignment_subtraction:
      return L"-=";
    case language::id_assignment_multiplication:
      return L"*=";
    case language::id_assignment_division:
      return L"/=";
    case language::id_lt:
      return L"<";
    case language::id_lt_eq:
      return L"<=";
    case language::id_gt:
      return L">";
    case language::id_gt_eq:
      return L">=";
    case language::id_matrix_multiplication:
      return L"*";
    case language::id_scalar_vector_multiplication:
      return L"*";
    case language::id_scalar_vector_division:
      return L"/";
    case language::id_scalar_matrix_multiplication:
      return L"*";
    case language::id_scalar_matrix_division:
      return L"/";
    case language::id_vector_scalar_multiplication:
      return L"*";
    case language::id_vector_scalar_division:
      return L"/";
    case language::id_matrix_scalar_multiplication:
      return L"*";
    case language::id_matrix_scalar_division:
      return L"/";
  }

  return nullptr;
}

const wchar_t* ns::to_conditional_string(language::conditional_id id)
{
  switch(id)
  {
  case language::id_if:
    return L"if";
  case language::id_else:
    return L"else";
  case language::id_else_if:
    return L"else if";
  }

  return nullptr;
}

const wchar_t* ns::to_component_string(language::type_dimension_t component_idx)
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

const wchar_t* ns::to_keyword_string(language::keyword_id id)
{
  switch(id)
  {
  case language::id_return:
    return L"return";
  }

  return nullptr;
}
