#include <gtest/gtest.h>

#include "syntax/block.h"
#include "syntax/block_manager.h"

#include "scalar.h"


TEST(block, variable_info_find)
{
  sltl::syntax::block b1(sltl::syntax::block::global);

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

  ASSERT_TRUE(sltl::syntax::block_manager::get().get_block_stack().empty());
}

TEST(block, variable_info_find_scope_parent)
{
  sltl::syntax::block b1(sltl::syntax::block::global);

  ASSERT_EQ(b1, sltl::syntax::get_current_block());
  ASSERT_FALSE(sltl::syntax::is_override_active());

  sltl::syntax::block& b2 = b1.add<sltl::syntax::block>(sltl::syntax::block::local);

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

  ASSERT_TRUE(sltl::syntax::block_manager::get().get_block_stack().empty());
}
