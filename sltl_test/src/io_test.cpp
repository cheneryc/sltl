#include <gtest/gtest.h>

#include "io/io.h"

#include "shader.h"
#include "output.h"
#include "scalar.h"
#include "vector.h"
#include "matrix.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::output>(sltl::output_version::none, sltl::output_flags::flag_indent_space);
  }

  std::wstring to_string(const sltl::shader& shader, sltl::detail::enum_flags<sltl::output::layout_flags> layout_flags)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::output>(sltl::output::layout_manager(layout_flags), sltl::output_version::none, sltl::output_flags::flag_indent_space);
  }
}

TEST(io, in_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in)
  {
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, in_many)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::scalar<double>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::scalar<int>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::scalar<unsigned int>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::scalar<bool>, sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 6U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 7U>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in)
  {
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
in double i_d2;
in int i_i3;
in unsigned int i_u4;
in bool i_b5;
in vec2 i_v6;
in vec3 i_v7;
in vec4 i_v8;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, in_ref_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
    sltl::vector<float, 2> local = input.get<sltl::core::semantic::texcoord>();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in vec2 i_v1;
void main()
{
  vec2 v1 = i_v1;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, out_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    return io_block_out();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
out float o_f1;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, out_many)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::scalar<double>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::scalar<int>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::scalar<unsigned int>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::scalar<bool>, sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 6U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 7U>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    return io_block_out();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
out float o_f1;
out double o_d2;
out int o_i3;
out unsigned int o_u4;
out bool o_b5;
out vec2 o_v6;
out vec3 o_v7;
out vec4 o_v8;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, out_ref_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_out output;
    output.get<sltl::core::semantic::texcoord>() = sltl::vector<float, 2>(0.0f, 0.0f);
    return output;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
out vec2 o_v1;
void main()
{
  o_v1 = vec2(0.0f, 0.0f);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, uniform_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
uniform float u_f1;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, uniform_many)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::scalar<double>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::scalar<int>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::scalar<unsigned int>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::scalar<bool>, sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 6U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 7U>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
uniform float u_f1;
uniform double u_d2;
uniform int u_i3;
uniform unsigned int u_u4;
uniform bool u_b5;
uniform vec2 u_v6;
uniform vec3 u_v7;
uniform vec4 u_v8;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, uniform_ref_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
    sltl::vector<float, 2> local = uniform.get<sltl::core::semantic::texcoord>();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
uniform vec2 u_v1;
void main()
{
  vec2 v1 = u_v1;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, uniform_introspection)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord, 0>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord, 1>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
  };

  auto shader = sltl::make_shader(test_shader);

  const std::wstring actual_texcoord0 = shader.apply_action<sltl::output_introspector, false>(sltl::core::qualifier_storage::uniform, sltl::core::semantic_pair(sltl::core::semantic::texcoord, 0));
  const std::wstring expected_texcoord0 = L"u_v1";

  ASSERT_EQ(expected_texcoord0, actual_texcoord0);

  const std::wstring actual_texcoord1 = shader.apply_action<sltl::output_introspector, false>(sltl::core::qualifier_storage::uniform, sltl::core::semantic_pair(sltl::core::semantic::texcoord, 1));
  const std::wstring expected_texcoord1 = L"u_v2";

  ASSERT_EQ(expected_texcoord1, actual_texcoord1);
}

TEST(io, inout_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user>> io_block_in;
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in)
  {
    return io_block_out();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
out float o_f1;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, inout_many)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::scalar<double>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::scalar<int>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::scalar<unsigned int>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::scalar<bool>, sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 6U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 7U>> io_block_in;

  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::scalar<double>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::scalar<int>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::scalar<unsigned int>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::scalar<bool>, sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 6U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 7U>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in)
  {
    return io_block_out();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
in double i_d2;
in int i_i3;
in unsigned int i_u4;
in bool i_b5;
in vec2 i_v6;
in vec3 i_v7;
in vec4 i_v8;
out float o_f1;
out double o_d2;
out int o_i3;
out unsigned int o_u4;
out bool o_b5;
out vec2 o_v6;
out vec3 o_v7;
out vec4 o_v8;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, inout_ref_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord>> io_block_in;
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
    io_block_out output;
    output.get<sltl::core::semantic::texcoord>() = input.get<sltl::core::semantic::texcoord>();
    return output;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in vec2 i_v1;
out vec2 o_v1;
void main()
{
  o_v1 = i_v1;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, semantic_built_in)
{
  typedef sltl::io::block<
    sltl::io::variable_system<sltl::core::semantic_system::position>,
    sltl::io::variable_system<sltl::core::semantic_system::depth>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>)
  {
    io_block_out output;
    output.get<sltl::core::semantic_system::position>() = sltl::vector<float, 4>(0.0f, 0.0f, 0.0f, 0.0f);
    output.get<sltl::core::semantic_system::depth>() = 0.0f;
    return output;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void main()
{
  gl_Position = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  gl_FragDepth = 0.0f;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, layout_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user>> io_block_in;
  typedef sltl::io::block<sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
    return io_block_out();
  };

  const auto flags = sltl::output::layout_flags::flag_in |
                     sltl::output::layout_flags::flag_out;

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader), flags);
  const std::wstring expected = LR"(
layout(location = 0) in vec4 i_v1;
layout(location = 0) out vec4 o_v1;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, layout_many)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>,    sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 3U>> io_block_in;

  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>,    sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::vector<float, 3>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::user, 3U>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
    return io_block_out();
  };

  const auto flags = sltl::output::layout_flags::flag_in |
                     sltl::output::layout_flags::flag_out;

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader), flags);
  const std::wstring expected = LR"(
layout(location = 0) in float i_f1;
layout(location = 1) in vec2 i_v2;
layout(location = 2) in vec3 i_v3;
layout(location = 3) in vec4 i_v4;
layout(location = 0) out float o_f1;
layout(location = 1) out vec2 o_v2;
layout(location = 2) out vec3 o_v3;
layout(location = 3) out vec4 o_v4;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, layout_many_double)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<double>,    sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::vector<double, 2>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::vector<double, 3>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::vector<double, 4>, sltl::core::semantic::user, 3U>> io_block_in;

  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<double>,    sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::vector<double, 2>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::vector<double, 3>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::vector<double, 4>, sltl::core::semantic::user, 3U>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
    return io_block_out();
  };

  const auto flags = sltl::output::layout_flags::flag_in |
                     sltl::output::layout_flags::flag_out;

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader), flags);
  const std::wstring expected = LR"(
layout(location = 0) in double i_d1;
layout(location = 1) in dvec2 i_v2;
layout(location = 2) in dvec3 i_v3;
layout(location = 4) in dvec4 i_v4;
layout(location = 0) out double o_d1;
layout(location = 1) out dvec2 o_v2;
layout(location = 2) out dvec3 o_v3;
layout(location = 4) out dvec4 o_v4;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, layout_matrix)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::matrix<float, 2, 2>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::matrix<float, 2, 3>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::matrix<float, 2, 4>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::matrix<float, 3, 2>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::matrix<float, 3>,    sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::matrix<float, 4, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::matrix<float, 4>,    sltl::core::semantic::user, 6U>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader), sltl::output::layout_flags::flag_in);
  const std::wstring expected = LR"(
layout(location = 0) in mat2x2 i_m1;
layout(location = 2) in mat3x2 i_m2;
layout(location = 5) in mat4x2 i_m3;
layout(location = 9) in mat2x3 i_m4;
layout(location = 11) in mat3x3 i_m5;
layout(location = 14) in mat2x4 i_m6;
layout(location = 16) in mat4x4 i_m7;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, layout_matrix_double)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::matrix<double, 2, 2>, sltl::core::semantic::user, 0U>,
    sltl::io::variable<sltl::matrix<double, 2, 3>, sltl::core::semantic::user, 1U>,
    sltl::io::variable<sltl::matrix<double, 2, 4>, sltl::core::semantic::user, 2U>,
    sltl::io::variable<sltl::matrix<double, 3, 2>, sltl::core::semantic::user, 3U>,
    sltl::io::variable<sltl::matrix<double, 3>,    sltl::core::semantic::user, 4U>,
    sltl::io::variable<sltl::matrix<double, 4, 2>, sltl::core::semantic::user, 5U>,
    sltl::io::variable<sltl::matrix<double, 4>,    sltl::core::semantic::user, 6U>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_in input)
  {
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader), sltl::output::layout_flags::flag_in);
  const std::wstring expected = LR"(
layout(location = 0) in dmat2x2 i_m1;
layout(location = 2) in dmat3x2 i_m2;
layout(location = 5) in dmat4x2 i_m3;
layout(location = 9) in dmat2x3 i_m4;
layout(location = 13) in dmat3x3 i_m5;
layout(location = 19) in dmat2x4 i_m6;
layout(location = 23) in dmat4x4 i_m7;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}

//TODO: add tests for uniform variables

//TODO: add tests for 'skipping' locations using semantic::none
