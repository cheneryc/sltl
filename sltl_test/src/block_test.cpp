#include <gtest/gtest.h>

#include "syntax/block.h"
#include "syntax/block_manager.h"

#include "scalar.h"


TEST(block, push_empty_stack)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  b1.push();
  sltl::syntax::block b2(sltl::syntax::block::local); // There must be a 'current' block or 'local' block creation will fail
  b1.pop();

  ASSERT_THROW(b2.push(), std::exception);
}

TEST(block, push_duplicate)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  b1.push();

  // Use EXPECT, rather than ASSERT, so the test can continue. This allows
  // the block(s) to be popped off the block stack and therefore avoid the
  // block_manager's destructor 'is empty' assert firing.
  EXPECT_THROW(b1.push(), std::exception);

  b1.pop();
}

TEST(block, push_duplicate_multiple)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  b1.push();

  sltl::syntax::block b2(sltl::syntax::block::local);

  b2.push();

  sltl::syntax::block b3(sltl::syntax::block::local);

  b3.push();

  // Use EXPECT, rather than ASSERT, so the test can continue. This allows
  // the block(s) to be popped off the block stack and therefore avoid the
  // block_manager's destructor 'is empty' assert firing.
  EXPECT_THROW(b1.push(), std::exception);

  b3.pop();
  b2.pop();
  b1.pop();
}

TEST(block, add_block_global)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  ASSERT_THROW(b1.add<sltl::syntax::block>(sltl::syntax::block::global), std::exception);
}

TEST(block, variable_info_find)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  b1.push();

  ASSERT_EQ(b1, sltl::syntax::get_current_block());
  ASSERT_FALSE(sltl::syntax::is_override_active());

  sltl::syntax::variable_declaration& vd = b1.add<sltl::syntax::variable_declaration>(sltl::language::type_helper<sltl::scalar<float>>());

  const auto* const var1_vi = b1.variable_info_find(vd._name);

  ASSERT_TRUE(var1_vi);
  ASSERT_EQ(0U, var1_vi->get_ref());

  // Make a copy of the variable_declaration's name before erasing it
  const std::wstring var1_name = vd._name;

  b1.erase(vd);

  ASSERT_FALSE(b1.variable_info_find(var1_name));

  b1.pop();

  ASSERT_TRUE(bm->get_block_stack().empty());
}

TEST(block, variable_info_find_scope_parent)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  b1.push();

  ASSERT_EQ(b1, sltl::syntax::get_current_block());
  ASSERT_FALSE(sltl::syntax::is_override_active());

  sltl::syntax::block& b2 = b1.add<sltl::syntax::block>(sltl::syntax::block::local);

  b2.push();

  ASSERT_EQ(b2, sltl::syntax::get_current_block());
  ASSERT_FALSE(sltl::syntax::is_override_active());

  sltl::syntax::variable_declaration& vd = b1.add<sltl::syntax::variable_declaration>(sltl::language::type_helper<sltl::scalar<float>>());

  const auto* const var1_vi = b2.variable_info_find(vd._name);

  ASSERT_TRUE(var1_vi);
  ASSERT_EQ(0U, var1_vi->get_ref());

  // Make a copy of the variable_declaration's name before erasing it
  const std::wstring var1_name = vd._name;

  b1.erase(vd);

  ASSERT_FALSE(b2.variable_info_find(var1_name));

  b2.pop();
  b1.pop();

  ASSERT_TRUE(bm->get_block_stack().empty());
}

TEST(block, variable_info_find_scope_block_type)
{
  sltl::syntax::block_manager_guard bm;
  sltl::syntax::block b1(sltl::syntax::block::global);

  // Add a variable declaration to the 'root' block
  sltl::syntax::variable_declaration& vd = b1.add<sltl::syntax::variable_declaration>(sltl::language::type_helper<sltl::scalar<float>>());

  b1.push();
  ASSERT_EQ(b1, sltl::syntax::get_current_block());

  sltl::syntax::block b2(sltl::syntax::block::local);

  b2.push();
  ASSERT_EQ(b2, sltl::syntax::get_current_block());

  // variable_info_find can only be called if a block is currently at the top of the stack
  // variable_info_find - should successfully find the variable_info for 'vd'
  ASSERT_TRUE(b2.variable_info_find(vd._name) != nullptr);

  sltl::syntax::block b3(sltl::syntax::block::global);

  b3.push();
  ASSERT_EQ(b3, sltl::syntax::get_current_block());

  sltl::syntax::block b4(sltl::syntax::block::local);

  b4.push();
  ASSERT_EQ(b4, sltl::syntax::get_current_block());

  // variable_info_find can only be called if a block is currently at the top of the stack
  // variable_info_find - cannot find the variable_info for 'vd'. Recursive querying of ancestor blocks should stop when it reaches 'global' block b3
  ASSERT_TRUE(b4.variable_info_find(vd._name) == nullptr);

  b4.pop();
  b3.pop();
  b2.pop();
  b1.pop();
}
