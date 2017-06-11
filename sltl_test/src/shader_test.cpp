#include <gtest/gtest.h>

#include "shader.h"
#include "output.h"

#include "io/io.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::output>(sltl::output_version::none, sltl::output_flags::flag_indent_space);
  }
}

TEST(shader, empty)
{
  auto test_shader = [](){};

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(shader, empty_shader)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, sltl::io::block<>){};

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}
