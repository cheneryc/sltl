#pragma once

#include "gl/shader_gl.h"
#include "dx/shader_dx.h"

#include "shader_stage.h"

#include "detail/variant.h"


namespace sltl
{
namespace api
{
  class shader final
  {
  public:
    shader(detail::variant_index var_idx, const char* shader_src, shader_stage stage);

    ~shader();

    template<typename T>
    const T& as() const
    {
      static_assert(std::is_same<T, gl::shader>::value || std::is_same<T, dx::shader>::value, "sltl::api::shader::as: template parameter T must be sltl::api::gl::shader or sltl::api::dx::shader");

      return _variant.get_type<T>();
    }

    using variant_t = detail::variant<gl::shader, dx::shader>;

  private:
    variant_t _variant;
  };
}
}
