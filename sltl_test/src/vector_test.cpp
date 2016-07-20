#include <gtest/gtest.h>

#include "vector.h"
#include "shader.h"
#include "output.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

TEST(vector, constructor)
{
  auto test_shader = []()
  {
    typedef sltl::vector<float, 2> vec2;
    typedef sltl::vector<float, 3> vec3;
    typedef sltl::vector<float, 4> vec4;

    vec2 test_construct1_2d;
    vec3 test_construct1_3d;
    vec4 test_construct1_4d;

    vec2 test_construct2_2d(1.0f, 1.0f);
    vec3 test_construct2_3d(1.0f, 1.0f, 1.0f);
    vec4 test_construct2_4d(1.0f, 1.0f, 1.0f, 1.0f);

    // Temporary objects eliminated by copy elision optimisation
    vec2 test_construct4_2d = vec2(1.0f, 1.0f);
    vec3 test_construct4_3d = vec3(1.0f, 1.0f, 1.0f);
    vec4 test_construct4_4d = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec2 test_construct5_2d = test_construct4_2d;
    vec3 test_construct5_3d = test_construct4_3d;
    vec4 test_construct5_4d = test_construct4_4d;

    vec2 test_construct6_2d(test_construct5_2d);
    vec3 test_construct6_3d(test_construct5_3d);
    vec4 test_construct6_4d(test_construct5_4d);

    // Temporary objects eliminated by copy elision optimisation
    vec2 test_construct7_2d(vec2(1.0f, 1.0f));
    vec3 test_construct7_3d(vec3(1.0f, 1.0f, 1.0f));
    vec4 test_construct7_4d(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // unpack tests
    vec2 v2;
    vec3 unpack1(1.0f, vec2(1.0f, 1.0f));
    vec3 unpack2(1.0f, v2);
    vec3 unpack3(vec2(1.0f, 1.0f), 1.0f);
    vec3 unpack4(v2, 1.0f);

    vec4 unpack5(1.0f, vec3(1.0f, 1.0f, 1.0f));
    vec4 unpack6(vec2(1.0f, 1.0f), vec2(1.0f, 1.0f));
    vec4 unpack7(v2, vec2(1.0f, 1.0f));
    vec4 unpack8(v2, v2);
    vec4 unpack9(v2 + v2, v2 + v2);
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec3 v2;
  vec4 v3;
  vec2 v4(1.0f, 1.0f);
  vec3 v5(1.0f, 1.0f, 1.0f);
  vec4 v6(1.0f, 1.0f, 1.0f, 1.0f);
  vec2 v7(1.0f, 1.0f);
  vec3 v8(1.0f, 1.0f, 1.0f);
  vec4 v9(1.0f, 1.0f, 1.0f, 1.0f);
  vec2 v10(v7);
  vec3 v11(v8);
  vec4 v12(v9);
  vec2 v13(v10);
  vec3 v14(v11);
  vec4 v15(v12);
  vec2 v16(1.0f, 1.0f);
  vec3 v17(1.0f, 1.0f, 1.0f);
  vec4 v18(1.0f, 1.0f, 1.0f, 1.0f);
  vec2 v19;
  vec3 v21(1.0f, vec2(1.0f, 1.0f));
  vec3 v22(1.0f, v19);
  vec3 v24(vec2(1.0f, 1.0f), 1.0f);
  vec3 v25(v19, 1.0f);
  vec4 v27(1.0f, vec3(1.0f, 1.0f, 1.0f));
  vec4 v30(vec2(1.0f, 1.0f), vec2(1.0f, 1.0f));
  vec4 v32(v19, vec2(1.0f, 1.0f));
  vec4 v33(v19, v19);
  vec4 v34(v19 + v19, v19 + v19);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, variable_naming)
{
  auto test_shader = []()
  {
    typedef sltl::vector<float, 2> vec2;
    typedef sltl::vector<float, 3> vec3;
    typedef sltl::vector<float, 4> vec4;

    vec2 test_vec2;
    vec3 test_vec3;
    vec4 test_vec4;

    typedef sltl::vector<double, 2> dvec2;
    typedef sltl::vector<double, 3> dvec3;
    typedef sltl::vector<double, 4> dvec4;

    dvec2 test_dvec2;
    dvec3 test_dvec3;
    dvec4 test_dvec4;

    typedef sltl::vector<int, 2> ivec2;
    typedef sltl::vector<int, 3> ivec3;
    typedef sltl::vector<int, 4> ivec4;

    ivec2 test_ivec2;
    ivec3 test_ivec3;
    ivec4 test_ivec4;

    typedef sltl::vector<unsigned int, 2> uvec2;
    typedef sltl::vector<unsigned int, 3> uvec3;
    typedef sltl::vector<unsigned int, 4> uvec4;

    uvec2 test_uvec2;
    uvec3 test_uvec3;
    uvec4 test_uvec4;

    typedef sltl::vector<bool, 2> bvec2;
    typedef sltl::vector<bool, 3> bvec3;
    typedef sltl::vector<bool, 4> bvec4;

    bvec2 test_bvec2;
    bvec3 test_bvec3;
    bvec4 test_bvec4;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec3 v2;
  vec4 v3;
  dvec2 v4;
  dvec3 v5;
  dvec4 v6;
  ivec2 v7;
  ivec3 v8;
  ivec4 v9;
  uvec2 v10;
  uvec3 v11;
  uvec4 v12;
  bvec2 v13;
  bvec3 v14;
  bvec4 v15;
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, assignment_operator)
{
  auto test_shader = []()
  {
    typedef sltl::vector<float, 2> vec2;

    vec2 test1, test2;

    test1 = test2;
    test1 = vec2();
    test1 = vec2(1.0f, 1.0f);
    test1 = test2 = test1;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec2 v2;
  v1 = v2;
  v1 = vec2();
  v1 = vec2(1.0f, 1.0f);
  v1 = (v2 = v1);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, addition_operator)
{
  auto test_shader = []()
  {
    typedef sltl::vector<float, 2> vec2;

    vec2 test1, test2, test3, test4;

    test1 += test2;
    test1 += vec2();
    test1 += vec2(1.0f, 1.0f);

    test1 + test2;
    test1 + vec2();

    vec2() + test1;

    test1 += test2 + vec2();
    test1 += test2 + test3;
    test1 += (test2 + test3) + test4;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec2 v2;
  vec2 v3;
  vec2 v4;
  v1 += v2;
  v1 += vec2();
  v1 += vec2(1.0f, 1.0f);
  v1 + v2;
  v1 + vec2();
  vec2() + v1;
  v1 += (v2 + vec2());
  v1 += (v2 + v3);
  v1 += ((v2 + v3) + v4);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, subtraction_operator)
{
  auto test_shader = []()
  {
    typedef sltl::vector<float, 2> vec2;

    vec2 test1, test2, test3, test4;

    test1 -= test2;
    test1 -= vec2();
    test1 -= vec2(1.0f, 1.0f);

    test1 - test2;
    test1 - vec2();

    vec2() - test1;

    test1 -= test2 - vec2();
    test1 -= test2 - test3;
    test1 -= (test2 - test3) - test4;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec2 v1;
  vec2 v2;
  vec2 v3;
  vec2 v4;
  v1 -= v2;
  v1 -= vec2();
  v1 -= vec2(1.0f, 1.0f);
  v1 - v2;
  v1 - vec2();
  vec2() - v1;
  v1 -= (v2 - vec2());
  v1 -= (v2 - v3);
  v1 -= ((v2 - v3) - v4);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, prefix_operators)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> ftest1;
    sltl::vector<float, 4> ftest2 = ++ftest1;
    sltl::vector<float, 4> ftest3 = --ftest2;

    ++ftest3;
    ++(ftest1 + ftest2);
    --ftest3;
    --(ftest1 - ftest2);

    sltl::vector<int, 4> itest1;
    sltl::vector<int, 4> itest2 = ++itest1;
    sltl::vector<int, 4> itest3 = --itest2;

    ++itest3;
    ++(itest1 + itest2);
    --itest3;
    --(itest1 - itest2);
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec4 v2(++v1);
  vec4 v3(--v2);
  ++v3;
  ++(v1 + v2);
  --v3;
  --(v1 - v2);
  ivec4 v4;
  ivec4 v5(++v4);
  ivec4 v6(--v5);
  ++v6;
  ++(v4 + v5);
  --v6;
  --(v4 - v5);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(vector, postfix_operators)
{
  auto test_shader = []()
  {
    sltl::vector<float, 4> ftest1;
    sltl::vector<float, 4> ftest2 = ftest1++;
    sltl::vector<float, 4> ftest3 = ftest2--;

    ftest3++;
    (ftest1 + ftest2)++;
    ftest3--;
    (ftest1 - ftest2)--;

    sltl::vector<int, 4> itest1;
    sltl::vector<int, 4> itest2 = itest1++;
    sltl::vector<int, 4> itest3 = itest2--;

    itest3++;
    (itest1 + itest2)++;
    itest3--;
    (itest1 - itest2)--;
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  vec4 v1;
  vec4 v2(v1++);
  vec4 v3(v2--);
  v3++;
  (v1 + v2)++;
  v3--;
  (v1 - v2)--;
  ivec4 v4;
  ivec4 v5(v4++);
  ivec4 v6(v5--);
  v6++;
  (v4 + v5)++;
  v6--;
  (v4 - v5)--;
}
)";

  ASSERT_EQ(expected, actual);
}
