#pragma once

#include "block_base.h"

#include <stack>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class block : public block_base
  {
  public:
    enum type
    {
      local,
      global
    };

    block(type t);

    void push() override;
    void pop() override;

    variable_info* variable_info_find(const std::wstring& name) override;

    bool apply_action(action& act) override;
    bool apply_action(const_action& cact) const override;

    const type _t;

  protected:
    statement& add_impl(statement::ptr&&) override;

    variable_declaration& add_variable_declaration(std::wstring&& name, expression::ptr&& initializer) override;
    variable_declaration& add_variable_declaration(std::wstring&& name, const language::type& type, core::semantic_pair semantic) override;

    std::wstring get_child_name() override;

  private:
    variable_info* variable_info_find(const std::wstring& name, std::stack<std::reference_wrapper<block>> block_stack);
  };
}
}
