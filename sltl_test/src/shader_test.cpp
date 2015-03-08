#include <gtest/gtest.h>

#include "shader.h"
#include "scalar.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

TEST(shader, empty)
{
  auto test_shader = []()
  {
    return 0;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(shader, empty_shader)
{
  auto test_shader = [](sltl::shader::tag<sltl::shader::test>)
  {
    return 0;
  };

  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void main()
{
}
)";

  ASSERT_EQ(expected, actual);
}