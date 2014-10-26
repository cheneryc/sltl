#pragma once


namespace sltl
{
namespace language
{
  enum type_id
  {
    id_unknown,
    id_float,
    id_float2,
    id_float3,
    id_float4,
    id_double,
    id_double2,
    id_double3,
    id_double4,
    id_int,
    id_int2,
    id_int3,
    id_int4,
    id_uint,
    id_uint2,
    id_uint3,
    id_uint4,
    id_bool,
    id_bool2,
    id_bool3,
    id_bool4
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
    id_else,
    id_else_if
  };

  const wchar_t* to_type_string(type_id id);
  const wchar_t* to_prefix_string(type_id id);
  const wchar_t* to_operator_string(operator_id id);
  const wchar_t* to_assignment_operator_string(assignment_operator_id id);
  const wchar_t* to_conditional_string(conditional_id id);
}
}
