#include "output.h"

#include "language.h"

#include <syntax/block.h>
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

#include <detail/numeric.h>
#include <detail/type_traits.h>

#include <cassert>


namespace
{
  namespace ns = sltl;

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

ns::output::output(core::shader_stage stage, detail::enum_flags<output_flags> flags) : _flags(flags), _stage(stage), _indent_count(0U)
{
}

std::wstring ns::output::get_result() const
{
  return _ss.str();
}

ns::syntax::action_return_t ns::output::operator()(const syntax::block&, bool is_start)
{
  syntax::action_return_t return_val;

  if(is_start)
  {
    _ss << get_indent(indent_t::increase);
    operator()(language::bracket_tag<language::id_brace>(), true);
    _ss << get_newline();

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    _ss << get_indent(indent_t::decrease);
    operator()(language::bracket_tag<language::id_brace>(), false);
    _ss << get_newline();

    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::io_block&, bool is_start)
{
  syntax::action_return_t return_val;

  if(is_start)
  {
    if(_flags.has_flag<output_flags::flag_extra_newlines>())
    {
      _ss << get_newline();
    }

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_declaration& pd)
{
  _ss << get_type_name(pd.get_type()) << L' ' << get_parameter_name(pd);

  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::parameter_list& pl, bool is_start)
{
  syntax::action_return_t return_val;

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
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::reference& r)
{
  if(auto vd = dynamic_cast<const syntax::variable_declaration*>(&r._declaration))
  {
    _ss << get_variable_name(*vd);
  }

  if(auto pd = dynamic_cast<const syntax::parameter_declaration*>(&r._declaration))
  {
    _ss << get_parameter_name(*pd);
  }

  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::temporary& t, bool is_start)
{
  const syntax::expression* initializer = t.get_initializer();

  if(is_start)
  {
    const language::type type = t.get_type();

    if(!initializer || !detail::is_type<syntax::constructor_call>(initializer))
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
    if(!initializer || !detail::is_type<syntax::constructor_call>(initializer))
    {
      _ss << L')';
    }
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::operator_unary& ou, bool is_start)
{
  syntax::action_return_t return_val;

  if(is_start)
  {
    if(language::is_prefix_operator(ou._operator_id))
    {
      _ss << to_operator_unary_string(ou._operator_id);
    }

    if(detail::is_type<syntax::operator_binary>(ou._operand.get()))
    {
      operator()(language::bracket_tag<language::id_parenthesis>(), true);
    }

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    if(detail::is_type<syntax::operator_binary>(ou._operand.get()))
    {
      operator()(language::bracket_tag<language::id_parenthesis>(), false);
    }

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
  syntax::action_return_t return_val;

  if(is_start)
  {
    auto fn_is_parentheses_required = [this](const syntax::expression* const exp)
    {
      bool is_parentheses_required = false;

      if(const auto* const ob = dynamic_cast<const syntax::operator_binary*>(exp))
      {
        is_parentheses_required = !to_intrinsic_operator_string(*ob);
      }

      return is_parentheses_required;
    };

    const auto* const intrinsic_op = to_intrinsic_operator_string(ob);

    if(intrinsic_op)
    {
      _ss << intrinsic_op << L'(';
    }
    else if(fn_is_parentheses_required(ob._operand_lhs.get()))
    {
      operator()(language::bracket_tag<language::id_parenthesis>(), true);
    }

    bool is_continuing;

    if(!(is_continuing = ob._operand_lhs->apply_action(*this)))
    {
      goto stop_label;
    }

    if(intrinsic_op)
    {
      _ss << L", ";
    }
    else
    {
      if(fn_is_parentheses_required(ob._operand_lhs.get()))
      {
        operator()(language::bracket_tag<language::id_parenthesis>(), false);
      }

      _ss << L' ' << to_operator_binary_string(ob._operator_id) << L' ';

      if(fn_is_parentheses_required(ob._operand_rhs.get()))
      {
        operator()(language::bracket_tag<language::id_parenthesis>(), true);
      }
    }

    if(!(is_continuing = ob._operand_rhs->apply_action(*this)))
    {
      goto stop_label;
    }

    if(intrinsic_op)
    {
      _ss << L')';
    }
    else if(fn_is_parentheses_required(ob._operand_rhs.get()))
    {
      operator()(language::bracket_tag<language::id_parenthesis>(), false);
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    stop_label: return_val = (is_continuing ?
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::operator_component_access& oca, bool is_start)
{
  syntax::action_return_t return_val;

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
        const auto it_find  = std::find(
          access.begin(),
          access.end(),
          syntax::component_accessor::_idx_default);

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
  syntax::action_return_t return_val;

  if(is_start)
  {
    if(_flags.has_flag<output_flags::flag_extra_newlines>())
    {
      _ss << get_newline();
    }

    _ss << get_indent(indent_t::current);
    _ss << to_conditional_string(c._id);

    bool is_continuing = true;

    if(const auto* condition = c.get_condition())
    {
      operator()(language::bracket_tag<language::id_parenthesis>(), true);

      if(!(is_continuing = condition->apply_action(*this)))
      {
        goto stop_label;
      }

      operator()(language::bracket_tag<language::id_parenthesis>(), false);
    }

    _ss << get_newline();

    if(const auto* statement = c.get_statement())
    {
      is_continuing = is_continuing && statement->apply_action(*this);
    }

    if(const auto* statement_else = c.get_statement_else())
    {
      is_continuing = is_continuing && statement_else->apply_action(*this);
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    stop_label: return_val = (is_continuing ?
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::constructor_call& cc, bool is_start)
{
  if(is_start)
  {
    _ss << get_type_name(cc.get_type());
  }

  operator()(language::bracket_tag<language::id_parenthesis>(), is_start);

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::expression_statement&, bool is_start)
{
  if(is_start)
  {
    _ss << get_indent(indent_t::current);
  }
  else
  {
    _ss << get_terminal_newline();
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::expression_list& el, bool is_start)
{
  syntax::action_return_t return_val;

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
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::function_call& fc, bool is_start)
{
  if(is_start)
  {
    _ss << fc.get_function_name();
  }

  operator()(language::bracket_tag<language::id_parenthesis>(), is_start);

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::function_definition& fd, bool is_start)
{
  syntax::action_return_t return_val;

  if(is_start)
  {
    if(_flags.has_flag<output_flags::flag_extra_newlines>())
    {
      _ss << get_newline();
    }

    _ss << get_indent(indent_t::current);
    _ss << get_type_name(fd.get_type()) << L' ' << fd._name << L'(';

    bool is_continuing;

    if(!(is_continuing = fd.get_params().apply_action(*this)))
    {
      goto stop_label;
    }

    _ss << L')' << get_newline();

    if(!(is_continuing = fd.get_body().apply_action(*this)))
    {
      goto stop_label;
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    stop_label: return_val = (is_continuing ?
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
  }
  else
  {
    return_val = syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::return_statement&, bool is_start)
{
  if(is_start)
  {
    if(_flags.has_flag<output_flags::flag_extra_newlines>())
    {
      _ss << get_newline();
    }

    _ss << get_indent(indent_t::current);
    _ss << to_keyword_string(language::id_return) << L' ';
  }
  else
  {
    _ss << get_terminal_newline();
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::intrinsic_call& ic, bool is_start)
{
  if(is_start)
  {
    _ss << to_intrinsic_string(ic.get_intrinsic());
  }

  operator()(language::bracket_tag<language::id_parenthesis>(), is_start);

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(const syntax::intrinsic_declaration& id, bool is_start)
{
  // The initial return value is 'step_over' as there is no need to traverse any child nodes
  return is_start ? syntax::action_return_t::step_over :
                    syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(float f)
{
  _ss << to_string(f);
  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(double d)
{
  _ss << to_string(d);
  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(int i)
{
  _ss << to_string(i);
  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(unsigned int ui)
{
  _ss << to_string(ui);
  return syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output::operator()(bool b)
{
  _ss << to_string(b);
  return syntax::action_return_t::step_out;
}

void ns::output::operator()(language::bracket_tag<language::id_brace>, bool is_start)
{
  if(is_start)
  {
    _ss << L'{';
  }
  else
  {
    _ss << L'}';
  }
}

void ns::output::operator()(language::bracket_tag<language::id_parenthesis>, bool is_start)
{
  if(is_start)
  {
    _ss << L'(';
  }
  else
  {
    _ss << L')';
  }
}

ns::syntax::action_return_t ns::output::get_default(bool is_start)
{
  assert(false);

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

std::wstring ns::output::get_indent(indent_t indent_op)
{
  size_t indent_count = _indent_count;

  switch(indent_op)
  {
    case indent_t::increase:
      indent_count = _indent_count++; // Increase the indent after beginning the line
      break;
    case indent_t::decrease:
      indent_count = --_indent_count; // Decrease the indent before beginning the line
      break;
  }

  wchar_t indent_character = L' ';

  if(_flags.has_flag<output_flags::flag_indent_space>())
  {
    indent_count <<= 1U; // Two space characters per indent
  }
  else
  {
    indent_character = L'\t';
  }

  return std::wstring(indent_count, indent_character);
}

const wchar_t ns::output::get_newline() const
{
  return L'\n';
}

const wchar_t ns::output::get_terminal() const
{
  return L';';
}

const wchar_t* ns::output::get_terminal_newline() const
{
  return L";\n";
}
