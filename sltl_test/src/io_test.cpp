#include <gtest/gtest.h>

#include "io/io.h"

#include "shader.h"
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
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::position>> io_block_in;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in input)
  {
    sltl::scalar<float> local = input.get<sltl::core::semantic::position>();
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
void main()
{
  float f1(i_f1);
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
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::position>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, sltl::io::block<>)
  {
    io_block_out output;
    output.get<sltl::core::semantic::position>() = 0.0f;
    return output;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
out float o_f1;
void main()
{
  o_f1 = 0.0f;
}
)";

  ASSERT_EQ(expected, actual);
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
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::position>> io_block_in;
  typedef sltl::io::block<sltl::io::variable<sltl::scalar<float>, sltl::core::semantic::position>> io_block_out;

  auto test_shader = [](sltl::shader::tag<sltl::shader::test>, io_block_in input)
  {
    io_block_out output;
    output.get<sltl::core::semantic::position>() = input.get<sltl::core::semantic::position>();
    return output;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
in float i_f1;
out float o_f1;
void main()
{
  o_f1 = i_f1;
}
)";

  ASSERT_EQ(expected, actual);
}
