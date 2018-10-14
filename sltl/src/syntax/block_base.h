#pragma once

#include "statement.h"
#include "variable_declaration.h"
#include "variable_info.h"

#include <detail/conditional_traits.h>

#include <map>
#include <vector>
#include <string>


namespace sltl
{
namespace syntax
{
  class block_base : public statement
  {
  public:
    template<typename T, typename ...A>
    auto add(A&&... a) -> typename std::enable_if<std::is_same<T, variable_declaration>::value, T&>::type
    {
      return add_variable_declaration(get_child_name(), std::forward<A>(a)...);
    }

    template<typename T, typename ...A>
    auto add(A&&... a) -> typename std::enable_if<detail::negate<std::is_same<T, variable_declaration>>::value, T&>::type
    {
      add_impl(statement::make<T>(std::forward<A>(a)...));

      auto& statement_ptr = _statements.back();
      auto& statement = static_cast<T&>(*statement_ptr);

      return statement;
    }

    virtual void push() = 0;
    virtual void pop() = 0;

    void erase(const statement& s);

    bool operator==(const block_base& rhs) const;
    bool operator!=(const block_base& rhs) const;

    virtual variable_info* variable_info_find(const std::wstring& name);

  protected:
    block_base(std::wstring&& name);
    block_base(const wchar_t* name);

    virtual statement& add_impl(statement::ptr&& s);

    virtual variable_declaration& add_variable_declaration(std::wstring&& name, expression::ptr&& initializer) = 0;
    virtual variable_declaration& add_variable_declaration(std::wstring&& name, const language::type& type, core::semantic_pair semantic = core::semantic_pair::none) = 0;

    variable_declaration& add_variable_declaration_impl(statement::ptr&& s);

    const std::wstring& get_name() const;
    virtual std::wstring get_child_name();

    std::vector<statement::ptr> _statements;

  private:
    size_t _current_child_id;
    const std::wstring _name;

    std::map<std::wstring, variable_info> _variable_map;
  };
}
}
