#include "output.h"

#include "language.h"

#include <syntax/block.h>
#include <syntax/io_block.h>
#include <syntax/variable_declaration.h>
#include <syntax/reference.h>
#include <syntax/temporary.h>
#include <syntax/operator.h>
#include <syntax/operator_component_access.h>
#include <syntax/conditional.h>
#include <syntax/constructor_call.h>
#include <syntax/function_call.h>
#include <syntax/function_definition.h>
#include <syntax/parameter_declaration.h>
#include <syntax/intrinsic_call.h>
#include <syntax/intrinsic_declaration.h>

#include <type.h>
#include <traits.h>

#include <detail/numeric.h>

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

  std::wstring get_zero_initialization(const ns::language::type& type)
  {
    std::wstring value;

    typedef unsigned int uint_t;

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
        value = to_string(uint_t());
        break;
      case ns::language::id_bool:
        value = to_string(bool());
        break;
      default:
        assert((type.get_id() != ns::language::id_unknown) && (type.get_id() != ns::language::id_void));
    }

    return value;
  }
}

ns::output::output(core::shader_stage stage, detail::enum_flags<output_flags> flags) : _flags(flags), _indent_count(0U), _stage(stage)
{
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

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_declaration& pd)
{
  _ss << get_type_name(pd.get_type()) << L' ' << get_parameter_name(pd);

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
      _ss << get_type_name(type) << L'(';
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
      _ss << to_operator_unary_string(ou._operator_id);
    }

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    if(language::is_postfix_operator(ou._operator_id))
    {
      _ss << to_operator_unary_string(ou._operator_id);
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
        _ss << to_operator_binary_string(ob._operator_id);
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
          ss << to_component_string(idx);
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
    _ss << syntax::visit(*oca._accessor, fn);

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
    _ss << to_conditional_string(c._id);

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
    _ss << get_type_name(cc.get_type()) << L'(';
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
    _ss << get_type_name(fd.get_type()) << L' ' << fd._name << L'(';
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
    _ss << to_keyword_string(language::id_return) << L' ';
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
