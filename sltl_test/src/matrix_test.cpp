#include <gtest/gtest.h>

#include "matrix.h"
#include "basic_operators.h"
#include "shader.h"
#include "output.h"
#include "scope.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader, sltl::detail::enum_flags<sltl::output_flags> flags = sltl::output_flags::flag_none)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::output>(sltl::output_version::none, flags | sltl::output_flags::flag_indent_space);
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
    sltl::matrix<float, 2> test_construct11(test_construct10);

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
  mat2x2 m11 = m10;
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

TEST(matrix, operator_multiplication_associativity)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 4, 4> m1, m2, m3;
    sltl::vector<float, 4> v1;
    sltl::vector<float, 4> v2 = v1 * m1 * m2 * m3;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  mat4x4 m1;
  mat4x4 m2;
  mat4x4 m3;
  vec4 v4;
  vec4 v5 = ((v4 * m1) * m2) * m3;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(matrix, output_matrix_order)
{
  auto test_shader = []()
  {
    sltl::matrix<float, 4, 3> m1;
    sltl::matrix<float, 3, 2> m2;
    sltl::vector<float, 2> v1 = sltl::vector<float, 4>() * m1 * m2;
  };

  sltl::shader shader = sltl::make_test(test_shader);

  // There is an issue with associativity when converting between row-major/vector
  // and column-major/vector as both C++ and glsl define the '*' operator as having
  // left-to-right associativity. Therefore, sltl code written in C++ like:
  //
  // m1 * m2 * m3
  //
  // is really:
  //
  // (m1 * m2) * m3
  //
  // Note that sltl will insert parentheses nodes into the syntax tree to make this
  // explicit. However, when converted to column-major it will be stored as:
  //
  // m3 * (m2 * m1)
  //
  // Which is different than if the following code were written in glsl:
  //
  // m3 * m2 * m1
  //
  // Which is really
  //
  // (m3 * m2) * m1
  //
  // So, while matrix multiplication is associative (i.e. the order of the parentheses
  // shouldn't matter) in practice there may be subtle differences due to floating-point
  // rounding issues.

  shader.apply_action<sltl::output_matrix_order>();

  const std::wstring actual = ::to_string(shader, sltl::output_flags::flag_transpose_type);
  const std::wstring expected = LR"(
{
  mat3x4 m1;
  mat2x3 m2;
  vec2 v4 = m2 * (m1 * vec4(0.0f));
}
)";

  ASSERT_EQ(expected, actual);
}
