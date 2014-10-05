#pragma once


namespace sltl
{
namespace language
{
  enum type_id
  {
    id_unknown,
    id_float,
    id_double,
    id_int,
    id_uint,
    id_bool
  };

  enum operator_id
  {
    id_addition,
    id_subtraction,
    id_multiplication,
    id_division
  };

  enum assignment_operator_id
  {
    id_assignment,
    id_assignment_addition,
    id_assignment_subtraction,
    id_assignment_multiplication,
    id_assignment_division
  };

  enum conditional_id
  {
    id_if,
    id_else
  };

  const wchar_t* to_type_string(type_id id);
  const wchar_t* to_prefix_string(type_id id);
  const wchar_t* to_operator_string(operator_id id);
  const wchar_t* to_assignment_operator_string(assignment_operator_id id);
  const wchar_t* to_conditional_string(conditional_id id);
}
}
