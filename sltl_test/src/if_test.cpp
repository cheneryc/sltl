#include <gtest/gtest.h>

#include "if.h"
#include "shader.h"


namespace
{
  std::wstring to_string(const sltl::shader& shader)
  {
    // Prepend a newline character to exactly match the raw string literals
    return L'\n' + shader.str<sltl::output>(false);
  }
}

TEST(if_, if_empty)
{
  auto test_shader = []()
  {
    sltl::if_(true, []()
    {
    });

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  if(true)
  {
  }
}
)";

  ASSERT_EQ(result, expected);
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

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_test(test_shader));
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

  ASSERT_EQ(result, expected);
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

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_test(test_shader));
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

  ASSERT_EQ(result, expected);
}

TEST(if_, if_condition)
{
  auto test_shader = []()
  {
    sltl::scalar<bool> cond1 = true;

    sltl::if_(cond1, []()
    {
    });

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_test(test_shader));
  const std::wstring expected = LR"(
{
  bool b1(true);
  if(b1)
  {
  }
}
)";

  ASSERT_EQ(result, expected);
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

    return 0;
  };

  const std::wstring result = ::to_string(sltl::make_test(test_shader));
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

  ASSERT_EQ(result, expected);
}
