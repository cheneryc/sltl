#pragma once

#include "../shader_stage.h"


namespace sltl
{
namespace api
{
namespace gl
{
  class shader final
  {
  public:
    shader(const char* shader_src, shader_stage stage);

    ~shader();

  private:
    friend class shader_program;

    struct impl;
    impl* _impl;
  };

  //TODO: move shader_program class into its own .h/cpp (requires removal of the pimpl idiom)
  class shader_program final
  {
  public:
    shader_program(const shader& shader_vertex, const shader& shader_fragment);

    ~shader_program();

  private:
    struct impl;
    impl* _impl;
  };
}
}
}