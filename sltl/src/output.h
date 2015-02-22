#pragma once

#include <string>
#include <sstream>


namespace sltl
{
  // Forward declarations - sltl::syntax namespace
  namespace syntax
  {
    class block;
    class variable_declaration;
    class parameter_declaration;
    class reference;
    class temporary;
    class assignment_operator;
    class binary_operator;
    class conditional;
    class expression_statement;
    class parentheses;
    class list_separator;
    class function_call;
    class function_definition;
    class return_statement;

    template<typename T>
    class literal;
  }

  class output
  {
  public:
    output(bool is_indent_tab = true);

    void operator()(const syntax::block&, bool is_start = true);
    void operator()(const syntax::variable_declaration& vd, bool is_start = true);
    void operator()(const syntax::parameter_declaration& pd);
    void operator()(const syntax::reference& r);
    void operator()(const syntax::temporary& t, bool is_start = true);
    void operator()(const syntax::assignment_operator& op);
    void operator()(const syntax::binary_operator& op);
    void operator()(const syntax::conditional& c, bool is_start = true);
    void operator()(const syntax::expression_statement& es, bool is_start = true);
    void operator()(const syntax::parentheses& p, bool is_start = true);
    void operator()(const syntax::list_separator& ls);
    void operator()(const syntax::function_call& fc, bool is_start = true);
    void operator()(const syntax::function_definition& fd, bool is_start = true);
    void operator()(const syntax::return_statement& rs, bool is_start = true);

    template<typename T>
    void operator()(const syntax::literal<T>& cd)
    {
      (*this)(cd._t);
    }

    std::wstring str() const;

  private:
    void operator()(float f);
    void operator()(double d);
    void operator()(int i);
    void operator()(unsigned int ui);
    void operator()(bool b);

    void line_begin();
    void line_end(bool has_semi_colon = true);

    size_t _indent_count;
    std::wstringstream _ss;
    bool _is_indent_tab;
  };
}
