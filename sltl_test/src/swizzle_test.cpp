#include <gtest/gtest.h>

#include "vector.h"
#include "shader.h"

#include "output/glsl/output_glsl.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::glsl::output_glsl>(sltl::glsl::output_version::none, sltl::output_flags::flag_indent_space);
  }
}

TEST(swizzle, scalar_conversion)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::scalar<float> f1 = v1.x;
    sltl::scalar<float> f2 = v1.y;
    sltl::scalar<float> f3 = v1.z;
    sltl::scalar<float> f4 = v1.w;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  float f2 = v1.x;
  float f3 = v1.y;
  float f4 = v1.z;
  float f5 = v1.w;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, scalar_conversion_rvalue)
{
  auto test_shader = []()
  {
    sltl::scalar<float> f1 = sltl::vector<float, 4>().x;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f2 = vec4(0.0f).x;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, scalar_assignment)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::scalar<float> f1, f2, f3, f4;

    f1 = v1.x;
    f2 = v1.y;
    f3 = v1.z;
    f4 = v1.w;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  float f2;
  float f3;
  float f4;
  float f5;
  f2 = v1.x;
  f3 = v1.y;
  f4 = v1.z;
  f5 = v1.w;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, scalar_assignment_rvalue)
{
  auto test_shader = []()
  {
    sltl::scalar<float> f1, f2, f3, f4;

    f1 = sltl::vector<float, 4>().x;
    f2 = sltl::vector<float, 4>().y;
    f3 = sltl::vector<float, 4>().z;
    f4 = sltl::vector<float, 4>().w;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2;
  float f3;
  float f4;
  f1 = vec4(0.0f).x;
  f2 = vec4(0.0f).y;
  f3 = vec4(0.0f).z;
  f4 = vec4(0.0f).w;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, scalar_permutation_assignment)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::vector<float, 4> v2;
    sltl::scalar<float> f1;

    v1.x = 1.0f;
    v1.y = f1;

    // exact type match: operator=(permutation&&)
    v1.x = sltl::vector<float, 4>().x;
    // exact type match: operator=(const permutation&)
    v1.x = v2.x;
    // implicit call to conversion operator: operator=(proxy_out_t&&)
    v1.y = v2.x;

    sltl::vector<float, 4>().x = f1;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec4 v2;
  float f3;
  v1.x = 1.0f;
  v1.y = f3;
  v1.x = vec4(0.0f).x;
  v1.x = v2.x;
  v1.y = v2.x;
  vec4(0.0f).x = f3;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, vector_conversion)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::vector<float, 2> v2 = v1.yy;
    sltl::vector<float, 3> v3 = v1.zzz;
    sltl::vector<float, 4> v4 = v1.wwww;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec2 v2 = v1.yy;
  vec3 v3 = v1.zzz;
  vec4 v4 = v1.wwww;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, vector_conversion_rvalue)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1 = sltl::vector<float, 4>().xxxx;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v2 = vec4(0.0f).xxxx;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, vector_assignment)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::vector<float, 2> v2;
    sltl::vector<float, 3> v3;
    sltl::vector<float, 4> v4;

    v2 = v1.yy;
    v3 = v1.zzz;
    v4 = v1.wwww;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec2 v2;
  vec3 v3;
  vec4 v4;
  v2 = v1.yy;
  v3 = v1.zzz;
  v4 = v1.wwww;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, vector_assignment_rvalue)
{
  auto test_shader = []()
  {
    sltl::vector<float, 2> v1;
    sltl::vector<float, 3> v2;
    sltl::vector<float, 4> v3;

    v1 = sltl::vector<float, 4>().yy;
    v2 = sltl::vector<float, 4>().zzz;
    v3 = sltl::vector<float, 4>().wwww;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec3 v2;
  vec4 v3;
  v1 = vec4(0.0f).yy;
  v2 = vec4(0.0f).zzz;
  v3 = vec4(0.0f).wwww;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(swizzle, vector_permutation_assignment)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> v1;
    sltl::vector<float, 4> v2;
    sltl::vector<float, 2> v3;

    v1.xx = v3;
    v1.xxxx = v2;

    // exact type match: operator=(permutation&&)
    v1.xx = sltl::vector<float, 4>().xx;
    // exact type match: operator=(const permutation&)
    v1.xx = v2.xx;
    // implicit call to conversion operator: operator=(proxy_out_t&&)
    v1.yy = v2.xx;

    sltl::vector<float, 4>().xxxx = v1;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec4 v2;
  vec2 v3;
  v1.xx = v3;
  v1.xxxx = v2;
  v1.xx = vec4(0.0f).xx;
  v1.xx = v2.xx;
  v1.yy = v2.xx;
  vec4(0.0f).xxxx = v1;
}
)";

  ASSERT_EQ(expected, actual);
}
