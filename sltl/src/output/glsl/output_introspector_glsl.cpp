#include "output_introspector_glsl.h"

#include "glsl_convention.h"


namespace
{
  namespace ns = sltl::glsl;
}

ns::output_introspector_glsl::output_introspector_glsl(sltl::core::shader_stage stage, sltl::core::qualifier_storage qualifier, sltl::core::semantic_pair semantic) : output_introspector(stage, qualifier, semantic)
{
}

std::wstring ns::output_introspector_glsl::get_variable_name(const sltl::syntax::variable_declaration& vd) const
{
  return sltl::glsl::get_variable_name(vd);
}
