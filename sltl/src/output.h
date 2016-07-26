#pragma once

#include "syntax/action.h"

#include "core/qualifier.h"
#include "core/semantic.h"
#include "core/shader_stage.h"

#include "detail/enum_flags.h"

#include <map>
#include <string>
#include <sstream>


namespace sltl
{
  // According to the glsl spec shaders that do not include a version directive will be treated as targeting version 1.10
  enum class output_version
  {
    none,
    v330
  };

  class output : public syntax::const_action_result<std::wstring>
  {
  public:
    enum class layout_flags : unsigned int
    {
      flag_none    = 0x0,
      flag_in      = 0x1,
      flag_out     = flag_in  << 1,
      flag_uniform = flag_out << 1
    };

    typedef unsigned int layout_index_t;

    class layout_map_key
    {
    public:
      layout_map_key(const syntax::variable_declaration& vd);

      // Non-assignable
      layout_map_key& operator=(layout_map_key&&) = delete;
      layout_map_key& operator=(const layout_map_key&) = delete;

      bool operator<(const layout_map_key& rhs) const;

    private:
      const core::semantic _s;
      const core::semantic_index_t _idx;
    };

    class layout_map
    {
    public:
      layout_map(layout_flags flag);
      layout_map(layout_map&& map);

      // Non-copyable and non-assignable
      layout_map(const layout_map&) = delete;
      layout_map& operator=(layout_map&&) = delete;
      layout_map& operator=(const layout_map&) = delete;

      //TODO: if the map was pre-populated then a successful insertion shouldn't be possible - should throw in this situation
      std::pair<layout_index_t, bool> insert(const syntax::variable_declaration& vd);

      bool is_layout_enabled() const;
      bool is_layout_qualified(const syntax::variable_declaration& vd) const;

      void set_location_next(layout_index_t location);

      layout_index_t get_location(const syntax::variable_declaration& vd) const;
      layout_index_t get_location_next(const syntax::variable_declaration& vd) const;

      const layout_flags _flag;

    private:
      layout_index_t _location_next;
      std::map<layout_map_key, layout_index_t> _location_map;
    };

    class layout_manager
    {
    public:
      //TODO: need a way to pre-populate layout_map(s) via io::block(s)
      layout_manager(detail::enum_flags<layout_flags> flags);
      layout_manager(layout_manager&& manager);

      // Non-copyable and non-assignable
      layout_manager(const layout_manager&) = delete;
      layout_manager& operator=(layout_manager&&) = delete;
      layout_manager& operator=(const layout_manager&) = delete;

      layout_map& get_layout_map(const syntax::variable_declaration& vd);

    private:
      layout_map _layout_in;
      layout_map _layout_out;
      layout_map _layout_uniform;
    };

    output(core::shader_stage stage, output_version version = output_version::v330, bool is_indent_tab = true);
    output(core::shader_stage stage, layout_manager&& manager, output_version version = output_version::v330, bool is_indent_tab = true);

    // Non-copyable and non-assignable
    output(const output&) = delete;
    output& operator=(output&&) = delete;
    output& operator=(const output&) = delete;

    virtual syntax::action_return_t operator()(const syntax::block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::io_block&, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::parameter_declaration& pd) override;
    virtual syntax::action_return_t operator()(const syntax::parameter_list& pl, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::reference& r) override;
    virtual syntax::action_return_t operator()(const syntax::temporary& t, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::operator_unary& ou, bool is_start = true) override;
    virtual syntax::action_return_t operator()(const syntax::operator_binary& ob, bool is_start = true) override;
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
    const core::shader_stage _stage;

    layout_manager _layout_manager;
  };

  class output_introspector : public syntax::const_action_result<std::wstring>
  {
  public:
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

  // Overloaded bitwise operators make the detail::enum_flags helper class more useful
  detail::enum_flags<output::layout_flags> operator|(detail::enum_flags<output::layout_flags> lhs, output::layout_flags rhs);
  detail::enum_flags<output::layout_flags> operator&(detail::enum_flags<output::layout_flags> lhs, output::layout_flags rhs);
}
