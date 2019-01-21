#pragma once

#include <syntax/action.h>

#include <core/shader_stage.h>


namespace sltl
{
  class output_matrix_order : public syntax::action
  {
  public:
    output_matrix_order(output_matrix_order&&) = default;
    output_matrix_order(core::shader_stage);

    // Non-copyable and non-assignable
    output_matrix_order(const output_matrix_order&) = delete;
    output_matrix_order& operator=(output_matrix_order&&) = delete;
    output_matrix_order& operator=(const output_matrix_order&) = delete;

    virtual syntax::action_return_t operator()(syntax::variable_declaration& vd, bool is_start = true) override;
    virtual syntax::action_return_t operator()(syntax::temporary& t, bool is_start = true) override;
    virtual syntax::action_return_t operator()(syntax::operator_binary& ob, bool is_start = true) override;
    virtual syntax::action_return_t operator()(syntax::operator_component_access& oca, bool is_start = true) override;
    virtual syntax::action_return_t operator()(syntax::constructor_call& cc, bool is_start = true) override;
    virtual syntax::action_return_t operator()(syntax::function_definition& fd, bool is_start = true) override;

  protected:
    virtual syntax::action_return_t get_default(bool is_start) override;
  };
}
