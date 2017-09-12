#include <gtest/gtest.h>

#include "vector.h"
#include "shader.h"
#include "output.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::output>(sltl::output_version::none, sltl::output_flags::flag_indent_space);
  }
}

TEST(intrinsic, dot_product)
{
  auto test_shader = []()
  {
    sltl::vector<float, 3> v1, v2;
    sltl::scalar<float> f1 = sltl::dot(v1, v2);
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec3 v1;
  vec3 v2;
  float f3 = dot(v1, v2);
}
)";

  ASSERT_EQ(expected, actual);
}
