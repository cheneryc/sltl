#pragma once

#include <string>
#include <sstream>


namespace sltl
{
  namespace syntax
  {
    // Foward declarations - sltl::syntax namespace
    class block;
    class variable_declaration;
    class variable_reference;
    class assignment_operator;
    class binary_operator;
    class conditional;

    template<typename T>
    class constant_declaration;
  }

  class output
  {
  public:
    output(bool is_indent_tab = true);

    void operator()(const syntax::block&, bool is_start = true);
    void operator()(const syntax::variable_declaration& vd, bool is_start = true);
    void operator()(const syntax::variable_reference& vr);
    void operator()(const syntax::assignment_operator& op);
    void operator()(const syntax::binary_operator& op);
    void operator()(const syntax::conditional& c, bool is_start = true);

    template<typename T>
    void operator()(const syntax::constant_declaration<T>& cd)
    {
      (*this)(cd._t);
    }

    void comma();

    void line_begin();
    void line_end();

    void parentheses_begin();
    void parentheses_end();

    std::wstring str() const;

  private:
    void operator()(float f);
    void operator()(double d);
    void operator()(int i);
    void operator()(unsigned int ui);
    void operator()(bool b);

    size_t _indent_count;
    std::wstringstream _ss;
    bool _is_indent_tab;
  };
}
