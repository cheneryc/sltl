#include "shader.h"
#include "output.h"

#include "io/io.h"

#include "core/qualifier.h"
#include "core/shader_stage.h"

#include <iostream>


namespace sltl_glsl
{
  typedef sltl::vector<float, 2> vec2;
  typedef sltl::vector<float, 3> vec3;
  typedef sltl::vector<float, 4> vec4;

  typedef sltl::matrix<float, 2> mat2;
  typedef sltl::matrix<float, 3> mat3;
  typedef sltl::matrix<float, 4> mat4;

  template<typename ...T>
  using io_block =  sltl::io::block<T...>;

  template<typename T, sltl::core::semantic S, sltl::core::semantic_index_t N = 0U>
  using io_var = sltl::io::variable<T, S, N>;

  template<sltl::core::semantic_system S, sltl::core::semantic_index_t N = 0U>
  using io_var_sys = sltl::io::variable_system<S, N>;

  template<sltl::core::semantic_transform S>
  using io_var_trans = sltl::io::variable_transform<S>;

  typedef sltl::core::semantic semantic;
  typedef sltl::core::semantic_system semantic_sys;
  typedef sltl::core::semantic_transform semantic_trans;

  constexpr sltl::core::qualifier_storage tag_in = sltl::core::qualifier_storage::in;
  constexpr sltl::core::qualifier_storage tag_out = sltl::core::qualifier_storage::out;
  constexpr sltl::core::qualifier_storage tag_uniform = sltl::core::qualifier_storage::uniform;
}

namespace
{
  using namespace sltl_glsl;

  // --- In block ---

  typedef io_block<
    io_var<vec3, semantic::position>,
    io_var<vec3, semantic::normal>,
    io_var<vec2, semantic::texcoord>> vs_in_t;

  // --- Out block ---

  //1. position (gl_Position)
  //2. position (world space)
  //3. normal
  //4. texcoord

  typedef io_block<
    io_var_sys<semantic_sys::position>,
    io_var<vec3, semantic::position>,
    io_var<vec3, semantic::normal>,
    io_var<vec2, semantic::texcoord>> vs_fs_t;

  // --- Uniform block ---

  //1. mat4 model
  //2. mat4 view
  //3. mat4 projection
  //4. mat3 normal (modelviewproj_inversetrans)
  //5. vec3 camera

  typedef io_block<
    io_var_trans<semantic_trans::model>,
    io_var_trans<semantic_trans::view>,
    io_var_trans<semantic_trans::proj>,
    io_var_trans<semantic_trans::normal>,
    io_var<vec3, semantic::user>> uniform_t;

  namespace vs
  {
    using in_t  = vs_in_t;
    using out_t = vs_fs_t;

    out_t pbr_vs(sltl::shader_tag_vertex, in_t in)
    {
      uniform_t uniform(tag_uniform);

      vec4 pos_world = vec4(in.get<semantic::position>(), 1.0f) * uniform.get<semantic_trans::model>();
      vec4 pos_out = pos_world * uniform.get<semantic_trans::view>() * uniform.get<semantic_trans::proj>();

      out_t out(tag_out);

      out.get<semantic_sys::position>() = pos_out;
      out.get<semantic::position>() = pos_world.xyz;
      out.get<semantic::normal>() = in.get<semantic::normal>() * uniform.get<semantic_trans::normal>();
      out.get<semantic::texcoord>() = in.get<semantic::texcoord>();

      return out;
    }
  }

  typedef io_block<io_var_sys<semantic_sys::target>> fs_out_t;

  namespace fs
  {
    using in_t  = vs_fs_t;
    using out_t = fs_out_t;

    out_t pbr_fs(sltl::shader_tag_fragment, in_t in)
    {
      out_t out(tag_out);
      out.get<semantic_sys::target>() = vec4(1.0f, 1.0f, 1.0f, 1.0f);
      return out;
    }
  }
}

int main()
{
  auto shader_vs = sltl::make_shader(&vs::pbr_vs);

  std::wcout << L"--- Vertex Shader ---" << L"\n\n";
  std::wstring shader_vs_txt = shader_vs.apply_action<sltl::output>();
  std::wcout << shader_vs_txt.c_str() << L"\n\n";//TODO: make sltl::newline<2> to replace std::endl

  auto shader_fs = sltl::make_shader(&fs::pbr_fs);

  std::wcout << L"--- Fragment Shader ---" << L"\n\n";;
  std::wstring shader_fs_txt = shader_fs.apply_action<sltl::output>();
  std::wcout << shader_fs_txt.c_str() << std::endl;

  return 0;
}
