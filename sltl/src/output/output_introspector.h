#pragma once

#include <syntax/action.h>

#include <core/qualifier.h>
#include <core/semantic.h>
#include <core/shader_stage.h>

#include <string>


namespace sltl
{
  class output_introspector : public syntax::const_action_result<std::wstring>
  {
  public:
    output_introspector(output_introspector&&) = default;
    output_introspector(core::shader_stage, core::qualifier_storage qualifier, core::semantic_pair semantic);

    // Non-copyable and non-assignable
    output_introspector(const output_introspector&) = delete;
    output_introspector& operator=(output_introspector&&) = delete;
    output_introspector& operator=(const output_introspector&) = delete;

    virtual syntax::action_return_t operator()(const syntax::io_block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;

    virtual std::wstring get_result() const override;

    const core::semantic _semantic;
    const core::semantic_index_t _semantic_index;

    const core::qualifier_storage _qualifier;

  protected:
    virtual syntax::action_return_t get_default(bool is_start) override;

  private:
    std::wstring _name;
  };
}
