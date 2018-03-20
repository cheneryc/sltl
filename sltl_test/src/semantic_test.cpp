#include <gtest/gtest.h>

#include "core/semantic.h"


TEST(semantic, semantic_system_to_index)
{
  constexpr sltl::core::semantic_index_t max_index = sltl::core::semantic_index_limits<sltl::core::semantic_system>::max();

  ASSERT_EQ(0,  sltl::core::detail::to_semantic_index(sltl::core::semantic_system::position, 0));
  ASSERT_EQ(1,  sltl::core::detail::to_semantic_index(sltl::core::semantic_system::position, 1));
  ASSERT_EQ(15, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::position, max_index));

  ASSERT_EQ(16, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::depth, 0));
  ASSERT_EQ(17, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::depth, 1));
  ASSERT_EQ(31, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::depth, max_index));

  ASSERT_EQ(32, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::target, 0));
  ASSERT_EQ(33, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::target, 1));
  ASSERT_EQ(47, sltl::core::detail::to_semantic_index(sltl::core::semantic_system::target, max_index));
}

TEST(semantic, semantic_system_to_index_none)
{
  ASSERT_THROW(sltl::core::detail::to_semantic_index(sltl::core::semantic_system::none, 0), std::exception);
}

TEST(semantic, semantic_transform_to_index)
{
  ASSERT_EQ(0, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::model));
  ASSERT_EQ(1, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::modelview));
  ASSERT_EQ(2, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::modelviewproj));
  ASSERT_EQ(3, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::modelviewproj_inverse));
  ASSERT_EQ(4, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::modelviewproj_inversetrans));
  ASSERT_EQ(5, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::view));
  ASSERT_EQ(6, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::viewproj));
  ASSERT_EQ(7, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::proj));
  ASSERT_EQ(8, sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::normal));
}

TEST(semantic, semantic_transform_to_index_none)
{
  ASSERT_THROW(sltl::core::detail::to_semantic_index(sltl::core::semantic_transform::none), std::exception);
}

TEST(semantic, semantic_index_to_system_pair)
{
  constexpr sltl::core::semantic_index_t max_index = sltl::core::semantic_index_limits<sltl::core::semantic_system>::max();

  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::position, sltl::core::semantic_index_t(0)), sltl::core::detail::to_semantic_system_pair(0U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::position, sltl::core::semantic_index_t(1)), sltl::core::detail::to_semantic_system_pair(1U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::position, max_index),                       sltl::core::detail::to_semantic_system_pair(15U));

  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::depth, sltl::core::semantic_index_t(0)), sltl::core::detail::to_semantic_system_pair(16U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::depth, sltl::core::semantic_index_t(1)), sltl::core::detail::to_semantic_system_pair(17U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::depth, max_index),                       sltl::core::detail::to_semantic_system_pair(31U));

  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::target, sltl::core::semantic_index_t(0)), sltl::core::detail::to_semantic_system_pair(32U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::target, sltl::core::semantic_index_t(1)), sltl::core::detail::to_semantic_system_pair(33U));
  ASSERT_EQ(std::make_pair(sltl::core::semantic_system::target, max_index),                       sltl::core::detail::to_semantic_system_pair(47U));
}
