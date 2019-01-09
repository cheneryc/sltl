#pragma once

namespace sltl
{
namespace api
{
  enum class shader_stage
  {
    vertex,
    fragment
  };

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

  class shader_program final
  {
  public:
    shader_program(const shader& shader_vertex, const shader& shader_fragment);

    ~shader_program();

  private:
    struct impl;
    impl* _impl;
  };

  class context final
  {
  public:
    context();

    ~context();

  private:
    struct impl;
    impl* _impl;
  };
}
}
