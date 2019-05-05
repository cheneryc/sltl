#include <gtest/gtest.h>

#include "detail/comparison.h"
#include "detail/variadic_algorithm.h"

#include <vector>


namespace
{
  enum class op_t
  {
    lt,
    lt_eq,
    gt,
    gt_eq,
    eq,
    neq
  };

  template<op_t Op1, op_t Op2>
  struct is_op : std::false_type {};

  template<op_t Op>
  struct is_op<Op, Op> : std::true_type {};

  template<op_t ...Op>
  struct op_tracker
  {
    op_tracker(std::vector<op_t>& ops) : _ops(ops) {}

    bool operator< (const op_tracker&) const { _ops.push_back(op_t::lt);    return sltl::detail::any<is_op<Op, op_t::lt>...>::value; }
    bool operator<=(const op_tracker&) const { _ops.push_back(op_t::lt_eq); return sltl::detail::any<is_op<Op, op_t::lt_eq>...>::value; }
    bool operator> (const op_tracker&) const { _ops.push_back(op_t::gt);    return sltl::detail::any<is_op<Op, op_t::gt>...>::value; }
    bool operator>=(const op_tracker&) const { _ops.push_back(op_t::gt_eq); return sltl::detail::any<is_op<Op, op_t::gt_eq>...>::value; }
    bool operator==(const op_tracker&) const { _ops.push_back(op_t::eq);    return sltl::detail::any<is_op<Op, op_t::eq>...>::value; }
    bool operator!=(const op_tracker&) const { _ops.push_back(op_t::neq);   return sltl::detail::any<is_op<Op, op_t::neq>...>::value; }

    std::vector<op_t>& _ops;
  };
}

TEST(comparison, less)
{
  // 0 values
  ASSERT_EQ(false, sltl::detail::less(0, 0, 0, 0));

  // 1 value
  ASSERT_EQ(false, sltl::detail::less(1, 0, 0, 0));
  ASSERT_EQ(false, sltl::detail::less(0, 1, 0, 0));
  ASSERT_EQ(true,  sltl::detail::less(0, 0, 1, 0));
  ASSERT_EQ(true,  sltl::detail::less(0, 0, 0, 1));

  // 2 values
  ASSERT_EQ(false, sltl::detail::less(1, 1, 0, 0));
  ASSERT_EQ(false, sltl::detail::less(1, 0, 1, 0));
  ASSERT_EQ(false, sltl::detail::less(1, 0, 0, 1));
  ASSERT_EQ(true,  sltl::detail::less(0, 1, 1, 0));
  ASSERT_EQ(false, sltl::detail::less(0, 1, 0, 1));
  ASSERT_EQ(true,  sltl::detail::less(0, 0, 1, 1));

  // 3 values
  ASSERT_EQ(false, sltl::detail::less(1, 1, 1, 0));
  ASSERT_EQ(false, sltl::detail::less(1, 1, 0, 1));
  ASSERT_EQ(true,  sltl::detail::less(1, 0, 1, 1));
  ASSERT_EQ(true,  sltl::detail::less(0, 1, 1, 1));

  // 4 values
  ASSERT_EQ(false, sltl::detail::less(1, 1, 1, 1));
}

TEST(comparison, less_order)
{
  {
    std::vector<op_t> ops_actual;
    std::vector<op_t> ops_expected = { op_t::lt };

    op_tracker<op_t::lt> lhs_first(ops_actual);
    op_tracker<op_t::lt> rhs_first(ops_actual);
    op_tracker<> lhs_second(ops_actual);
    op_tracker<> rhs_second(ops_actual);

    ASSERT_TRUE(sltl::detail::less(lhs_first, lhs_second, rhs_first, rhs_second));
    ASSERT_EQ(ops_expected, ops_actual);
  }

  {
    std::vector<op_t> ops_actual;
    std::vector<op_t> ops_expected = { op_t::lt, op_t::eq };

    op_tracker<> lhs_first(ops_actual);
    op_tracker<> rhs_first(ops_actual);
    op_tracker<> lhs_second(ops_actual);
    op_tracker<> rhs_second(ops_actual);

    ASSERT_FALSE(sltl::detail::less(lhs_first, lhs_second, rhs_first, rhs_second));
    ASSERT_EQ(ops_expected, ops_actual);
  }

  {
    std::vector<op_t> ops_actual;
    std::vector<op_t> ops_expected = { op_t::lt, op_t::eq, op_t::lt };

    op_tracker<op_t::eq> lhs_first(ops_actual);
    op_tracker<op_t::eq> rhs_first(ops_actual);
    op_tracker<> lhs_second(ops_actual);
    op_tracker<> rhs_second(ops_actual);

    ASSERT_FALSE(sltl::detail::less(lhs_first, lhs_second, rhs_first, rhs_second));
    ASSERT_EQ(ops_expected, ops_actual);
  }

  {
    std::vector<op_t> ops_actual;
    std::vector<op_t> ops_expected = { op_t::lt, op_t::eq, op_t::lt };

    op_tracker<op_t::eq> lhs_first(ops_actual);
    op_tracker<op_t::eq> rhs_first(ops_actual);
    op_tracker<op_t::lt> lhs_second(ops_actual);
    op_tracker<op_t::lt> rhs_second(ops_actual);

    ASSERT_TRUE(sltl::detail::less(lhs_first, lhs_second, rhs_first, rhs_second));
    ASSERT_EQ(ops_expected, ops_actual);
  }
}
