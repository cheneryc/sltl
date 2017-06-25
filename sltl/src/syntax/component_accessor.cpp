#pragma once

#include "component_accessor.h"


namespace
{
  namespace ns = sltl::syntax;
}

ns::component_accessor::ptr ns::component_accessor::make_impl(tag<mode::scalar>, sltl::language::type_dimension_t count)
{
  return std::unique_ptr<component_accessor_scalar>(new component_accessor_scalar(count));
}

ns::component_accessor::ptr ns::component_accessor::make_impl(tag<mode::vector>,
                                                              sltl::language::type_dimension_t i0,
                                                              sltl::language::type_dimension_t i1,
                                                              sltl::language::type_dimension_t i2,
                                                              sltl::language::type_dimension_t i3)
{
  return std::unique_ptr<component_accessor_vector>(new component_accessor_vector(i0, i1, i2, i3));
}

ns::component_accessor::ptr ns::component_accessor::make_impl(tag<mode::matrix>,
                                                              sltl::language::type_dimension_t idx_m,
                                                              sltl::language::type_dimension_t idx_n)
{
  return std::unique_ptr<component_accessor_matrix>(new component_accessor_matrix(idx_m, idx_n));
}
