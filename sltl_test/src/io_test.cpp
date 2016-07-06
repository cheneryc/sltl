#include <gtest/gtest.h>

#include "io/io.h"

#include "shader.h"
#include "output.h"
#include "scalar.h"
#include "vector.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

TEST(io, in_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in)
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
    sltl::io::variable<sltl::scalar<float>>,
    sltl::io::variable<sltl::scalar<double>>,
    sltl::io::variable<sltl::scalar<int>>,
    sltl::io::variable<sltl::scalar<unsigned int>>,
    sltl::io::variable<sltl::scalar<bool>>,
    sltl::io::variable<sltl::vector<float, 2>>,
    sltl::io::variable<sltl::vector<float, 3>>,
    sltl::io::variable<sltl::vector<float, 4>>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in)
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

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in input)
  {
    sltl::vector<float, 2> local = input.get<sltl::core::semantic::texcoord>();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in vec2 i_v1;
void main()
{
  vec2 v1(i_v1);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, out_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
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
    sltl::io::variable<sltl::scalar<float>>,
    sltl::io::variable<sltl::scalar<double>>,
    sltl::io::variable<sltl::scalar<int>>,
    sltl::io::variable<sltl::scalar<unsigned int>>,
    sltl::io::variable<sltl::scalar<bool>>,
    sltl::io::variable<sltl::vector<float, 2>>,
    sltl::io::variable<sltl::vector<float, 3>>,
    sltl::io::variable<sltl::vector<float, 4>>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
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

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
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
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
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
    sltl::io::variable<sltl::scalar<float>>,
    sltl::io::variable<sltl::scalar<double>>,
    sltl::io::variable<sltl::scalar<int>>,
    sltl::io::variable<sltl::scalar<unsigned int>>,
    sltl::io::variable<sltl::scalar<bool>>,
    sltl::io::variable<sltl::vector<float, 2>>,
    sltl::io::variable<sltl::vector<float, 3>>,
    sltl::io::variable<sltl::vector<float, 4>>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
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

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
    sltl::vector<float, 2> local = uniform.get<sltl::core::semantic::texcoord>();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
uniform vec2 u_v1;
void main()
{
  vec2 v1(u_v1);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(io, uniform_introspection)
{
  typedef sltl::io::block<
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord, 0>,
    sltl::io::variable<sltl::vector<float, 2>, sltl::core::semantic::texcoord, 1>> io_block_uniform;

  auto test_shader = [](sltl::shader::tag<sltl::shader::fragment>, sltl::io::block<>)
  {
    io_block_uniform uniform(sltl::core::qualifier_storage::uniform);
  };

  auto shader = sltl::make_shader(test_shader);

  const std::wstring actual_texcoord0 = shader.str<sltl::output_introspector, false>(sltl::core::qualifier_storage::uniform, sltl::core::semantic_pair(sltl::core::semantic::texcoord, 0));
  const std::wstring expected_texcoord0 = L"u_v1";

  ASSERT_EQ(expected_texcoord0, actual_texcoord0);

  const std::wstring actual_texcoord1 = shader.str<sltl::output_introspector, false>(sltl::core::qualifier_storage::uniform, sltl::core::semantic_pair(sltl::core::semantic::texcoord, 1));
  const std::wstring expected_texcoord1 = L"u_v2";

  ASSERT_EQ(expected_texcoord1, actual_texcoord1);
}

TEST(io, inout_single)
{
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>>> io_block_in;
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in)
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
    sltl::io::variable<sltl::scalar<float>>,
    sltl::io::variable<sltl::scalar<double>>,
    sltl::io::variable<sltl::scalar<int>>,
    sltl::io::variable<sltl::scalar<unsigned int>>,
    sltl::io::variable<sltl::scalar<bool>>,
    sltl::io::variable<sltl::vector<float, 2>>,
    sltl::io::variable<sltl::vector<float, 3>>,
    sltl::io::variable<sltl::vector<float, 4>>> io_block_in;

  typedef sltl::io::block<
    sltl::io::variable<sltl::scalar<float>>,
    sltl::io::variable<sltl::scalar<double>>,
    sltl::io::variable<sltl::scalar<int>>,
    sltl::io::variable<sltl::scalar<unsigned int>>,
    sltl::io::variable<sltl::scalar<bool>>,
    sltl::io::variable<sltl::vector<float, 2>>,
    sltl::io::variable<sltl::vector<float, 3>>,
    sltl::io::variable<sltl::vector<float, 4>>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in)
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

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in input)
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
    sltl::io::variable<sltl::vector<float, 4>, sltl::core::semantic::position>,
    sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::depth>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
  {
    io_block_out output;
    output.get<sltl::core::semantic::position>() = sltl::vector<float, 4>(0.0f, 0.0f, 0.0f, 0.0f);
    output.get<sltl::core::semantic::depth>() = 0.0f;
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
