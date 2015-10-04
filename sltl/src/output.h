#pragma once

#include "syntax/action.h"

#include <string>
#include <sstream>


namespace sltl
{
  class output : public syntax::const_action
  {
  public:
    output(bool is_indent_tab = true);

    virtual bool operator()(const syntax::block&, bool is_start = true) override;
    virtual bool operator()(const syntax::io_block&, bool is_start = true) override;
    virtual bool operator()(const syntax::variable_declaration& vd, bool is_start = true) override;
    virtual bool operator()(const syntax::parameter_declaration& pd) override;
    virtual bool operator()(const syntax::reference& r) override;
    virtual bool operator()(const syntax::temporary& t, bool is_start = true) override;
    virtual bool operator()(const syntax::assignment_operator& op) override;
    virtual bool operator()(const syntax::binary_operator& op) override;
    virtual bool operator()(const syntax::conditional& c, bool is_start = true) override;
    virtual bool operator()(const syntax::expression_statement& es, bool is_start = true) override;
    virtual bool operator()(const syntax::parentheses& p, bool is_start = true) override;
    virtual bool operator()(const syntax::list_separator& ls) override;
    virtual bool operator()(const syntax::function_call& fc, bool is_start = true) override;
    virtual bool operator()(const syntax::function_definition& fd, bool is_start = true) override;
    virtual bool operator()(const syntax::return_statement& rs, bool is_start = true) override;

    std::wstring str() const;

  protected:
    virtual bool operator()(float f) override;
    virtual bool operator()(double d) override;
    virtual bool operator()(int i) override;
    virtual bool operator()(unsigned int ui) override;
    virtual bool operator()(bool b) override;

    virtual bool get_default() override;

  private:
    void line_begin();
    void line_end(bool has_semi_colon = true);

    size_t _indent_count;
    std::wstringstream _ss;
    bool _is_indent_tab;
  };
}
