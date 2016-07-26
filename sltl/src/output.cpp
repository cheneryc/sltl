#include "output.h"
#include "syntax/block.h"
#include "syntax/io_block.h"
#include "syntax/variable_declaration.h"
#include "syntax/reference.h"
#include "syntax/temporary.h"
#include "syntax/operator.h"
#include "syntax/conditional.h"
#include "syntax/function_call.h"
#include "syntax/function_definition.h"
#include "language.h"

#include <numeric>

#include <cmath>
#include <cassert>


namespace
{
  namespace ns = sltl;

  void output_indent(std::wstringstream& ss, const size_t indent_count, const bool is_indent_tab)
  {
    //TODO: this should be precomputed when _indent_count is changed for performance reasons
    for(size_t i = 0; i < indent_count; ++i)
    {
      ss << (is_indent_tab ? L"\t" : L"  ");
    }
  }

  bool is_variable_built_in(ns::core::semantic semantic)
  {
    return (semantic == ns::core::semantic::position) ||
           (semantic == ns::core::semantic::depth);
  }

  bool is_variable_built_in(const ns::syntax::variable_declaration& vd)
  {
    return is_variable_built_in(vd._semantic);
  }

  bool is_variable_omitted(const ns::syntax::variable_declaration& vd, ns::output::layout_manager& layout_manager)
  {
    const auto qualifier_storage = static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value;

    if(qualifier_storage != ns::core::qualifier_storage::default)
    {
      bool is_omitted = false;

      if(vd._semantic == ns::core::semantic::none)
      {
        auto& layout_map = layout_manager.get_layout_map(vd);

        if(layout_map.is_layout_enabled())
        {
          layout_map.set_location_next(layout_map.get_location_next(vd));
        }

        is_omitted = true;
      }
      else if(is_variable_built_in(vd))
      {
        is_omitted = true;
      }

      return is_omitted;
    }

    return false;
  }

  bool is_layout_flag_valid(const ns::syntax::variable_declaration& vd, ns::output::layout_flags flag)
  {
    const auto qualifier_storage = static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value;

    switch(qualifier_storage)
    {
      case ns::core::qualifier_storage::in:
        return (flag == ns::output::layout_flags::flag_in);
      case ns::core::qualifier_storage::out:
        return (flag == ns::output::layout_flags::flag_out);
      case ns::core::qualifier_storage::uniform:
        return (flag == ns::output::layout_flags::flag_uniform);
      default:
        return false;
    }
  }

  std::wstring to_built_in_string(ns::core::semantic semantic, ns::core::semantic_index_t index)
  {
    assert(is_variable_built_in(semantic));
    assert(index == 0);

    //TODO: validation that the built-in is of the correct type and used in the correct shader stage

    std::wstringstream ss;

    switch(semantic)
    {
    case ns::core::semantic::position:
      //TODO: if the variable is vertex shader output then this is gl_Position
      //TODO: if the variable is pixel shader input then this is gl_FragCoord
      //TODO: note that index must be zero for position semantic
      ss << L"gl_Position";
      break;
    case ns::core::semantic::depth:
      //TODO: only valid as a fragment shader output
      //TODO: note that index must be zero for depth semantic
      ss << L"gl_FragDepth";
      break;
    }

    if(index > 0)
    {
      ss << L'[' << index << L']';
    }

    return ss.str();
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

  std::wstring get_variable_name(const ns::syntax::variable_declaration& vd)
  {
    std::wstringstream ss;

    if(is_variable_built_in(vd))
    {
      ss << to_built_in_string(vd._semantic, vd._semantic_index);
    }
    else
    {
      // Prepend the storage qualifier to the variable name to ensure it is globally unique
      if(auto qualifier = ns::language::to_qualifier_prefix_string(static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value))
      {
        ss << qualifier << L'_';
      }

      ss << ns::language::to_type_prefix_string(vd._type) << vd._name;
    }

    return ss.str();
  }

  std::wstring get_qualifier_layout(const ns::syntax::variable_declaration& vd, ns::output::layout_manager& layout_manager)
  {
    std::wstringstream ss;

    if(vd._semantic != ns::core::semantic::none)
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

  const wchar_t* get_qualifier_storage(const ns::syntax::variable_declaration& vd)
  {
    if(is_variable_built_in(vd))
    {
      throw std::exception();//TODO: exception type and message
    }

    return ns::language::to_qualifier_string(static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value);
  }

  ns::detail::enum_flags<ns::output::layout_flags> get_default_layout_flags(ns::core::shader_stage stage)
  {
    ns::detail::enum_flags<ns::output::layout_flags> flags = ns::output::layout_flags::flag_none;

    switch(stage)
    {
      case ns::core::shader_stage::vertex:
        flags = ns::output::layout_flags::flag_out;
        break;
      case ns::core::shader_stage::geometry:
        flags = ns::output::layout_flags::flag_out |
                ns::output::layout_flags::flag_in;
        break;
      case ns::core::shader_stage::fragment:
        flags = ns::output::layout_flags::flag_in;
        break;
      default:
        assert(stage == ns::core::shader_stage::test);
        break;
    }

    return flags;
  }
}

// output definitions

ns::output::output(core::shader_stage stage, output_version version, bool is_indent_tab) : output(stage, layout_manager(get_default_layout_flags(stage)), version, is_indent_tab)
{
}

ns::output::output(core::shader_stage stage, layout_manager&& manager, output_version version, bool is_indent_tab) : _indent_count(0), _is_indent_tab(is_indent_tab), _stage(stage), _layout_manager(std::move(manager))
{
  if(const auto v_str = to_version_string(version))
  {
    _ss << v_str << std::endl;
  }
}

std::wstring ns::output::get_result() const
{
  return _ss.str();
}

ns::syntax::action_return_t ns::output::operator()(const syntax::block&, bool is_start)
{
  wchar_t brace;

  if(is_start)
  {
    output_indent(_ss, _indent_count++, _is_indent_tab);
    brace = L'{';
  }
  else
  {
    output_indent(_ss, --_indent_count, _is_indent_tab);
    brace = L'}';
  }

  _ss << brace << std::endl;

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::io_block&, bool is_start)
{
  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::variable_declaration& vd, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    if(!is_variable_omitted(vd, _layout_manager))
    {
      line_begin();

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

      _ss << language::to_type_string(vd._type) << L' ' << get_variable_name(vd);

      if(vd.has_initializer())
      {
        _ss << (vd.is_direct_initialized() ? L"(" : L" = ");
      }

      return_val = ns::syntax::action_return_t::step_in;
    }
    else
    {
      return_val = ns::syntax::action_return_t::step_over;
    }
  }
  else
  {
    if(!is_variable_omitted(vd, _layout_manager))
    {
      if(vd.has_initializer() && vd.is_direct_initialized())
      {
        _ss << L')';
      }

      line_end();
    }

    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_declaration&)
{
  assert(false); //TODO: implement this...
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_list& pl, bool is_start)
{
  assert(pl.begin() == pl.end()); //TODO: implement this...

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::reference& r)
{
  _ss << get_variable_name(r._declaration);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::temporary& t, bool is_start)
{
  if(is_start)
  {
    _ss << language::to_type_string(t._type) << L'(';
  }
  else
  {
    _ss << L')';
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::operator_unary& ou, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    if(language::is_prefix_operator(ou._operator_id))
    {
      _ss << language::to_operator_unary_string(ou._operator_id);
    }

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    if(language::is_postfix_operator(ou._operator_id))
    {
      _ss << language::to_operator_unary_string(ou._operator_id);
    }

    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::operator_binary& ob, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    bool is_continuing = false;

    if(ob._operand_lhs->apply_action(*this))
    {
      _ss << L' ';
      _ss << language::to_operator_binary_string(ob._operator_id);
      _ss << L' ';

      is_continuing = ob._operand_rhs->apply_action(*this);
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    return_val = (is_continuing ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::stop);
  }
  else
  {
    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::conditional& c, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    line_begin();
    _ss << language::to_conditional_string(c._id);

    bool is_continuing = true;

    if(const auto* condition = c.get_condition())
    {
      _ss << L'(';
      is_continuing = condition->apply_action(*this);

      if(is_continuing)
      {
        _ss << L')';
      }
    }

    if(is_continuing)
    {
      line_end(false);
    }

    if(const auto* statement = c.get_statement())
    {
      is_continuing = is_continuing && statement->apply_action(*this);
    }

    if(const auto* statement_else = c.get_statement_else())
    {
      is_continuing = is_continuing && statement_else->apply_action(*this);
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    return_val = (is_continuing ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::stop);
  }
  else
  {
    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::expression_statement&, bool is_start)
{
  if(is_start)
  {
    line_begin();
  }
  else
  {
    line_end();
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::expression_list& el, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    auto it = el.begin();
    const auto it_end = el.end();

    if(it != it_end)
    {
      while((*it)->apply_action(*this) && (++it != it_end))
      {
        _ss << L", ";
      }
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    return_val = ((it == it_end) ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::stop);
  }
  else
  {
    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parentheses&, bool is_start)
{
  if(is_start)
  {
    _ss << L'(';
  }
  else
  {
    _ss << L')';
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::function_call& fc, bool is_start)
{
  if(is_start)
  {
    _ss << fc.get_function_name() << L'(';
  }
  else
  {
    _ss << L')';
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::function_definition& fd, bool is_start)
{
  if(is_start)
  {
    line_begin();
    _ss << language::to_type_string(fd._type_return) << L' ' << fd._name << L'(';
  }
  else
  {
    _ss << L')';
    line_end(false);
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::return_statement&, bool is_start)
{
  if(is_start)
  {
    line_begin();
    _ss << language::to_keyword_string(language::id_return) << L' ';
  }
  else
  {
    line_end();
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(float f)
{
  float integral;

  // Output a decimal point when the float has no fractional part, otherwise appending 'f' will result in a syntax error
  if(std::modf(f, &integral) == 0.0f)
  {
    _ss << f << L".0f";
  }
  else
  {
    _ss << f << L'f';
  }

  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(double d)
{
  double integral;

  // Output a decimal point when the double has no fractional part
  if(std::modf(d, &integral) == 0.0)
  {
    _ss << d << L".0";
  }
  else
  {
    _ss << d;
  }

  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(int i)
{
  _ss << i;
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(unsigned int ui)
{
  _ss << ui << L'U';
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(bool b)
{
  _ss << std::boolalpha << b;
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::get_default(bool is_start)
{
  assert(false);

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

void ns::output::line_begin()
{
  output_indent(_ss, _indent_count, _is_indent_tab);
}

void ns::output::line_end(bool has_semi_colon)
{
  if(has_semi_colon)
  {
    _ss << L';';
  }

  _ss << std::endl;
}

// output_introspector definitions

ns::output_introspector::output_introspector(core::shader_stage, core::qualifier_storage qualifier, core::semantic_pair semantic) :
  _semantic(semantic._semantic),
  _semantic_index(semantic._index),
  _qualifier(qualifier)
{
}

ns::syntax::action_return_t ns::output_introspector::operator()(const syntax::io_block& iob, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    return_val = ((iob._qualifier == _qualifier) ?
      ns::syntax::action_return_t::step_in :
      ns::syntax::action_return_t::step_over);
  }
  else
  {
    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_introspector::operator()(const syntax::variable_declaration& vd, bool is_start)
{
  ns::syntax::action_return_t return_val = (is_start ?
    ns::syntax::action_return_t::step_over :
    ns::syntax::action_return_t::step_out);

  if(is_start)
  {
    const auto qualifier_storage = static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value;

    if((qualifier_storage == _qualifier) &&
       (vd._semantic == _semantic) &&
       (vd._semantic_index == _semantic_index))
    {
      _name = get_variable_name(vd); return_val = ns::syntax::action_return_t::stop;
    }
  }

  return return_val;
}

std::wstring ns::output_introspector::get_result() const
{
  return _name;
}

ns::syntax::action_return_t ns::output_introspector::get_default(bool is_start)
{
  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

// output::layout_map_key definitions

ns::output::layout_map_key::layout_map_key(const ns::syntax::variable_declaration& vd) : _s(vd._semantic), _idx(vd._semantic_index)
{
  const auto qualifier_storage = static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value;

  assert(qualifier_storage == ns::core::qualifier_storage::in ||
         qualifier_storage == ns::core::qualifier_storage::out ||
         qualifier_storage == ns::core::qualifier_storage::uniform);
}

bool ns::output::layout_map_key::operator<(const ns::output::layout_map_key& rhs) const
{
  return ((_s < rhs._s) || (!(rhs._s < _s) && (_idx < rhs._idx)));
}

// output::layout_map definitions

ns::output::layout_map::layout_map(layout_flags flag) : _flag(flag), _location_next(0U)
{
}

ns::output::layout_map::layout_map(layout_map&& map) : _flag(map._flag), _location_next(map._location_next), _location_map(std::move(map._location_map))
{
}

std::pair<ns::output::layout_index_t, bool> ns::output::layout_map::insert(const syntax::variable_declaration& vd)
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

bool ns::output::layout_map::is_layout_enabled() const
{
  return (_flag != layout_flags::flag_none);
}

bool ns::output::layout_map::is_layout_qualified(const syntax::variable_declaration& vd) const
{
  // Ensure the storage qualifier matches the map's flag and that the semantic isn't 'none'
  if(!is_layout_flag_valid(vd, _flag) || (vd._semantic == core::semantic::none))
  {
    throw std::exception();//TODO: exception type and message
  }

  return (_location_map.find(layout_map_key(vd)) != _location_map.end());
}

void ns::output::layout_map::set_location_next(layout_index_t location)
{
  _location_next = location;
}

ns::output::layout_index_t ns::output::layout_map::get_location(const syntax::variable_declaration& vd) const
{
  // Ensure the storage qualifier matches the map's flag and that the semantic isn't 'none'
  if(!is_layout_flag_valid(vd, _flag) || (vd._semantic == core::semantic::none))
  {
    throw std::exception();//TODO: exception type and message
  }

  // Throws std::out_of_range if the key doesn't exist in the map
  return _location_map.at(layout_map_key(vd));
}

ns::output::layout_index_t ns::output::layout_map::get_location_next(const syntax::variable_declaration& vd) const
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
  // The matrix type and first dimension value are considered as if they were a
  // vector type. This value is then multiplied by each subsequent dimension value.

  //TODO: according to the glsl 4.4 spec this slot size doubling isn't applied to vertex shader input. Test this!
  const layout_index_t slots = (((vd._type._id == sltl::language::type_id::id_double) && (vd._type.front() > 2U)) ? 2U : 1U);

  return _location_next + std::accumulate(std::next(vd._type.cbegin()), vd._type.cend(), slots, [](layout_index_t lhs, layout_index_t rhs)
  {
    return (lhs * rhs);
  });
}

// output::layout_manager definitions

ns::output::layout_manager::layout_manager(detail::enum_flags<layout_flags> flags) :
  _layout_in(flags.has_flag<layout_flags::flag_in>() ? layout_flags::flag_in : layout_flags::flag_none),
  _layout_out(flags.has_flag<layout_flags::flag_out>() ? layout_flags::flag_out : layout_flags::flag_none),
  _layout_uniform(flags.has_flag<layout_flags::flag_uniform>() ? layout_flags::flag_uniform : layout_flags::flag_none)
{
}

ns::output::layout_manager::layout_manager(layout_manager&& manager) :
  _layout_in(std::move(manager._layout_in)),
  _layout_out(std::move(manager._layout_out)),
  _layout_uniform(std::move(manager._layout_uniform))
{
}

ns::output::layout_map& ns::output::layout_manager::get_layout_map(const syntax::variable_declaration& vd)
{
  const auto qualifier_storage = static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value;

  switch(qualifier_storage)
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

ns::detail::enum_flags<ns::output::layout_flags> ns::operator|(ns::detail::enum_flags<output::layout_flags> lhs, ns::output::layout_flags rhs)
{
  return lhs |= rhs;
}

ns::detail::enum_flags<ns::output::layout_flags> ns::operator&(ns::detail::enum_flags<output::layout_flags> lhs, ns::output::layout_flags rhs)
{
  return lhs &= rhs;
}
