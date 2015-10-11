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

  const wchar_t* get_variable_qualifier(const ns::syntax::variable_declaration& vd)
  {
    if(is_variable_built_in(vd))
    {
      throw std::exception();//TODO: exception type and message
    }

    return ns::language::to_qualifier_string(static_cast<const ns::core::storage_qualifier&>(vd.get_qualifier())._value);
  }
}

ns::output::output(bool is_indent_tab) : _indent_count(0), _is_indent_tab(is_indent_tab)
{
}

std::wstring ns::output::str() const
{
  return _ss.str();
}

bool ns::output::operator()(const syntax::block&, bool is_start)
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
  return true;
}

bool ns::output::operator()(const syntax::io_block&, bool)
{
  return true;
}

bool ns::output::operator()(const syntax::variable_declaration& vd, bool is_start)
{
  if(!is_variable_built_in(vd))
  {
    if(is_start)
    {
      line_begin();

      //TODO: for 'in' qualified variables it is also necessary to figure out the layout qualifier stuff (e.g. 'layout (location = 0)')
      if(auto qualifier = get_variable_qualifier(vd))
      {
        _ss << qualifier << L' ';
      }

      _ss << language::to_type_string(vd._type) << L' ';
      _ss << get_variable_name(vd);

      if(vd.has_initializer())
      {
        _ss << (vd.is_direct_initialized() ? L"(" : L" = ");
      }
    }
    else
    {
      if(vd.has_initializer() && vd.is_direct_initialized())
      {
        _ss << L')';
      }

      line_end();
    }
  }

  return true;
}

bool ns::output::operator()(const syntax::parameter_declaration&)
{
  assert(false); //TODO: implement this...
  return true;
}

bool ns::output::operator()(const syntax::reference& r)
{
  _ss << get_variable_name(r._declaration);
  return true;
}

bool ns::output::operator()(const syntax::temporary& t, bool is_start)
{
  if(is_start)
  {
    _ss << language::to_type_string(t._type) << L'(';
  }
  else
  {
    _ss << L')';
  }

  return true;
}

bool ns::output::operator()(const syntax::assignment_operator& op)
{
  _ss << L' ' << language::to_assignment_operator_string(op._id) << ' ';
  return true;
}

bool ns::output::operator()(const syntax::binary_operator& op)
{
  _ss << L' ' << language::to_operator_string(op._id) << ' ';
  return true;
}

bool ns::output::operator()(const syntax::conditional& c, bool is_start)
{
  if(is_start)
  {
    line_begin();
    _ss << language::to_conditional_string(c._id);
  }
  else
  {
    line_end(false);
  }

  return true;
}

bool ns::output::operator()(const syntax::expression_statement&, bool is_start)
{
  if(is_start)
  {
    line_begin();
  }
  else
  {
    line_end();
  }

  return true;
}

bool ns::output::operator()(const syntax::parentheses&, bool is_start)
{
  if(is_start)
  {
    _ss << L'(';
  }
  else
  {
    _ss << L')';
  }

  return true;
}

bool ns::output::operator()(const syntax::list_separator&)
{
    _ss << L", ";
    return true;
}

bool ns::output::operator()(const syntax::function_call& fc, bool is_start)
{
  if(is_start)
  {
    _ss << fc.get_function_name() << L'(';
  }
  else
  {
    _ss << L')';
  }

  return true;
}

bool ns::output::operator()(const syntax::function_definition& fd, bool is_start)
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

  return true;
}

bool ns::output::operator()(const syntax::return_statement&, bool is_start)
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

  return true;
}

bool ns::output::operator()(float f)
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

  return true;
}

bool ns::output::operator()(double d)
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

  return true;
}

bool ns::output::operator()(int i)
{
  _ss << i;
  return true;
}

bool ns::output::operator()(unsigned int ui)
{
  _ss << ui << L'U';
  return true;
}

bool ns::output::operator()(bool b)
{
  _ss << std::boolalpha << b;
  return true;
}

bool ns::output::get_default()
{
  assert(false);
  return true;
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
