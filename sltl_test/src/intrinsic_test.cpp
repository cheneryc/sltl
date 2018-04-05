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

TEST(intrinsic, call_intrinsic_dot)
{
  sltl::syntax::block blk(sltl::syntax::block::global);

  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block_guard bg(blk);

  sltl::syntax::intrinsic_manager_guard im;

  sltl::vector<float, 3> vec1, vec2;
  sltl::vector<float, 3>::proxy p1(vec1), p2(vec2);

  auto exp_return = sltl::syntax::call_intrinsic_dot(p1.move(), p2.move());

  ASSERT_TRUE(exp_return);
  ASSERT_TRUE(dynamic_cast<sltl::syntax::intrinsic_call*>(exp_return.get()));
}

TEST(intrinsic, call_intrinsic_dot_exceptions)
{
  sltl::syntax::block blk(sltl::syntax::block::global);

  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block_guard bg(blk);

  {
    sltl::vector<int, 3> vec1;
    sltl::vector<int, 3>::proxy p1(vec1);
    sltl::vector<float, 3> vec2;
    sltl::vector<float, 3>::proxy p2(vec2);

    // call_intrinsic_dot - only floating point types allowed for exp_x argument
    EXPECT_THROW(sltl::syntax::call_intrinsic_dot(p1.move(), p2.move()), std::exception);
  }

  {
    sltl::vector<float, 3> vec1;
    sltl::vector<float, 3>::proxy p1(vec1);
    sltl::vector<int, 3> vec2;
    sltl::vector<int, 3>::proxy p2(vec2);

    // call_intrinsic_dot - only floating point types allowed for exp_y argument
    EXPECT_THROW(sltl::syntax::call_intrinsic_dot(p1.move(), p2.move()), std::exception);
  }

  {
    sltl::matrix<float, 3> vec1;
    sltl::matrix<float, 3>::proxy p1(vec1);
    sltl::vector<float, 3> vec2;
    sltl::vector<float, 3>::proxy p2(vec2);

    // call_intrinsic_dot - only scalar or vector types allowed for exp_x argument
    EXPECT_THROW(sltl::syntax::call_intrinsic_dot(p1.move(), p2.move()), std::exception);
  }

  {
    sltl::vector<float, 3> vec1;
    sltl::vector<float, 3>::proxy p1(vec1);
    sltl::matrix<float, 3> vec2;
    sltl::matrix<float, 3>::proxy p2(vec2);

    // call_intrinsic_dot - only scalar or vector types allowed for exp_y argument
    EXPECT_THROW(sltl::syntax::call_intrinsic_dot(p1.move(), p2.move()), std::exception);
  }

  {
    sltl::vector<float, 3> vec1;
    sltl::vector<float, 3>::proxy p1(vec1);
    sltl::vector<float, 4> vec2;
    sltl::vector<float, 4>::proxy p2(vec2);

    // call_intrinsic_dot - type dimensions must match
    EXPECT_THROW(sltl::syntax::call_intrinsic_dot(p1.move(), p2.move()), std::exception);
  }
}
