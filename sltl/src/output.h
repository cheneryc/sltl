#pragma once

#include "syntax/action.h"

#include "core/qualifier.h"
#include "core/semantic.h"

#include <string>
#include <sstream>


namespace sltl
{
  class output : public syntax::const_action_result<std::wstring>
  {
  public:
    output(bool is_indent_tab = true);

    virtual syntax::action_return_t operator()(const syntax::block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::io_block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::parameter_declaration& pd) override;
    virtual syntax::action_return_t operator()(const syntax::parameter_list& pl, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::reference& r) override;
    virtual syntax::action_return_t operator()(const syntax::temporary& t, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::assignment_operator& op) override;
    virtual syntax::action_return_t operator()(const syntax::binary_operator& op) override;
    virtual syntax::action_return_t operator()(const syntax::conditional& c, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::expression_statement& es, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::expression_list& el, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::parentheses& p, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::function_call& fc, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::function_definition& fd, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::return_statement& rs, bool is_start = true) override;

    virtual std::wstring get_result() const override;

  protected:
    virtual syntax::action_return_t operator()(float f) override;
    virtual syntax::action_return_t operator()(double d) override;
    virtual syntax::action_return_t operator()(int i) override;
    virtual syntax::action_return_t operator()(unsigned int ui) override;
    virtual syntax::action_return_t operator()(bool b) override;

    virtual syntax::action_return_t get_default(bool is_start) override;

  private:
    void line_begin();
    void line_end(bool has_semi_colon = true);

    size_t _indent_count;
    std::wstringstream _ss;
    bool _is_indent_tab;
  };

  class output_introspector : public syntax::const_action_result<std::wstring>
  {
  public:
    output_introspector(core::qualifier_storage qualifier, core::semantic_pair semantic) :
      _semantic(semantic._semantic),
      _semantic_index(semantic._index),
      _qualifier(qualifier) {}

    virtual syntax::action_return_t operator()(const syntax::io_block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;

    // Non-assignable
    output_introspector& operator=(output_introspector&&) = delete;
    output_introspector& operator=(const output_introspector&) = delete;

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
