#include "output_glsl.h"

#include "glsl_language.h"
#include "glsl_convention.h"

#include <syntax/operator.h>
#include <syntax/variable_declaration.h>
#include <syntax/parameter_declaration.h>

#include <type.h>

#include <core/qualifier.h>


namespace
{
  namespace ns = sltl::glsl;

  bool is_variable_omitted(const sltl::syntax::variable_declaration& vd, ns::output_glsl::layout_manager& layout_manager)
  {
    if(vd._qualifier != sltl::core::qualifier_storage::none)
    {
      bool is_omitted = false;

      if(vd._semantic == sltl::core::semantic::none)
      {
        auto& layout_map = layout_manager.get_layout_map(vd);

        if(layout_map.is_layout_enabled())
        {
          layout_map.set_location_next(layout_map.get_location_next(vd));
        }

        is_omitted = true;
      }
      else if(ns::is_variable_built_in(vd))
      {
        is_omitted = true;
      }

      return is_omitted;
    }

    return false;
  }

  bool is_layout_flag_valid(const sltl::syntax::variable_declaration& vd, ns::output_glsl::layout_flags flag)
  {
    switch(vd._qualifier)
    {
      case sltl::core::qualifier_storage::in:
        return (flag == ns::output_glsl::layout_flags::flag_in);
      case sltl::core::qualifier_storage::out:
        return (flag == ns::output_glsl::layout_flags::flag_out);
      case sltl::core::qualifier_storage::uniform:
        return (flag == ns::output_glsl::layout_flags::flag_uniform);
      default:
        return false;
    }
  }

  const wchar_t* to_version_string(ns::output_version version)
  {
    switch(version)
    {
      case ns::output_version::v330:
        return L"#version 330";
      default:
        assert(version == ns::output_version::none);
    }

    return nullptr;
  }

  std::wstring get_qualifier_layout(const sltl::syntax::variable_declaration& vd, ns::output_glsl::layout_manager& layout_manager)
  {
    std::wstringstream ss;

    if(vd._semantic != sltl::core::semantic::none)
    {
      auto& layout_map = layout_manager.get_layout_map(vd);

      if(layout_map.is_layout_enabled())
      {
        const auto result = layout_map.insert(vd);

        ss << L"layout(location = ";
        ss << result.first;
        ss << L')';

        assert(result.second);
      }
    }

    return ss.str();
  }

  const wchar_t* get_qualifier_storage(const sltl::syntax::variable_declaration& vd)
  {
    if(ns::is_variable_built_in(vd))
    {
      throw std::exception();//TODO: exception type and message
    }

    return ns::to_qualifier_string(vd._qualifier);
  }

  sltl::detail::enum_flags<ns::output_glsl::layout_flags> get_default_layout_flags(sltl::core::shader_stage stage)
  {
    sltl::detail::enum_flags<ns::output_glsl::layout_flags> flags = ns::output_glsl::layout_flags::flag_none;

    switch(stage)
    {
      case sltl::core::shader_stage::vertex:
        flags = ns::output_glsl::layout_flags::flag_out;
        break;
      case sltl::core::shader_stage::geometry:
        flags = ns::output_glsl::layout_flags::flag_out |
                ns::output_glsl::layout_flags::flag_in;
        break;
      case sltl::core::shader_stage::fragment:
        flags = ns::output_glsl::layout_flags::flag_in;
        break;
      default:
        assert(stage == sltl::core::shader_stage::test);
        break;
    }

    return flags;
  }
}

// output_glsl definitions

ns::output_glsl::output_glsl(sltl::core::shader_stage stage, output_version version, sltl::detail::enum_flags<sltl::output_flags> flags) : output_glsl(stage, layout_manager(get_default_layout_flags(stage)), version, flags)
{
}

ns::output_glsl::output_glsl(sltl::core::shader_stage stage, layout_manager&& manager, output_version version, sltl::detail::enum_flags<sltl::output_flags> flags) : output(stage, flags), _layout_manager(std::move(manager))
{
  if(const auto v_str = to_version_string(version))
  {
    _ss << v_str << std::endl;
  }
}

sltl::syntax::action_return_t ns::output_glsl::operator()(const sltl::syntax::variable_declaration& vd, bool is_start)
{
  syntax::action_return_t return_val;

  if(is_start)
  {
    if(!is_variable_omitted(vd, _layout_manager))
    {
      _ss << get_indent(indent_t::current);

      const auto qualifier_layout = get_qualifier_layout(vd, _layout_manager);
      const auto qualifier_storage = get_qualifier_storage(vd);

      if(!qualifier_layout.empty())
      {
        _ss << qualifier_layout << L' ';
      }

      if(qualifier_storage)
      {
        _ss << qualifier_storage << L' ';
      }

      _ss << get_type_name(vd.get_type()) << L' ' << get_variable_name(vd);

      if(vd.has_initializer())
      {
        _ss << L" = ";
      }

      return_val = syntax::action_return_t::step_in;
    }
    else
    {
      return_val = syntax::action_return_t::step_over;
    }
  }
  else
  {
    if(!is_variable_omitted(vd, _layout_manager))
    {
      _ss << get_terminal_newline();
    }

    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

std::wstring ns::output_glsl::get_type_name(const sltl::language::type& type) const
{
  return to_type_string(_flags.has_flag<output_flags::flag_transpose_type>() ? type.transpose() : type);
}

std::wstring ns::output_glsl::get_variable_name(const sltl::syntax::variable_declaration& vd) const
{
  return sltl::glsl::get_variable_name(vd);
}

std::wstring ns::output_glsl::get_parameter_name(const sltl::syntax::parameter_declaration& pd) const
{
  std::wstringstream ss(to_parameter_prefix_string(pd._qualifier), std::ios::in | std::ios::out | std::ios::ate);

  ss << L'_';
  ss << to_type_prefix_string(pd.get_type());
  ss << pd._name;

  return ss.str();
}

const wchar_t* ns::output_glsl::to_intrinsic_string(sltl::core::intrinsic intrinsic) const
{
  switch(intrinsic)
  {
    case core::intrinsic::dot:
      return L"dot";
    case core::intrinsic::normalize:
      return L"normalize";
    case core::intrinsic::clamp:
      return L"clamp";
    case core::intrinsic::lerp:
      return L"mix";
    case core::intrinsic::pow:
      return L"pow";
  }

  return nullptr;
}

const wchar_t* ns::output_glsl::to_intrinsic_operator_string(const sltl::syntax::operator_binary& ob) const
{
  const language::type& t = ob.get_type();
  const language::type_id t_id = t.get_id();

  switch(ob._operator_id)
  {
    case language::id_element_wise_eq:
      assert(t.get_dimensions().is_vector());
      return L"equal";
    case language::id_element_wise_ne:
      assert(t.get_dimensions().is_vector());
      return L"notEqual";
    case language::id_element_wise_lt:
      assert(t.get_dimensions().is_vector());
      assert(t_id != language::id_bool);
      return L"lessThan";
    case language::id_element_wise_lt_eq:
      assert(t.get_dimensions().is_vector());
      assert(t_id != language::id_bool);
      return L"lessThanEqual";
    case language::id_element_wise_gt:
      assert(t.get_dimensions().is_vector());
      assert(t_id != language::id_bool);
      return L"greaterThan";
    case language::id_element_wise_gt_eq:
      assert(t.get_dimensions().is_vector());
      assert(t_id != language::id_bool);
      return L"greaterThanEqual";
    case language::id_element_wise_multiplication:
      return t.get_dimensions().is_matrix() ? L"matrixCompMult" : nullptr;
  }

  return nullptr;
}

// output_glsl::layout_map_key definitions

ns::output_glsl::layout_map_key::layout_map_key(const sltl::syntax::variable_declaration& vd) : _s(vd._semantic), _idx(vd._semantic_index)
{
  assert(vd._qualifier == core::qualifier_storage::in ||
         vd._qualifier == core::qualifier_storage::out ||
         vd._qualifier == core::qualifier_storage::uniform);
}

ns::output_glsl::layout_map_key::layout_map_key(const layout_map_key& key) : _s(key._s), _idx(key._idx)
{
}

bool ns::output_glsl::layout_map_key::operator<(const ns::output_glsl::layout_map_key& rhs) const
{
  return ((_s < rhs._s) || (!(rhs._s < _s) && (_idx < rhs._idx)));
}

// output_glsl::layout_map definitions

ns::output_glsl::layout_map::layout_map(layout_flags flag) : _flag(flag), _location_next(0U)
{
}

ns::output_glsl::layout_map::layout_map(layout_map&& map) : _flag(map._flag), _location_next(map._location_next), _location_map(std::move(map._location_map))
{
}

std::pair<ns::output_glsl::layout_index_t, bool> ns::output_glsl::layout_map::insert(const sltl::syntax::variable_declaration& vd)
{
  // Ensure the storage qualifier matches the map's flag and that the semantic isn't 'none'
  if(!is_layout_flag_valid(vd, _flag) || (vd._semantic == core::semantic::none))
  {
    throw std::exception();//TODO: exception type and message
  }

  const auto result = _location_map.emplace(layout_map_key(vd), _location_next);

  // If an insertion occurred then increase the location index by an
  // increment that is determined using the variable_declaration's type
  if(result.second)
  {
    _location_next = get_location_next(vd);
  }

  return std::make_pair(result.first->second, result.second);
}

bool ns::output_glsl::layout_map::is_layout_enabled() const
{
  return (_flag != layout_flags::flag_none);
}

bool ns::output_glsl::layout_map::is_layout_qualified(const sltl::syntax::variable_declaration& vd) const
{
  // Ensure the storage qualifier matches the map's flag and that the semantic isn't 'none'
  if(!is_layout_flag_valid(vd, _flag) || (vd._semantic == core::semantic::none))
  {
    throw std::exception();//TODO: exception type and message
  }

  return (_location_map.find(layout_map_key(vd)) != _location_map.end());
}

void ns::output_glsl::layout_map::set_location_next(layout_index_t location)
{
  _location_next = location;
}

ns::output_glsl::layout_index_t ns::output_glsl::layout_map::get_location(const sltl::syntax::variable_declaration& vd) const
{
  // Ensure the storage qualifier matches the map's flag and that the semantic isn't 'none'
  if(!is_layout_flag_valid(vd, _flag) || (vd._semantic == core::semantic::none))
  {
    throw std::exception();//TODO: exception type and message
  }

  // Throws std::out_of_range if the key doesn't exist in the map
  return _location_map.at(layout_map_key(vd));
}

ns::output_glsl::layout_index_t ns::output_glsl::layout_map::get_location_next(const sltl::syntax::variable_declaration& vd) const
{
  // Ensure the storage qualifier matches the map's flag
  if(!is_layout_flag_valid(vd, _flag))
  {
    throw std::exception();//TODO: exception type and message
  }

  // Types: scalar, vector
  // A single location index, or slot, is consumed by all scalar and vector
  // types other than dvec3 or dvec4 - which require two slots each.

  // Types: matrix
  // The matrix component type and 'm' dimension value are considered as if
  // they were a vector type. This value is then multiplied by the 'n'
  // dimension value.

  //TODO: according to the glsl 4.4 spec this slot size doubling isn't applied to vertex shader input. Test this!

  language::type_dimension_t d1 = 1U;
  language::type_dimension_t d2 = 1U;

  const language::type type = vd.get_type();

  const language::type_id id = type.get_id();
  const language::type_dimensions& dimensions = type.get_dimensions();

  if(dimensions.is_matrix())
  {
    d1 = dimensions.m();
    d2 = dimensions.n();
  }
  else if(dimensions.is_vector())
  {
    d1 = (language::is_row_vector(dimensions) ? dimensions.n() : dimensions.m());
  }

  return _location_next + ((((id == language::id_double) && (d1 > 2U)) ? 2U : 1U) * d2);
}

// output::layout_manager definitions

ns::output_glsl::layout_manager::layout_manager(sltl::detail::enum_flags<layout_flags> flags) :
  _layout_in(flags.has_flag<layout_flags::flag_in>() ? layout_flags::flag_in : layout_flags::flag_none),
  _layout_out(flags.has_flag<layout_flags::flag_out>() ? layout_flags::flag_out : layout_flags::flag_none),
  _layout_uniform(flags.has_flag<layout_flags::flag_uniform>() ? layout_flags::flag_uniform : layout_flags::flag_none)
{
}

ns::output_glsl::layout_manager::layout_manager(layout_manager&& manager) :
  _layout_in(std::move(manager._layout_in)),
  _layout_out(std::move(manager._layout_out)),
  _layout_uniform(std::move(manager._layout_uniform))
{
}

ns::output_glsl::layout_map& ns::output_glsl::layout_manager::get_layout_map(const sltl::syntax::variable_declaration& vd)
{
  switch(vd._qualifier)
  {
    case core::qualifier_storage::in:
      return _layout_in;
      break;
    case core::qualifier_storage::out:
      return _layout_out;
      break;
    case core::qualifier_storage::uniform:
      return _layout_uniform;
      break;
    default:
      throw std::exception();//TODO: exception type and message...
  }
}

// non-member definitions

sltl::detail::enum_flags<ns::output_glsl::layout_flags> ns::operator|(sltl::detail::enum_flags<output_glsl::layout_flags> lhs, ns::output_glsl::layout_flags rhs)
{
  return lhs |= rhs;
}

sltl::detail::enum_flags<ns::output_glsl::layout_flags> ns::operator&(sltl::detail::enum_flags<output_glsl::layout_flags> lhs, ns::output_glsl::layout_flags rhs)
{
  return lhs &= rhs;
}
