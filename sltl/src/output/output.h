#pragma once

#include <syntax/action.h>

#include <core/shader_stage.h>

#include <detail/enum_flags.h>

#include <string>
#include <sstream>


namespace sltl
{
  // Forward declarations - sltl::language namespace
  namespace language
  {
    class type;
  }

  enum class output_flags
  {
    flag_none           = 0x0,
    flag_indent_space   = 0x1,
    flag_transpose_type = flag_indent_space << 1
  };

  class output : public syntax::const_action_result<std::wstring>
  {
  public:
    output(output&&) = default;
    output(core::shader_stage stage, detail::enum_flags<output_flags> flags = output_flags::flag_none);

    // Non-copyable and non-assignable
    output(const output&) = delete;
    output& operator=(output&&) = delete;
    output& operator=(const output&) = delete;

    syntax::action_return_t operator()(const syntax::block&, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::io_block&, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::parameter_declaration& pd) override;
    syntax::action_return_t operator()(const syntax::parameter_list& pl, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::reference& r) override;
    syntax::action_return_t operator()(const syntax::temporary& t, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::operator_unary& ou, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::operator_binary& ob, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::operator_component_access& oca, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::conditional& c, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::constructor_call& cc, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::expression_statement& es, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::expression_list& el, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::parentheses& p, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::function_call& fc, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::function_definition& fd, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::return_statement& rs, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::intrinsic_call& ic, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::intrinsic_declaration& id, bool is_start = true) override;

    std::wstring get_result() const override;

  protected:
    syntax::action_return_t operator()(float f) override;
    syntax::action_return_t operator()(double d) override;
    syntax::action_return_t operator()(int i) override;
    syntax::action_return_t operator()(unsigned int ui) override;
    syntax::action_return_t operator()(bool b) override;

    syntax::action_return_t get_default(bool is_start) override;

    virtual std::wstring get_type_name(const language::type& type) const = 0;

    void line_begin();
    void line_end(bool has_semi_colon = true);

    std::wstringstream _ss;

    const detail::enum_flags<output_flags> _flags;

  private:
    size_t _indent_count;
    const core::shader_stage _stage;
  };
}
