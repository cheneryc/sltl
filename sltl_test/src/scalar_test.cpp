#include <gtest/gtest.h>

#include "scalar.h"
#include "shader.h"
#include "output.h"
#include "scope.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

TEST(scalar, constructor)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test_construct1;
    sltl::scalar<float> test_construct2(1.0f);
    sltl::scalar<float> test_construct3 = 1.0f;
    sltl::scalar<float> test_construct4 = sltl::scalar<float>(1.0f);// Temporary object eliminated by copy elision optimisation
    sltl::scalar<float> test_construct5 = test_construct4;
    sltl::scalar<float> test_construct6(test_construct5);
    sltl::scalar<float> test_construct7(sltl::scalar<float>(1.0f));// Temporary object eliminated by copy elision optimisation
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2(1.0f);
  float f3(1.0f);
  float f4(1.0f);
  float f5(f4);
  float f6(f5);
  float f7(1.0f);
}
)";

  ASSERT_EQ(expected, actual);
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
      sltl::scope block_scope;
      sltl::scalar<float> test_flt_scope;
    }
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
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

  ASSERT_EQ(expected, actual);
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
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2;
  f1 = 1.0f;
  f1 = f2;
  f1 = float();
  f1 = float(1.0f);
  f1 = (f2 = f1);
}
)";

  ASSERT_EQ(expected, actual);
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
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
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
  f1 += (f2 + 1.0f);
  f1 += (f2 + float());
  f1 += (f2 + f3);
  f1 += ((f2 + 1.0f) + f3);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(scalar, subtraction_operator)
{
  auto test_shader = []()
  {
    sltl::scalar<float> test1, test2, test3;

    test1 -= 1.0f;
    test1 -= test2;
    test1 -= sltl::scalar<float>();
    test1 -= sltl::scalar<float>(1.0f);

    test1 - 1.0f;
    test1 - test2;
    test1 - sltl::scalar<float>();

    1.0f - test1;
    sltl::scalar<float>() - test1;

    test1 -= test2 - 1.0f;
    test1 -= test2 - sltl::scalar<float>();
    test1 -= test2 - test3;
    test1 -= (test2 - 1.0f) - test3;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2;
  float f3;
  f1 -= 1.0f;
  f1 -= f2;
  f1 -= float();
  f1 -= float(1.0f);
  f1 - 1.0f;
  f1 - f2;
  f1 - float();
  1.0f - f1;
  float() - f1;
  f1 -= (f2 - 1.0f);
  f1 -= (f2 - float());
  f1 -= (f2 - f3);
  f1 -= ((f2 - 1.0f) - f3);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(scalar, prefix_operators)
{
  auto test_shader = []()
  {
    sltl::scalar<float> ftest1;
    sltl::scalar<float> ftest2 = ++ftest1;
    sltl::scalar<float> ftest3 = --ftest2;

    ++ftest3;
    ++(ftest1 + ftest2);
    --ftest3;
    --(ftest1 - ftest2);

    sltl::scalar<int> itest1;
    sltl::scalar<int> itest2 = ++itest1;
    sltl::scalar<int> itest3 = --itest2;

    ++itest3;
    ++(itest1 + itest2);
    --itest3;
    --(itest1 - itest2);
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2(++f1);
  float f3(--f2);
  ++f3;
  ++(f1 + f2);
  --f3;
  --(f1 - f2);
  int i4;
  int i5(++i4);
  int i6(--i5);
  ++i6;
  ++(i4 + i5);
  --i6;
  --(i4 - i5);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(scalar, postfix_operators)
{
  auto test_shader = []()
  {
    sltl::scalar<float> ftest1;
    sltl::scalar<float> ftest2 = ftest1++;
    sltl::scalar<float> ftest3 = ftest2--;

    ftest3++;
    (ftest1 + ftest2)++;
    ftest3--;
    (ftest1 - ftest2)--;

    sltl::scalar<int> itest1;
    sltl::scalar<int> itest2 = itest1++;
    sltl::scalar<int> itest3 = itest2--;

    itest3++;
    (itest1 + itest2)++;
    itest3--;
    (itest1 - itest2)--;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  float f1;
  float f2(f1++);
  float f3(f2--);
  f3++;
  (f1 + f2)++;
  f3--;
  (f1 - f2)--;
  int i4;
  int i5(i4++);
  int i6(i5--);
  i6++;
  (i4 + i5)++;
  i6--;
  (i4 - i5)--;
}
)";

  ASSERT_EQ(expected, actual);
}
