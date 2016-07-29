#include <gtest/gtest.h>

#include "call.h"
#include "shader.h"
#include "output.h"
#include "scalar.h"
#include "vector.h"

#include "io/io.h"


namespace
{
  typedef sltl::io::block<> io_block_empty;

  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(sltl::output_version::none, false);
  }

  void fn_empty_returns_void() {}

  sltl::scalar<int> fn_empty_returns_int()
  {
    return 0;
  }

  sltl::vector<float, 3> fn_empty_returns_vector()
  {
    return sltl::vector<float, 3>(0.0f, 0.0f, 0.0f);
  }

  sltl::vector<float, 3> fn_empty_returns_vector_default()
  {
    return sltl::vector<float, 3>();
  }

  sltl::vector<float, 3> fn_simple_returns_vector_variable_reference()
  {
    sltl::vector<float, 3> v1(0.0f, 0.0f, 0.0f);
    v1 += sltl::vector<float, 3>(1.0f, 1.0f, 1.0f);
    return v1;
  }
}

TEST(call, call_fn_empty_returns_void)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::call(fn_empty_returns_void);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn1()
{
}
void main()
{
  fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_empty_returns_int)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::call(fn_empty_returns_int);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
int fn1()
{
  return 0;
}
void main()
{
  fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_empty_returns_int)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::scalar<int> test_assignment = sltl::call(fn_empty_returns_int);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
int fn1()
{
  return 0;
}
void main()
{
  int i1 = fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_empty_returns_vector)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::vector<float, 3> test_assignment = sltl::call(fn_empty_returns_vector);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
vec3 fn1()
{
  return vec3(0.0f, 0.0f, 0.0f);
}
void main()
{
  vec3 v1 = fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_empty_returns_vector_default)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::vector<float, 3> test_assignment = sltl::call(fn_empty_returns_vector_default);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
vec3 fn1()
{
  return vec3(0.0f);
}
void main()
{
  vec3 v1 = fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_simple_returns_vector_variable_reference)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::vector<float, 3> test_assignment = sltl::call(fn_simple_returns_vector_variable_reference);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
vec3 fn1()
{
  vec3 v1 = vec3(0.0f, 0.0f, 0.0f);
  v1 += vec3(1.0f, 1.0f, 1.0f);
  return v1;
}
void main()
{
  vec3 v1 = fn1();
}
)";

  ASSERT_EQ(expected, actual);
}
