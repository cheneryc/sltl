#include "output.h"
#include "syntax/block.h"
#include "syntax/io_block.h"
#include "syntax/variable_declaration.h"
#include "syntax/reference.h"
#include "syntax/temporary.h"
#include "syntax/operator.h"
#include "syntax/operator_component_access.h"
#include "syntax/conditional.h"
#include "syntax/constructor_call.h"
#include "syntax/function_call.h"
#include "syntax/function_definition.h"
#include "syntax/intrinsic_call.h"
#include "syntax/intrinsic_declaration.h"
#include "language.h"
#include "traits.h"

#include "detail/numeric.h"

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

  bool is_variable_built_in(ns::core::semantic semantic, ns::core::semantic_index_t semantic_index)
  {
    bool is_built_in = false;

    if(semantic == ns::core::semantic::system)
    {
      const auto semantic_system_pair = ns::core::detail::to_semantic_system_pair(semantic_index);

      switch(semantic_system_pair.first)
      {
        // Only a built-in variable if the semantic is position or depth
        // Only a built-in variable if the index is zero
        case ns::core::semantic_system::position:
        case ns::core::semantic_system::depth:
          is_built_in = !semantic_system_pair.second;
          break;
      }
    }

    return is_built_in;
  }

  bool is_variable_built_in(const ns::syntax::variable_declaration& vd)
  {
    return is_variable_built_in(vd._semantic, vd._semantic_index);
  }

  bool is_variable_omitted(const ns::syntax::variable_declaration& vd, ns::output::layout_manager& layout_manager)
  {
    if(vd._qualifier != ns::core::qualifier_storage::default)
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
    switch(vd._qualifier)
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

  template<typename T>
  auto to_string(T t) -> typename std::enable_if<std::is_floating_point<T>::value, std::wstring>::type
  {
    std::wstringstream ss;

    // Output a decimal point when the floating point value has no fractional part
    if(ns::detail::has_fractional_part(t))
    {
      ss << t;
    }
    else
    {
      ss << t << L".0";
    }

    return ss.str();
  }

  std::wstring to_string(float f)
  {
    return to_string<float>(f) + L'f';
  }

  std::wstring to_string(double d)
  {
    return to_string<double>(d) + L"lf";
  }

  std::wstring to_string(int i)
  {
    return std::to_wstring(i);
  }

  std::wstring to_string(unsigned int ui)
  {
    return std::to_wstring(ui) + L'U';
  }

  std::wstring to_string(bool b)
  {
    std::wstringstream ss;
    ss << std::boolalpha << b;
    return ss.str();
  }

  const wchar_t* to_intrinsic_string(ns::core::intrinsic intrinsic)
  {
    switch(intrinsic)
    {
      case ns::core::intrinsic::dot:
        return L"dot";
      case ns::core::intrinsic::normalize:
        return L"normalize";
      case ns::core::intrinsic::clamp:
        return L"clamp";
      case ns::core::intrinsic::lerp:
        return L"mix";
      case ns::core::intrinsic::pow:
        return L"pow";
    }

    return nullptr;
  }

  const wchar_t* to_intrinsic_operator_string(const ns::syntax::operator_binary& ob)
  {
    const ns::language::type& t = ob.get_type();
    const ns::language::type_id t_id = t.get_id();

    switch(ob._operator_id)
    {
      case ns::language::id_element_wise_eq:
        assert(t.get_dimensions().is_vector());
        return L"equal";
      case ns::language::id_element_wise_ne:
        assert(t.get_dimensions().is_vector());
        return L"notEqual";
      case ns::language::id_element_wise_lt:
        assert(t.get_dimensions().is_vector());
        assert(t_id != ns::language::id_bool);
        return L"lessThan";
      case ns::language::id_element_wise_lt_eq:
        assert(t.get_dimensions().is_vector());
        assert(t_id != ns::language::id_bool);
        return L"lessThanEqual";
      case ns::language::id_element_wise_gt:
        assert(t.get_dimensions().is_vector());
        assert(t_id != ns::language::id_bool);
        return L"greaterThan";
      case ns::language::id_element_wise_gt_eq:
        assert(t.get_dimensions().is_vector());
        assert(t_id != ns::language::id_bool);
        return L"greaterThanEqual";
      case ns::language::id_element_wise_multiplication:
        return t.get_dimensions().is_matrix() ? L"matrixCompMult" : nullptr;
    }

    return nullptr;
  }

  std::wstring to_built_in_string(ns::core::semantic semantic, ns::core::semantic_index_t semantic_index)
  {
    assert(is_variable_built_in(semantic, semantic_index));

    const auto semantic_system_pair = ns::core::detail::to_semantic_system_pair(semantic_index);

    //TODO: validation that the built-in is of the correct type and used in the correct shader stage

    std::wstringstream ss;

    switch(semantic_system_pair.first)
    {
    case ns::core::semantic_system::position:
      //TODO: if the variable is vertex shader output then this is gl_Position
      //TODO: if the variable is pixel shader input then this is gl_FragCoord
      //TODO: note that index must be zero for position semantic
      ss << L"gl_Position";
      break;
    case ns::core::semantic_system::depth:
      //TODO: only valid as a fragment shader output
      //TODO: note that index must be zero for depth semantic
      ss << L"gl_FragDepth";
      break;
    }

    // Revisit this once built-in variables other than gl_Position & gl_FragDepth are supported
    assert(semantic_system_pair.second == 0);

    if(semantic_system_pair.second > 0)
    {
      ss << L'[' << semantic_system_pair.second << L']';
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

  std::wstring get_type_name(const ns::language::type& type, ns::detail::enum_flags<ns::output_flags> flags)
  {
    return ns::language::to_type_string(flags.has_flag<ns::output_flags::flag_transpose_type>() ? type.transpose() : type);
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
      if(auto qualifier = ns::language::to_qualifier_prefix_string(vd._qualifier))
      {
        ss << qualifier << L'_';
      }

      ss << ns::language::to_type_prefix_string(vd.get_type()) << vd._name;
    }

    return ss.str();
  }

  std::wstring get_parameter_name(const ns::syntax::parameter_declaration& pd)
  {
    std::wstringstream ss(ns::language::to_parameter_prefix_string(pd._qualifier), std::ios::in | std::ios::out | std::ios::ate);

    ss << L'_';
    ss << ns::language::to_type_prefix_string(pd.get_type());
    ss << pd._name;

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

    return ns::language::to_qualifier_string(vd._qualifier);
  }

  std::wstring get_zero_initialization(const ns::language::type& type)
  {
    std::wstring value;

    switch(type.get_id())
    {
      case ns::language::id_float:
        value = to_string(float());
        break;
      case ns::language::id_double:
        value = to_string(double());
        break;
      case ns::language::id_int:
        value = to_string(int());
        break;
      case ns::language::id_uint:
        value = to_string(unsigned int());
        break;
      case ns::language::id_bool:
        value = to_string(bool());
        break;
      default:
        assert((type.get_id() != ns::language::id_unknown) && (type.get_id() != ns::language::id_void));
    }

    return value;
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

ns::output::output(core::shader_stage stage, output_version version, detail::enum_flags<output_flags> flags) : output(stage, layout_manager(get_default_layout_flags(stage)), version, flags)
{
}

ns::output::output(core::shader_stage stage, layout_manager&& manager, output_version version, detail::enum_flags<output_flags> flags) : _indent_count(0), _flags(flags), _stage(stage), _layout_manager(std::move(manager))
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
    output_indent(_ss, _indent_count++, !_flags.has_flag<output_flags::flag_indent_space>());
    brace = L'{';
  }
  else
  {
    output_indent(_ss, --_indent_count, !_flags.has_flag<output_flags::flag_indent_space>());
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

      _ss << get_type_name(vd.get_type(), _flags) << L' ' << get_variable_name(vd);

      if(vd.has_initializer())
      {
        _ss << L" = ";
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
      line_end();
    }

    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_declaration& pd)
{
  _ss << get_type_name(pd.get_type(), _flags) << L' ' << get_parameter_name(pd);

  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_list& pl, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    auto it = pl.begin();
    auto it_end = pl.end();

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

ns::syntax::action_return_t ns::output::operator()(const syntax::reference& r)
{
  if (auto vd = dynamic_cast<const ns::syntax::variable_declaration*>(&r._declaration))
  {
    _ss << get_variable_name(*vd);
  }

  if(auto pd = dynamic_cast<const ns::syntax::parameter_declaration*>(&r._declaration))
  {
    _ss << get_parameter_name(*pd);
  }

  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::temporary& t, bool is_start)
{
  const syntax::expression* initializer = t.get_initializer();

  if(is_start)
  {
    const language::type type = t.get_type();

    if(!initializer || !sltl::is_type<syntax::constructor_call>(initializer))
    {
      _ss << get_type_name(type, _flags) << L'(';
    }

    if(!initializer)
    {
      _ss << get_zero_initialization(type);
    }
  }
  else
  {
    if(!initializer || !sltl::is_type<syntax::constructor_call>(initializer))
    {
      _ss << L')';
    }
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
    const auto* const intrinsic_op = to_intrinsic_operator_string(ob);

    if(intrinsic_op)
    {
      _ss << intrinsic_op << L'(';
    }

    bool is_continuing = false;

    if(ob._operand_lhs->apply_action(*this))
    {
      if(intrinsic_op)
      {
        _ss << L", ";
      }
      else
      {
        _ss << L' ';
        _ss << language::to_operator_binary_string(ob._operator_id);
        _ss << L' ';
      }

      is_continuing = ob._operand_rhs->apply_action(*this);
    }

    if(intrinsic_op)
    {
      _ss << L')';
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

ns::syntax::action_return_t ns::output::operator()(const syntax::operator_component_access& oca, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    struct
    {
      std::wstring operator()(const syntax::component_accessor_scalar& access) const
      {
        std::wstringstream ss;

        for(size_t i = 0; i < access._count; ++i)
        {
          ss << L'x';
        }

        return ss.str();
      }

      std::wstring operator()(const syntax::component_accessor_vector& access) const
      {
        const auto it_find = std::find(std::begin(access._indices), std::end(access._indices), syntax::component_accessor::_idx_default);

        std::wstringstream ss;

        std::for_each(std::begin(access._indices), it_find, [&ss](language::type_dimension_t idx)
        {
          ss << language::to_component_string(idx);
        });

        return ss.str();
      }

      std::wstring operator()(const syntax::component_accessor_matrix& access) const
      {
        std::wstringstream ss;

        ss << L'[';
        ss << access._idx_n;
        ss << L']';

        if(access._idx_m != syntax::component_accessor::_idx_default)
        {
          ss << L'[';
          ss << access._idx_m;
          ss << L']';
        }

        return ss.str();
      }
    } fn;

    _ss << L'.';
    _ss << syntax::component_accessor::visit(*oca._accessor, fn);

    return_val = syntax::action_return_t::step_out;
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

ns::syntax::action_return_t ns::output::operator()(const syntax::constructor_call& cc, bool is_start)
{
  if(is_start)
  {
    _ss << get_type_name(cc.get_type(), _flags) << L'(';
  }
  else
  {
    _ss << L')';
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
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
    auto it_end = el.end();

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
    _ss << get_type_name(fd.get_type(), _flags) << L' ' << fd._name << L'(';
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

ns::syntax::action_return_t ns::output::operator()(const syntax::intrinsic_call& ic, bool is_start)
{
  if(is_start)
  {
    _ss << ::to_intrinsic_string(ic.get_intrinsic()) << L'(';
  }
  else
  {
    _ss << L')';
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::intrinsic_declaration& id, bool is_start)
{
  // The initial return value is 'step_over' as there is no need to traverse any child nodes
  return is_start ? ns::syntax::action_return_t::step_over :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(float f)
{
  _ss << to_string(f);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(double d)
{
  _ss << to_string(d);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(int i)
{
  _ss << to_string(i);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(unsigned int ui)
{
  _ss << to_string(ui);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(bool b)
{
  _ss << to_string(b);
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
  output_indent(_ss, _indent_count, !_flags.has_flag<output_flags::flag_indent_space>());
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
    if((vd._qualifier == _qualifier) &&
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

// output_matrix_order definitions

ns::output_matrix_order::output_matrix_order(core::shader_stage)
{
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::operator_binary& ob, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    return_val = ns::syntax::action_return_t::step_in;

    if(ob._operator_id == language::id_matrix_multiplication)
    {
      const language::type type_lhs = ob._operand_lhs->get_type();
      const language::type type_rhs = ob._operand_rhs->get_type();

      // matrix-matrix and vector-matrix multiplication need converting from row-order to column-order
      if((type_lhs.get_dimensions().is_matrix() || type_lhs.get_dimensions().is_vector()) &&
         (type_rhs.get_dimensions().is_matrix() || type_rhs.get_dimensions().is_vector()))
      {
        // both operands cannot be vectors
        assert(!(type_lhs.get_dimensions().is_vector() &&
                 type_rhs.get_dimensions().is_vector()));

        ob.swap_operands();
      }
    }
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::operator_component_access& oca, bool is_start)
{
  //TODO: implement this...
  assert(false);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::variable_declaration& vd, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    const bool is_transposable = vd.has_type();

    if(is_transposable)
    {
      vd.set_type(vd.get_type().transpose());
    }

    return_val = is_transposable ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::step_in;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::temporary& t, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    const bool is_transposable = t.has_type();

    if(is_transposable)
    {
      t.set_type(t.get_type().transpose());
    }

    return_val = is_transposable ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::step_in;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::constructor_call& cc, bool is_start)
{
  if(is_start)
  {
    cc.set_type(cc.get_type().transpose());
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::function_definition& fd, bool is_start)
{
  if(is_start)
  {
    fd.set_type(fd.get_type().transpose());
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::get_default(bool is_start)
{
  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

// output::layout_map_key definitions

ns::output::layout_map_key::layout_map_key(const ns::syntax::variable_declaration& vd) : _s(vd._semantic), _idx(vd._semantic_index)
{
  assert(vd._qualifier == ns::core::qualifier_storage::in ||
         vd._qualifier == ns::core::qualifier_storage::out ||
         vd._qualifier == ns::core::qualifier_storage::uniform);
}

ns::output::layout_map_key::layout_map_key(const layout_map_key& key) : _s(key._s), _idx(key._idx)
{
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

ns::detail::enum_flags<ns::output::layout_flags> ns::operator|(ns::detail::enum_flags<output::layout_flags> lhs, ns::output::layout_flags rhs)
{
  return lhs |= rhs;
}

ns::detail::enum_flags<ns::output::layout_flags> ns::operator&(ns::detail::enum_flags<output::layout_flags> lhs, ns::output::layout_flags rhs)
{
  return lhs &= rhs;
}
