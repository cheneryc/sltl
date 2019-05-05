#include "shader_program.h"


namespace
{
  namespace ns = sltl::api;

  struct shader_program_factory
  {
    void operator()(ns::shader_program::variant_t& var, ns::detail::variant_tag<ns::gl::shader_program>, const ns::shader& shader_vertex, const ns::shader& shader_fragment)
    {
      var.construct_type<ns::gl::shader_program>(shader_vertex.as<ns::gl::shader>(), shader_fragment.as<ns::gl::shader>());
    }

    void operator()(ns::shader_program::variant_t& var, ns::detail::variant_tag<ns::dx::shader_program>, const ns::shader& shader_vertex, const ns::shader& shader_fragment)
    {
      var.construct_type<ns::dx::shader_program>(shader_vertex.as<ns::dx::shader>(), shader_fragment.as<ns::dx::shader>());
    }
  };
}

ns::shader_program::shader_program(detail::variant_index var_idx, const shader& shader_vertex, const shader& shader_fragment) : _variant(var_idx)
{
  _variant.visit(::shader_program_factory(), shader_vertex, shader_fragment);
}

ns::shader_program::~shader_program()
{
  _variant.destruct();
}
