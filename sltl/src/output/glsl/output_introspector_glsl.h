#pragma once

#include <output/output_introspector.h>

namespace sltl
{
namespace glsl
{
  class output_introspector_glsl : public output_introspector
  {
  public:
    output_introspector_glsl(output_introspector_glsl&&) = default;
    output_introspector_glsl(core::shader_stage stage, core::qualifier_storage qualifier, core::semantic_pair semantic);

    // Non-copyable and non-assignable
    output_introspector_glsl(const output_introspector_glsl&) = delete;
    output_introspector_glsl& operator=(output_introspector_glsl&&) = delete;
    output_introspector_glsl& operator=(const output_introspector_glsl&) = delete;

  protected:
    std::wstring get_variable_name(const syntax::variable_declaration& vd) const override;
  };
}
}
