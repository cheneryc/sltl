#include <gtest/gtest.h>

#include "call.h"
#include "shader.h"
#include "scalar.h"
#include "vector.h"
#include "basic_operators.h"

#include "output/glsl/output_glsl.h"

#include "io/io.h"


namespace
{
  typedef sltl::io::block<> io_block_empty;

  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.apply_action<sltl::glsl::output_glsl>(sltl::glsl::output_version::none, sltl::output_flags::flag_indent_space);
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

  sltl::matrix<float, 3> fn_one_param(sltl::matrix<float, 3> m)
  {
    return m;
  }

  void fn_one_param_return_void(sltl::matrix<float, 3> m)
  {
    sltl::matrix<float, 3> m_local = m;
  }

  sltl::matrix<float, 3> fn_two_params(sltl::matrix<float, 3> m1, sltl::matrix<float, 3> m2)
  {
    return m1 * m2;
  }

  void fn_two_params_return_void(sltl::matrix<float, 3> m1, sltl::matrix<float, 3> m2)
  {
    sltl::matrix<float, 3> m_local = m1 * m2;
  }

  sltl::matrix<float, 3> fn_three_params(sltl::matrix<float, 3> m1, sltl::matrix<float, 3> m2, sltl::matrix<float, 3> m3)
  {
    return m1 * m2 * m3;
  }

  void fn_three_params_return_void(sltl::matrix<float, 3> m1, sltl::matrix<float, 3> m2, sltl::matrix<float, 3> m3)
  {
    sltl::matrix<float, 3> m_local = m1 * m2 * m3;
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

TEST(call, call_with_assignment_fn_one_param)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1;
    sltl::matrix<float, 3> m2 = sltl::call(fn_one_param, m1);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
mat3x3 fn1(mat3x3 p_m1)
{
  return p_m1;
}
void main()
{
  mat3x3 m1;
  mat3x3 m2 = fn1(m1);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_one_param_return_void)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1;
    sltl::call(fn_one_param_return_void, m1);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn1(mat3x3 p_m1)
{
  mat3x3 m1 = p_m1;
}
void main()
{
  mat3x3 m1;
  fn1(m1);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_two_params)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1, m2;
    sltl::matrix<float, 3> m3 = sltl::call(fn_two_params, m1, m2);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
mat3x3 fn1(mat3x3 p_m1, mat3x3 p_m2)
{
  return p_m1 * p_m2;
}
void main()
{
  mat3x3 m1;
  mat3x3 m2;
  mat3x3 m3 = fn1(m1, m2);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_two_params_return_void)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1, m2;
    sltl::call(fn_two_params_return_void, m1, m2);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn1(mat3x3 p_m1, mat3x3 p_m2)
{
  mat3x3 m1 = p_m1 * p_m2;
}
void main()
{
  mat3x3 m1;
  mat3x3 m2;
  fn1(m1, m2);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_with_assignment_fn_three_params)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1, m2, m3;
    sltl::matrix<float, 3> m4 = sltl::call(fn_three_params, m1, m2, m3);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
mat3x3 fn1(mat3x3 p_m1, mat3x3 p_m2, mat3x3 p_m3)
{
  return (p_m1 * p_m2) * p_m3;
}
void main()
{
  mat3x3 m1;
  mat3x3 m2;
  mat3x3 m3;
  mat3x3 m4 = fn1(m1, m2, m3);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_three_params_return_void)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    sltl::matrix<float, 3> m1, m2, m3;
    sltl::call(fn_three_params_return_void, m1, m2, m3);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn1(mat3x3 p_m1, mat3x3 p_m2, mat3x3 p_m3)
{
  mat3x3 m1 = (p_m1 * p_m2) * p_m3;
}
void main()
{
  mat3x3 m1;
  mat3x3 m2;
  mat3x3 m3;
  fn1(m1, m2, m3);
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_multiple_pointer)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    // Repeated calls to sltl::call with the same function pointer result in a single function definition being generated
    sltl::call(fn_empty_returns_void);
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
  fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_multiple_lambda)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    // non-empty capture list is necessary to avoid conversion of the lambda to a function pointer
    auto fn = [value = int()]{};

    // Repeated calls to sltl::call with a non equality comparable callable type (e.g. lambda) result in multiple function definitions being generated
    sltl::call(fn);
    sltl::call(fn);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn2()
{
}
void fn1()
{
}
void main()
{
  fn1();
  fn2();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_multiple_functor)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty) -> void
  {
    struct fn_type
    {
      void operator()() const
      {
        sltl::scalar<int> i = value;
      }

      bool operator==(const fn_type& other) const
      {
        return value == other.value;
      }

      int value;
    };

    fn_type fn1 = { 1 };
    fn_type fn2 = { 2 };
    fn_type fn3 = { 1 };

    // Repeated calls to sltl::call with an equality comparable callable type (e.g. functor) only
    // result in new function definitions being generated when the equality operator returns false
    sltl::call(fn1);
    sltl::call(fn2);
    sltl::call(fn3);
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn2()
{
  int i1 = 2;
}
void fn1()
{
  int i1 = 1;
}
void main()
{
  fn1();
  fn2();
  fn1();
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(call, call_fn_arg_wrong_type)
{
  sltl::syntax::block blk(sltl::syntax::block::global);

  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block_guard bg(blk);

  sltl::vector<float, 3> v1;

  ASSERT_THROW(sltl::call(fn_one_param, v1), std::exception); // Pass the wrong type of function argument
}

TEST(call, call_fn_arg_wrong_number)
{
  sltl::syntax::block blk(sltl::syntax::block::global);

  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block_guard bg(blk);

  sltl::matrix<float, 3> m1;
  sltl::matrix<float, 3> m2;

  ASSERT_THROW(sltl::call(fn_one_param, m1, m2), std::exception); // Pass the wrong number of function arguments
}

namespace
{
  void fn_order_1st(sltl::scalar<int>) // Called second will be assigned 'fn2' as a name
  {
  }

  void fn_order_2nd(sltl::scalar<bool>) // Called third will be assigned 'fn3' as a name
  {
    sltl::call(fn_order_1st, sltl::scalar<int>(int())); // Call fn_order_1st from a depth '2' function, re-assign it a depth of '3' (was depth '2')
  };

  void fn_order_3rd(sltl::scalar<float>) // Called first will be assigned 'fn1' as a name
  {
    sltl::call(fn_order_1st, sltl::scalar<int>(int()));  // Call fn_order_1st from a depth '1' function, assign it a depth of '2'
    sltl::call(fn_order_2nd, sltl::scalar<bool>(bool())); // Call fn_order_2nd from a depth '1' function, assign it a depth of '2'
  };
}

TEST(call, call_fn_order)
{
  auto test_shader = [](sltl::shader::tag<sltl::core::shader_stage::test>, io_block_empty)
  {
    sltl::call(fn_order_3rd, sltl::scalar<float>(float())); // Call fn_order_3rd from a depth '0' function, assign it a depth of '1'
  };

  // This test uses make_shader as make_test doesn't output function definitions
  const std::wstring actual = ::to_string(sltl::make_shader(test_shader));
  const std::wstring expected = LR"(
void fn2(int p_i1)
{
}
void fn3(bool p_b1)
{
  fn2(int(0));
}
void fn1(float p_f1)
{
  fn2(int(0));
  fn3(bool(false));
}
void main()
{
  fn1(float(0.0f));
}
)";

  ASSERT_EQ(expected, actual);
}
