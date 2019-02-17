#include <gtest/gtest.h>

#include "if.h"
#include "basic_operators.h"
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

TEST(if_then, if_then_empty)
{
  auto test_shader = []()
  {
    sltl::if_then(true, []()
    {
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  if(true)
  {
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(if_then, if_then_else_end_empty)
{
  auto test_shader = []()
  {
    sltl::if_then(true, []()
    {
    }).else_end([]()
    {
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  if(true)
  {
  }
  else
  {
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(if_then, if_then_else_if_else_end_empty)
{
  auto test_shader = []()
  {
    sltl::if_then(true, []()
    {
    }).else_if(false, []()
    {
    }).else_end([]()
    {
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  if(true)
  {
  }
  else if(false)
  {
  }
  else
  {
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(if_then, if_then_condition)
{
  auto test_shader = []()
  {
    sltl::scalar<bool> cond1 = true;

    sltl::if_then(cond1, []()
    {
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  bool b1 = true;
  if(b1)
  {
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(if_then, if_then_body)
{
  auto test_shader = []()
  {
    sltl::scalar<bool> cond1 = true;
    sltl::scalar<int> count = 0;

    sltl::if_then(cond1, [&]()
    {
      count += 1;
    }).else_end([&]()
    {
      count += 2;
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  bool b1 = true;
  int i2 = 0;
  if(b1)
  {
    i2 += 1;
  }
  else
  {
    i2 += 2;
  }
}
)";

  ASSERT_EQ(expected, actual);
}
