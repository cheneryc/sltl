#include "shader.h"


namespace
{
  namespace ns = sltl::api;

  struct shader_factory
  {
    void operator()(ns::shader::variant_t& var, ns::detail::variant_tag<ns::gl::shader>, const char* shader_src, ns::shader_stage stage)
    {
      var.construct_type<ns::gl::shader>(shader_src, stage);
    }

    void operator()(ns::shader::variant_t& var, ns::detail::variant_tag<ns::dx::shader>, const char* shader_src, ns::shader_stage stage)
    {
      auto blob = ns::dx::shader::compile(shader_src, stage);
      var.construct_type<ns::dx::shader>(blob, stage);
    }
  };
}

ns::shader::shader(detail::variant_index var_idx, const char* shader_src, shader_stage stage) : _variant(var_idx)
{
  _variant.visit(::shader_factory(), shader_src, stage);
}

ns::shader::~shader()
{
  _variant.destruct();
}
