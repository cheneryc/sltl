#pragma once

#include <syntax/action.h>

#include <type.h>

#include <core/shader_stage.h>

#include <detail/enum_flags.h>

#include <string>
#include <sstream>


namespace sltl
{
  enum class output_flags
  {
    flag_none           = 0x0,
    flag_indent_space   = 0x1,
    flag_extra_newlines = 0x2,
    flag_transpose_type = 0x4
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

    void operator()(language::bracket_tag<language::id_brace>, bool is_start);
    void operator()(language::bracket_tag<language::id_parenthesis>, bool is_start);

    syntax::action_return_t get_default(bool is_start) override;

    virtual std::wstring get_type_name(const language::type& type) const = 0;
    virtual std::wstring get_variable_name(const syntax::variable_declaration& vd) const = 0;
    virtual std::wstring get_parameter_name(const syntax::parameter_declaration& pd) const = 0;

    enum class indent_t
    {
      current,
      increase,
      decrease
    };

    std::wstring get_indent(indent_t indent_op);

    const wchar_t  get_newline() const;
    const wchar_t  get_terminal() const;
    const wchar_t* get_terminal_newline() const;

    std::wstringstream _ss;

    const detail::enum_flags<output_flags> _flags;
    const core::shader_stage _stage;

  private:
    size_t _indent_count;
  };
}
