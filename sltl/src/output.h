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
    class reference;
    class temporary;
    class assignment_operator;
    class binary_operator;
    class conditional;
    class expression_statement;
    class parentheses;

    template<typename T>
    class literal;
  }

  class output
  {
  public:
    output(bool is_indent_tab = true);

    void operator()(const syntax::block&, bool is_start = true);
    void operator()(const syntax::variable_declaration& vd, bool is_start = true);
    void operator()(const syntax::reference& r);
    void operator()(const syntax::temporary& t, bool is_start = true);
    void operator()(const syntax::assignment_operator& op);
    void operator()(const syntax::binary_operator& op);
    void operator()(const syntax::conditional& c, bool is_start = true);
    void operator()(const syntax::expression_statement& es, bool is_start = true);
    void operator()(const syntax::parentheses& p, bool is_start = true);

    template<typename T>
    void operator()(const syntax::literal<T>& cd)
    {
      (*this)(cd._t);
    }

    void comma();

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
