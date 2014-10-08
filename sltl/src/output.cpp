#include "output.h"
#include "syntax/block.h"
#include "syntax/variable_declaration.h"
#include "syntax/operator.h"
#include "syntax/conditional.h"
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
}

ns::output::output(bool is_indent_tab) : _indent_count(0), _is_indent_tab(is_indent_tab)
{
}

std::wstring ns::output::str() const
{
  return _ss.str();
}

//TODO: interestingly we don't actually make use of the passed block object here!
void ns::output::operator()(const syntax::block& b, bool is_start)
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
}

void ns::output::operator()(const syntax::variable_declaration& vd, bool is_start)
{
  if(is_start)
  {
    _ss << language::to_type_string(vd._id);

    if(!vd.is_rvalue())
    {
      _ss << " " << language::to_prefix_string(vd._id) << vd._name;
    }
  }
  
  if(vd.get_expression() || vd.is_rvalue())
  {
    _ss << (is_start ? L'(' : L')');
  }
}

void ns::output::operator()(const syntax::variable_reference& vr)
{
  _ss << language::to_prefix_string(vr._vd._id) << vr._vd._name;
}

void ns::output::operator()(const syntax::assignment_operator& op)
{
  _ss << L' ' << language::to_assignment_operator_string(op._id) << ' ';
}

void ns::output::operator()(const syntax::binary_operator& op)
{
  _ss << L' ' << language::to_operator_string(op._id) << ' ';
}

void ns::output::operator()(const syntax::conditional& c, bool is_start)
{
  if(is_start)
  {
    _ss << language::to_conditional_string(c._id);
  }

  _ss << (is_start ? L'(' : L')');
}

void ns::output::operator()(float f)
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
}

void ns::output::operator()(double d)
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
}

void ns::output::operator()(int i)
{
  _ss << i;
}

void ns::output::operator()(unsigned int ui)
{
  _ss << ui << L'U';
}

void ns::output::operator()(bool b)
{
  _ss << std::boolalpha << b;
}

void ns::output::line_begin()
{
  output_indent(_ss, _indent_count, _is_indent_tab);
}

void ns::output::line_end()
{
  _ss << L';' << std::endl;
}

void ns::output::parentheses_begin()
{
  _ss << L'(';
}

void ns::output::parentheses_end()
{
  _ss << L')';
}
