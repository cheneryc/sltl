#include "shader.h"

#include "call.h"
#include "if.h"
#include "element_wise.h"
#include "basic_operators.h"

#include "output/glsl/output_glsl.h"
#include "output/hlsl/output_hlsl.h"

#include "io/io.h"

#include "core/qualifier.h"
#include "core/shader_stage.h"

#include <iostream>


namespace sltl_glsl
{
  template<typename T>
  using scalar = sltl::scalar<T>;

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

  // --- In/Out block ---

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
  //6. vec3 light
  //7. vec3  material colour (rgb)
  //8. float material metallic factor
  //9. float material roughness factor

  typedef io_block<
    io_var_trans<semantic_trans::model>,
    io_var_trans<semantic_trans::view>,
    io_var_trans<semantic_trans::proj>,
    io_var_trans<semantic_trans::normal>,
    io_var<vec3, semantic::camera>,
    io_var<vec3, semantic::light, 0>,
    io_var<vec3, semantic::light, 1>,
    io_var<vec3, semantic::light, 2>,
    io_var<vec3, semantic::material, 0>,
    io_var<scalar<float>, semantic::material, 1>,
    io_var<scalar<float>, semantic::material, 2>> uniform_t;

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

  // --- Out block ---
  typedef io_block<io_var_sys<semantic_sys::target>> fs_out_t;

  namespace fs
  {
    using in_t  = vs_fs_t;
    using out_t = fs_out_t;

    // Normal Distribution function
    scalar<float> D_GGX(scalar<float> dot_NH, scalar<float> factor_roughness)
    {
      constexpr static const float PI = 3.14159265359f;

      scalar<float> alpha = factor_roughness * factor_roughness;
      scalar<float> alpha2 = alpha * alpha;
      scalar<float> denom = (dot_NH * dot_NH) * (alpha2 - 1.0f) + 1.0f;

      return (alpha2) / (PI * (denom * denom));
    }

    // Geometric Shadowing function
    scalar<float> G_SchlickSmithGGX(scalar<float> dot_NL, scalar<float> dot_NV, scalar<float> material_roughness)
    {
      scalar<float> r = (material_roughness + 1.0f);
      scalar<float> k = (r * r) / 8.0f;
      scalar<float> GL = dot_NL / (dot_NL * (1.0f - k) + k);
      scalar<float> GV = dot_NV / (dot_NV * (1.0f - k) + k);

      return GL * GV;
    }

    // Fresnel function
    vec3 F_Schlick(scalar<float> cos_theta, vec3 material_colour, scalar<float> material_metallic)
    {
      vec3 F0 = sltl::lerp(vec3(0.04f, 0.04f, 0.04f), material_colour, material_metallic);
      vec3 F = F0 + (vec3(1.0f, 1.0f, 1.0f) - F0) * sltl::pow(1.0f - cos_theta, 5.0f);

      return F;
    }

    vec3 BRDF(vec3 L, vec3 V, vec3 N, vec3 material_colour, scalar<float> material_metallic, scalar<float> material_roughness)
    {
      vec3 H = sltl::normalize(V + L);

      scalar<float> dot_NV = sltl::clamp(sltl::dot(N, V), 0.0f, 1.0f);
      scalar<float> dot_NL = sltl::clamp(sltl::dot(N, L), 0.0f, 1.0f);
      scalar<float> dot_LH = sltl::clamp(sltl::dot(L, H), 0.0f, 1.0f);
      scalar<float> dot_NH = sltl::clamp(sltl::dot(N, H), 0.0f, 1.0f);

      vec3 colour;
      vec3 colour_light(1.0f, 1.0f, 1.0f); //TODO: vec(1.0f) & mat(1.0f) constructors?

      sltl::if_then(dot_NL > 0.0f, [&]()
      {
        // Note: rroughness not used in sample PBR shader, could be a bug?
        // float rroughness = max(0.05, factor_roughness);

        scalar<float> D = sltl::call(&D_GGX, dot_NH, material_roughness);
        scalar<float> G = sltl::call(&G_SchlickSmithGGX, dot_NL, dot_NV, material_roughness);
        vec3 F = sltl::call(&F_Schlick, dot_NV, material_colour, material_metallic);

        vec3 specular = (D * F * G) / (4.0f * dot_NL * dot_NV);

        colour += sltl::element_wise(specular) * sltl::element_wise(colour_light) * dot_NL;
      });

      return colour;
    }

    out_t pbr_fs(sltl::shader_tag_fragment, in_t in)
    {
      uniform_t uniform(tag_uniform);

      vec3 N = sltl::normalize(in.get<semantic::normal>());
      vec3 V = sltl::normalize(uniform.get<semantic::camera>() - in.get<semantic::position>());

      vec3::proxy lights[] = {
        uniform.get<semantic::light, 0>(),
        uniform.get<semantic::light, 1>(),
        uniform.get<semantic::light, 2>()
      };

      vec3 Lo;

      //TODO: need sltl::for_each and sltl::array support to match the source pbr shader correctly
      for(auto& light : lights)
      {
        vec3 L = sltl::normalize(std::move(light) - in.get<semantic::position>());

        vec3::proxy          material_colour    = uniform.get<semantic::material, 0>();
        scalar<float>::proxy material_metallic  = uniform.get<semantic::material, 1>();
        scalar<float>::proxy material_roughness = uniform.get<semantic::material, 2>();

        Lo += sltl::call(&BRDF, L, V, N, std::move(material_colour), std::move(material_metallic), std::move(material_roughness));
      }

      out_t out(tag_out);
      out.get<semantic_sys::target>() = vec4(Lo, 1.0f);

      return out;
    }
  }
}

int main()
{
  constexpr bool is_glsl = true;

  std::wstring shader_vs_text;
  std::wstring shader_fs_text;

  //TODO: make sltl::newline<2> to replace "\n\n"

  std::wcout << L"--- Vertex Shader ---" << L"\n\n";

  {
    auto shader_vs = sltl::make_shader(&vs::pbr_vs);

    if(is_glsl)
    {
      shader_vs_text = shader_vs.apply_action<sltl::glsl::output_glsl>(sltl::glsl::output_version::v330, sltl::output_flags::flag_extra_newlines);
    }
    else
    {
      shader_vs_text = shader_vs.apply_action<sltl::hlsl::output_hlsl>(sltl::output_flags::flag_extra_newlines);
    }

    std::wcout << shader_vs_text.c_str() << L"\n\n";
  }

  std::wcout << L"--- Fragment Shader ---" << L"\n\n";

  {
    auto shader_fs = sltl::make_shader(&fs::pbr_fs);

    if(is_glsl)
    {
      shader_fs_text = shader_fs.apply_action<sltl::glsl::output_glsl>(sltl::glsl::output_version::v330, sltl::output_flags::flag_extra_newlines);
    }
    else
    {
      shader_fs_text = shader_fs.apply_action<sltl::hlsl::output_hlsl>(sltl::output_flags::flag_extra_newlines);
    }

    std::wcout << shader_fs_text.c_str() << std::endl;
  }

  return 0;
}
