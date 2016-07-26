#include <gtest/gtest.h>

#include "if.h"
#include "shader.h"
#include "output.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(sltl::output_version::none, false);
  }
}

TEST(if_, if_empty)
{
  auto test_shader = []()
  {
    sltl::if_(true, []()
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

TEST(if_, if_else_empty)
{
  auto test_shader = []()
  {
    sltl::if_(true, []()
    {
    }).else_([]()
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

TEST(if_, if_else_if_else_empty)
{
  auto test_shader = []()
  {
    sltl::if_(true, []()
    {
    }).else_if(false, []()
    {
    }).else_([]()
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

TEST(if_, if_condition)
{
  auto test_shader = []()
  {
    sltl::scalar<bool> cond1 = true;

    sltl::if_(cond1, []()
    {
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  bool b1(true);
  if(b1)
  {
  }
}
)";

  ASSERT_EQ(expected, actual);
}

TEST(if_, if_body)
{
  auto test_shader = []()
  {
    sltl::scalar<bool> cond1 = true;
    sltl::scalar<int> count = 0;

    sltl::if_(cond1, [&]()
    {
      count += 1;
    }).else_([&]()
    {
      count += 2;
    });
  };

  const std::wstring actual = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  bool b1(true);
  int i2(0);
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
