#include <gtest/gtest.h>

#include "scalar.h"
#include "scope.h"
#include "shader.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

//TODO: should be in a shader_test.cpp file
TEST(scalar, empty)
{
  auto test_shader = []()
  {
    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_shader(sltl::shader::vertex, test_shader));
  const std::wstring expected = LR"(
{
}
)";

  ASSERT_EQ(result, expected);
}

TEST(scalar, constructor)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test_construct1;
    sltl::scalar<float> test_construct2(1.0f);
    sltl::scalar<float> test_construct3 = 1.0f;
    sltl::scalar<float> test_construct4 = sltl::scalar<float>(1.0f);
    sltl::scalar<float> test_construct5 = test_construct4;
    sltl::scalar<float> test_construct6(test_construct5);
    sltl::scalar<float> test_construct7(sltl::scalar<float>(1.0f));

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_shader(sltl::shader::vertex, test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2(1.0f);
  float f3(1.0f);
  float f5(float(1.0f));
  float f6(f5);
  float f7(f6);
  float f9(float(1.0f));
}
)";

  ASSERT_EQ(result, expected);
}

TEST(scalar, variable_naming)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test_flt;
    sltl::scalar<double> test_dbl;
    sltl::scalar<int> test_int;
    sltl::scalar<unsigned int> test_uint;
    sltl::scalar<bool> test_bool;

    {
      sltl::scope block_scope(sltl::scope::block);
      sltl::scalar<float> test_flt_scope;
    }

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_shader(sltl::shader::vertex, test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  double d2;
  int i3;
  unsigned int u4;
  bool b5;
  {
    float f6_1;
  }
}
)";

  ASSERT_EQ(result, expected);
}

TEST(scalar, assignment_operator)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test1, test2;

    test1 = 1.0f;
    test1 = test2;
    test1 = sltl::scalar<float>();
    test1 = sltl::scalar<float>(1.0f);
    test1 = test2 = test1;

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_shader(sltl::shader::vertex, test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2;
  f1 = 1.0f;
  f1 = f2;
  f1 = float();
  f1 = float(1.0f);
  f1 = f2 = f1;
}
)";

  ASSERT_EQ(result, expected);
}

TEST(scalar, addition_operator)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test1, test2, test3;

    test1 += 1.0f;
    test1 += test2;
    test1 += sltl::scalar<float>();
    test1 += sltl::scalar<float>(1.0f);

    test1 + 1.0f;
    test1 + test2;
    test1 + sltl::scalar<float>();

    1.0f + test1;
    sltl::scalar<float>() + test1;

    test1 += test2 + 1.0f;
    test1 += test2 + sltl::scalar<float>();
    test1 += test2 + test3;
    test1 += (test2 + 1.0f) + test3;

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_shader(sltl::shader::vertex, test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2;
  float f3;
  f1 += 1.0f;
  f1 += f2;
  f1 += float();
  f1 += float(1.0f);
  f1 + 1.0f;
  f1 + f2;
  f1 + float();
  1.0f + f1;
  float() + f1;
  f1 += f2 + 1.0f;
  f1 += f2 + float();
  f1 += f2 + f3;
  f1 += (f2 + 1.0f) + f3;
}
)";

  ASSERT_EQ(result, expected);
}
