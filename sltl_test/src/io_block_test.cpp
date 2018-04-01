#include <gtest/gtest.h>

#include "syntax/block_manager.h"
#include "syntax/io_block.h"
#include "syntax/io_block_manager.h"

#include "scalar.h"


TEST(io_block, variable_info_find_variable_declaration)
{
  sltl::syntax::io_block_manager_guard bm;
  sltl::syntax::io_block& b1 = bm->add(sltl::core::qualifier_storage::uniform);

  b1.push();

  ASSERT_EQ(b1, sltl::syntax::get_current_block());
  ASSERT_TRUE(sltl::syntax::is_override_active());

  sltl::syntax::variable_declaration& vd = b1.add<sltl::syntax::variable_declaration>(sltl::language::type_helper<sltl::scalar<float>>(), sltl::core::semantic_pair(sltl::core::semantic::user));

  const auto* const var1_vi = b1.variable_info_find(vd._name);

  ASSERT_TRUE(var1_vi);
  ASSERT_EQ(0U, var1_vi->get_ref());

  // Make a copy of the variable_declaration's name before erasing it
  const std::wstring var1_name = vd._name;

  b1.erase(vd);

  ASSERT_FALSE(b1.variable_info_find(var1_name));

  b1.pop();

  ASSERT_FALSE(sltl::syntax::is_override_active());
}
