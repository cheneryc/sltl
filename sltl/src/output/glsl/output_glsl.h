#pragma once

#include <output/output.h>

#include <core/semantic.h>

#include <map>


namespace sltl
{
namespace glsl
{
  // According to the glsl spec shaders that do not include a version directive will be treated as targeting version 1.10
  enum class output_version
  {
    none,
    v330
  };

  class output_glsl : public output
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
      layout_map_key(const layout_map_key& key);

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

    output_glsl(output_glsl&&) = default;
    output_glsl(core::shader_stage stage, output_version version = output_version::v330, detail::enum_flags<output_flags> flags = output_flags::flag_none);
    output_glsl(core::shader_stage stage, layout_manager&& manager, output_version version = output_version::v330, detail::enum_flags<output_flags> flags = output_flags::flag_none);

    // Non-copyable and non-assignable
    output_glsl(const output_glsl&) = delete;
    output_glsl& operator=(output_glsl&&) = delete;
    output_glsl& operator=(const output_glsl&) = delete;

    syntax::action_return_t operator()(const syntax::variable_declaration& vd, bool is_start = true) override;

  protected:
    std::wstring get_type_name(const language::type& type) const override;

  private:
    layout_manager _layout_manager;
  };

  // Overloaded bitwise operators make the detail::enum_flags helper class more useful
  detail::enum_flags<output_glsl::layout_flags> operator|(detail::enum_flags<output_glsl::layout_flags> lhs, output_glsl::layout_flags rhs);
  detail::enum_flags<output_glsl::layout_flags> operator&(detail::enum_flags<output_glsl::layout_flags> lhs, output_glsl::layout_flags rhs);
}
}
