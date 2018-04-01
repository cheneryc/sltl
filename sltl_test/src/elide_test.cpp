#include <gtest/gtest.h>

#include "syntax/elide.h"
#include "syntax/block.h"
#include "syntax/block_guard.h"
#include "syntax/block_manager.h"
#include "syntax/literal.h"
#include "syntax/temporary.h"
#include "syntax/variable_declaration.h"

#include "output.h"

#include <locale>


namespace
{
  //TODO: move into a util/string.h/cpp file
  std::wstring& ltrim(std::wstring& str)
  {
    auto it = std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !std::isspace(ch, std::locale::classic()); });
    str.erase(str.begin(), it);
    return str;
  }

  std::wstring& rtrim(std::wstring& str)
  {
    auto it = std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !std::isspace(ch, std::locale::classic()); });
    str.erase(it.base(), str.end());
    return str;
  }

  std::wstring& trim(std::wstring& str)
  {
    return ltrim(rtrim(str));
  }

  std::wstring to_string(const sltl::syntax::node& node)
  {
    sltl::output o(sltl::core::shader_stage::test, sltl::output_version::none, sltl::output_flags::flag_indent_space);
    node.apply_action(o);

    std::wstring result = o.get_result();
    rtrim(result);// trim the trailing newline

    return std::move(result);
  }
}

TEST(elide, elide_default)
{
  using namespace sltl::syntax;

  expression::ptr exp;
  expression::ptr exp_result;

  block blk(block::global);

  block_manager_guard bm;
  block_guard bg(blk);

  {
    auto& vd1 = blk.add<variable_declaration>(expression::make<literal<float>>(1.0f));

    const std::wstring actual = ::to_string(vd1);
    const std::wstring expected = L"float f1 = 1.0f;";

    ASSERT_EQ(expected, actual);

    exp = vd1.move();
    exp_result = elide(std::move(exp), exp->get_type());

    blk.erase(vd1);
  }

  {
    auto& vd2 = blk.add<variable_declaration>(std::move(exp_result));

    const std::wstring actual = ::to_string(vd2);
    const std::wstring expected = L"float f2 = 1.0f;";

    ASSERT_EQ(expected, actual);
  }
}

TEST(elide, elide_temporary_single)
{
  using namespace sltl::syntax;

  expression::ptr exp;
  expression::ptr exp_result;

  block blk(block::global);

  block_manager_guard bm;
  block_guard bg(blk);

  {
    exp = expression::make<temporary>(
          expression::make<literal<float>>(1.0f));

    auto& vd1 = blk.add<variable_declaration>(std::move(exp));

    const std::wstring actual = ::to_string(vd1);
    const std::wstring expected = L"float f1 = float(1.0f);";

    ASSERT_EQ(expected, actual);

    exp = vd1.move();
    exp_result = elide(std::move(exp), exp->get_type());

    blk.erase(vd1);
  }

  {
    auto& vd2 = blk.add<variable_declaration>(std::move(exp_result));

    const std::wstring actual = ::to_string(vd2);
    const std::wstring expected = L"float f2 = 1.0f;";

    ASSERT_EQ(expected, actual);
  }
}

TEST(elide, elide_temporary_single_leaf)
{
  using namespace sltl::syntax;

  expression::ptr exp;
  expression::ptr exp_result;

  block blk(block::global);

  block_manager_guard bm;
  block_guard bg(blk);

  {
    auto& vd1 = blk.add<variable_declaration>(expression::make<temporary>(sltl::language::type_helper<float>()));

    const std::wstring actual = ::to_string(vd1);
    const std::wstring expected = L"float f1 = float(0.0f);";// The temporary has no initializer, the glsl output action is adding the zero-initialization

    ASSERT_EQ(expected, actual);

    exp = vd1.move();
    exp_result = elide(std::move(exp), exp->get_type());

    blk.erase(vd1);
  }

  {
    auto& vd2 = blk.add<variable_declaration>(std::move(exp_result));

    const std::wstring actual = ::to_string(vd2);
    const std::wstring expected = L"float f2 = float(0.0f);";// The temporary has no initializer, the glsl output action is adding the zero-initialization

    ASSERT_EQ(expected, actual);
  }
}

TEST(elide, elide_temporary_multiple)
{
  using namespace sltl::syntax;

  expression::ptr exp;
  expression::ptr exp_result;

  block blk(block::global);

  block_manager_guard bm;
  block_guard bg(blk);

  {
    exp = expression::make<temporary>(
          expression::make<temporary>(
          expression::make<temporary>(
          expression::make<literal<float>>(1.0f))));

    auto& vd1 = blk.add<variable_declaration>(std::move(exp));

    const std::wstring actual = ::to_string(vd1);
    const std::wstring expected = L"float f1 = float(float(float(1.0f)));";

    ASSERT_EQ(expected, actual);

    exp = vd1.move();
    exp_result = elide(std::move(exp), exp->get_type());

    blk.erase(vd1);
  }

  {
    auto& vd2 = blk.add<variable_declaration>(std::move(exp_result));

    const std::wstring actual = ::to_string(vd2);
    const std::wstring expected = L"float f2 = 1.0f;";

    ASSERT_EQ(expected, actual);
  }
}

TEST(elide, elide_temporary_multiple_leaf)
{
  using namespace sltl::syntax;

  expression::ptr exp;
  expression::ptr exp_result;

  block blk(block::global);

  block_manager_guard bm;
  block_guard bg(blk);

  {
    exp = expression::make<temporary>(
          expression::make<temporary>(
          expression::make<temporary>(sltl::language::type_helper<float>())));

    auto& vd1 = blk.add<variable_declaration>(std::move(exp));

    const std::wstring actual = ::to_string(vd1);
    const std::wstring expected = L"float f1 = float(float(float(0.0f)));";// The temporary has no initializer, the glsl output action is adding the zero-initialization

    ASSERT_EQ(expected, actual);

    exp = vd1.move();
    exp_result = elide(std::move(exp), exp->get_type());

    blk.erase(vd1);
  }

  {
    auto& vd2 = blk.add<variable_declaration>(std::move(exp_result));

    const std::wstring actual = ::to_string(vd2);
    const std::wstring expected = L"float f2 = float(0.0f);";// The temporary has no initializer, the glsl output action is adding the zero-initialization

    ASSERT_EQ(expected, actual);
  }
}
