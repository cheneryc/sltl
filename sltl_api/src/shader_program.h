#pragma once

#include "shader.h"

#include "detail/variant.h"


namespace sltl
{
namespace api
{
  class shader_program final
  {
  public:
    shader_program(detail::variant_index var_idx, const shader& shader_vertex, const shader& shader_fragment);

    ~shader_program();

    using variant_t = detail::variant<gl::shader_program, dx::shader_program>;

  private:
    variant_t _variant;
  };
}
}
