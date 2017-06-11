#pragma once

#include "block_base.h"

#include "../detail/pass_key.h"


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;
  class io_block_manager;

  class io_block : public block_base
  {
  public:
    io_block(detail::pass_key<io_block_manager>, core::qualifier_storage qualifier);

    void pop();

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

    const core::qualifier_storage _qualifier;

  protected:
    statement& add_impl(statement::ptr&&) override;

    variable_declaration& add_variable_declaration(std::wstring&&, expression::ptr&&) override;
    variable_declaration& add_variable_declaration(std::wstring&& name, const language::type& type, core::semantic_pair semantic) override;
  };
}
}
