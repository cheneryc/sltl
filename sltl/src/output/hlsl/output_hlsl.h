#pragma once

#include <output/output.h>


namespace sltl
{
namespace hlsl
{
  class output_hlsl : public output
  {
  public:
    output_hlsl(output_hlsl&&) = default;
    output_hlsl(core::shader_stage stage, detail::enum_flags<output_flags> flags = output_flags::flag_none);

    // Non-copyable and non-assignable
    output_hlsl(const output_hlsl&) = delete;
    output_hlsl& operator=(output_hlsl&&) = delete;
    output_hlsl& operator=(const output_hlsl&) = delete;

    syntax::action_return_t operator()(const syntax::io_block& iob, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::reference& r) override;
    syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;
    syntax::action_return_t operator()(const syntax::function_definition& fd, bool is_start = true) override;

  protected:
    std::wstring get_type_name(const language::type& type) const override;
    std::wstring get_variable_name(const syntax::variable_declaration& vd) const override;
    std::wstring get_parameter_name(const syntax::parameter_declaration& pd) const override;

    const wchar_t* to_intrinsic_string(core::intrinsic intrinsic) const override;
    const wchar_t* to_intrinsic_operator_string(const syntax::operator_binary& ob) const override;

  private:
    const syntax::io_block* _block_in;
    const syntax::io_block* _block_out;
  };
}
}
