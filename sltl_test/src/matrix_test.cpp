#include <gtest/gtest.h>

#include "matrix.h"
#include "shader.h"
#include "output.h"
#include "scope.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(sltl::output_version::none, false);
  }
}

TEST(matrix, constructor)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 2, 2> test_construct1;
    sltl::matrix<float, 2, 3> test_construct2;
    sltl::matrix<float, 2, 4> test_construct3;
    sltl::matrix<float, 3, 2> test_construct4;
    sltl::matrix<float, 3, 3> test_construct5;
    sltl::matrix<float, 3, 4> test_construct6;
    sltl::matrix<float, 4, 2> test_construct7;
    sltl::matrix<float, 4, 3> test_construct8;
    sltl::matrix<float, 4, 4> test_construct9;

    sltl::matrix<float, 2> test_construct10(1.0f, 1.0f, 1.0f, 1.0f);

    //TODO: add constructor test cases equivalent to those in vector/scalar_test.cpp
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  mat2x2 m1;
  mat3x2 m2;
  mat4x2 m3;
  mat2x3 m4;
  mat3x3 m5;
  mat4x3 m6;
  mat2x4 m7;
  mat3x4 m8;
  mat4x4 m9;
  mat2x2 m10 = mat2x2(1.0f, 1.0f, 1.0f, 1.0f);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(matrix, variable_naming)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 4> test_matrix_flt;
    sltl::matrix<double, 4> test_matrix_dbl;

    {
      sltl::scope block_scope;
      sltl::matrix<float, 4> test_matrix_flt_scope;
    }
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  mat4x4 m1;
  dmat4x4 m2;
  {
    mat4x4 m3_1;
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(matrix, operator_multiplication)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 4, 3> m1;
    sltl::matrix<float, 3, 2> m2;
    sltl::matrix<float, 4, 2> m3 = m1 * m2;
    sltl::matrix<float, 2, 3> m4;
    sltl::matrix<float, 4, 3> m5 = (m1 * m2) * m4;
    sltl::matrix<float, 4, 2> m6 = (m1 * m2) * (m4 * m2);
    sltl::matrix<float, 4, 4> m7 = (m1 * m2) * sltl::matrix<float, 2, 4>();
    sltl::matrix<float, 2, 3> m8 = sltl::matrix<float, 2, 3>() * sltl::matrix<float, 3, 3>();
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  mat3x4 m1;
  mat2x3 m2;
  mat2x4 m3 = m1 * m2;
  mat3x2 m4;
  mat3x4 m5 = (m1 * m2) * m4;
  mat2x4 m6 = (m1 * m2) * (m4 * m2);
  mat4x4 m8 = (m1 * m2) * mat4x2(0.0f);
  mat3x2 m11 = mat3x2(0.0f) * mat3x3(0.0f);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(matrix, operator_multiplication_vector)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 4, 3> m1;
    sltl::vector<float, 4> v1;
    sltl::vector<float, 3> v2 = v1 * m1;
    sltl::vector<float, 3> v3 = sltl::vector<float, 4>() * m1;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  mat3x4 m1;
  vec4 v2;
  vec3 v3 = v2 * m1;
  vec3 v5 = vec4(0.0f) * m1;
}
)";

  ASSERT_EQ(expected, actual);
}
