#include <gtest/gtest.h>

#include <detail/variant.h>

#include <memory>


TEST(variant, construct_type_same_success)
{
  using variant_t = sltl::api::detail::variant<int, char, bool, double, std::string>;

  {
    variant_t var(0U);

    const int int_expected = 1;
    var.construct(int_expected); // Construct int at index 0
    const int int_actual = var.get_type<int>();

    var.destruct_type<int>();

    EXPECT_EQ(int_expected, int_actual);
  }

  {
    variant_t var(1U);

    const char char_expected = 'a';
    var.construct(char_expected); // Construct char at index 1
    const char char_actual = var.get_type<char>();

    var.destruct_type<char>();

    EXPECT_EQ(char_expected, char_actual);
  }

  {
    variant_t var(2U);

    const bool bool_expected = true;
    var.construct(bool_expected); // Construct bool at index 2
    const bool bool_actual = var.get_type<bool>();

    var.destruct_type<bool>();

    EXPECT_EQ(bool_expected, bool_actual);
  }

  {
    variant_t var(3U);

    const double double_expected = 2.0;
    var.construct(double_expected); // Construct double at index 3
    const double double_actual = var.get_type<double>();

    var.destruct_type<double>();

    EXPECT_EQ(double_expected, double_actual);
  }

  {
    variant_t var(4U);

    const std::string string_expected = "Test!";
    var.construct(string_expected); // Construct std::string at index 4
    const std::string string_actual = var.get_type<std::string>();

    var.destruct_type<std::string>();

    EXPECT_EQ(string_expected, string_actual);
  }
}

TEST(variant, construct_type_different_success)
{
  using variant_t = sltl::api::detail::variant<int, char, bool, double, std::string>;

  {
    variant_t var(0U);

    const char char_expected = 'a';
    var.construct(char_expected); // Construct char at index 0 (int)
    const int int_actual = var.get_type<int>();

    var.destruct_type<int>();

    EXPECT_EQ(char_expected, int_actual);
  }

  {
    variant_t var(0U);

    const bool bool_expected = true;
    var.construct(bool_expected); // Construct bool at index 0 (int)
    const int int_actual = var.get_type<int>();

    var.destruct_type<int>();

    EXPECT_EQ(static_cast<int>(bool_expected), int_actual);
  }

  {
    variant_t var(1U);

    const bool bool_expected = true;
    var.construct(bool_expected); // Construct bool at index 1 (char)
    const char char_actual = var.get_type<char>();

    var.destruct_type<char>();

    EXPECT_EQ(static_cast<char>(bool_expected), char_actual);
  }

  {
    variant_t var(4U);

    const char char_expected = 'a';
    var.construct(char_expected); // Construct char at index 4 (string)
    const std::string string_actual = var.get_type<std::string>();

    var.destruct_type<std::string>();

    EXPECT_EQ(std::string{ char_expected }, string_actual);
  }

  {
    variant_t var(4U);

    const bool bool_expected = true;
    var.construct(bool_expected); // Construct bool at index 4 (string)
    const std::string string_actual = var.get_type<std::string>();

    var.destruct_type<std::string>();

    EXPECT_EQ(std::string{ bool_expected }, string_actual);
  }
}

TEST(variant, construct_type_different_exception)
{
  using variant_t = sltl::api::detail::variant<int, char, bool, double, std::string>;

  const int int_arg = 1;
  const char char_arg = 'a';
  const bool bool_arg = true;
  const double double_arg = 1.0;
  const std::string string_arg = "Test!";

  {
    variant_t var(0U); // index 0 -> int

    EXPECT_THROW(var.construct(double_arg), std::exception);
    EXPECT_THROW(var.construct(string_arg), std::exception);
  }

  {
    variant_t var(1U); // index 1 -> char

    EXPECT_THROW(var.construct(int_arg), std::exception);
    EXPECT_THROW(var.construct(double_arg), std::exception);
    EXPECT_THROW(var.construct(string_arg), std::exception);
  }

  {
    variant_t var(2U); // index 2 -> bool

    EXPECT_THROW(var.construct(int_arg), std::exception);
    EXPECT_THROW(var.construct(char_arg), std::exception);
    EXPECT_THROW(var.construct(double_arg), std::exception);
    EXPECT_THROW(var.construct(string_arg), std::exception);
  }

  {
    variant_t var(3U); // index 3 -> double

    EXPECT_THROW(var.construct(int_arg), std::exception);
    EXPECT_THROW(var.construct(char_arg), std::exception);
    EXPECT_THROW(var.construct(bool_arg), std::exception);
    EXPECT_THROW(var.construct(string_arg), std::exception);
  }

  {
    variant_t var(4U); // index 4 -> std::string

    EXPECT_THROW(var.construct(int_arg), std::exception);
    EXPECT_THROW(var.construct(double_arg), std::exception);
  }
}

TEST(variant, visit)
{
  using namespace sltl::api::detail;

  enum visitor_result
  {
    visited_none,
    visited_vector,
    visited_string
  };

  using variant_t = variant<std::vector<int>, std::string>;

  struct
  {
    std::shared_ptr<visitor_result> result; // The visitor will be copied so share this member's value amongst all the visitor instances

    void operator()(variant_t& var, variant_tag<std::vector<int>>)
    {
      *result = visited_vector;
    }

    void operator()(variant_t& var, variant_tag<std::string>)
    {
      *result = visited_string;
    }
  } visitor;

  visitor.result = std::make_shared<visitor_result>(visited_none);

  {
    variant_t var(0U);

    EXPECT_EQ(visited_none, *visitor.result);
    var.visit(visitor);
    EXPECT_EQ(visited_vector, *visitor.result);
  }

  *visitor.result = visited_none;

  {
    variant_t var(1U);

    EXPECT_EQ(visited_none, *visitor.result);
    var.visit(visitor);
    EXPECT_EQ(visited_string, *visitor.result);
  }
}
