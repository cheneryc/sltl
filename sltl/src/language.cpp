#include "language.h"

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

bool ns::is_operator_symmetric(operator_binary_id id)
{
  bool is_symmetric = true;

  switch(id)
  {
    case id_matrix_multiplication:
    case id_scalar_vector_multiplication:
    case id_scalar_vector_division:
    case id_scalar_matrix_multiplication:
    case id_scalar_matrix_division:
    case id_vector_scalar_multiplication:
    case id_vector_scalar_division:
    case id_matrix_scalar_multiplication:
    case id_matrix_scalar_division:
      is_symmetric = false;
      break;
  }

  return is_symmetric;
}

bool ns::is_operator_asymmetric(operator_binary_id id)
{
  return !is_operator_symmetric(id);
}
