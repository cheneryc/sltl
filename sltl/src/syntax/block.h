#pragma once

#include "statement.h"
#include "variable_info.h"

#include <map>
#include <stack>
#include <vector>
#include <string>


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class action;
  class const_action;

  class block : public statement
  {
  public:
    enum type
    {
      local,
      global
    };

    block(type t);

    bool operator==(const block& rhs) const;
    bool operator!=(const block& rhs) const;

    std::wstring get_child_name();

    template<typename T, typename ...A>
    T& add(A&&... a)
    {
      _statements.emplace_back(new T(std::forward<A>(a)...));
      return static_cast<T&>(*_statements.back());
    }

    void erase(const statement& s);
    void pop();

    virtual bool apply_action(action& act) override;
    virtual bool apply_action(const_action& cact) const override;

    void variable_info_add(const std::wstring& name);
    variable_info& variable_info_find(const std::wstring& name);

  protected:
    std::vector<statement::ptr> _statements;

  private:
    variable_info& variable_info_find(const std::wstring& name, std::stack<std::reference_wrapper<block>> block_stack);

    const type _t;

    size_t _current_child_id;
    const std::wstring _name;

    std::map<std::wstring, variable_info> _variable_map;
  };
}
}
