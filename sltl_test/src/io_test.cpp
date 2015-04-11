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
in float in_f1;
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
in float in_f1;
in double in_d2;
in int in_i3;
in unsigned int in_u4;
in bool in_b5;
in vec2 in_v6;
in vec3 in_v7;
in vec4 in_v8;
void main()
{
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
out float out_f1;
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
out float out_f1;
out double out_d2;
out int out_i3;
out unsigned int out_u4;
out bool out_b5;
out vec2 out_v6;
out vec3 out_v7;
out vec4 out_v8;
void main()
{
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
in float in_f1;
out float out_f1;
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
in float in_f1;
in double in_d2;
in int in_i3;
in unsigned int in_u4;
in bool in_b5;
in vec2 in_v6;
in vec3 in_v7;
in vec4 in_v8;
out float out_f1;
out double out_d2;
out int out_i3;
out unsigned int out_u4;
out bool out_b5;
out vec2 out_v6;
out vec3 out_v7;
out vec4 out_v8;
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}
