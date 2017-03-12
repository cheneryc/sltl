#pragma once

#include "statement.h"
#include "variable_info.h"

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
    virtual std::wstring get_child_name();

    template<typename T, typename ...A>
    T& add(A&&... a)
    {
      _statements.emplace_back(new T(std::forward<A>(a)...));
      return static_cast<T&>(*_statements.back());
    }

    void erase(const statement& s);

    void variable_info_add(const std::wstring& name);
    virtual variable_info& variable_info_find(const std::wstring& name);

  protected:
    block_base(std::wstring&& name);

    const std::wstring& get_name() const
    {
      return _name;
    }

    std::vector<statement::ptr> _statements;
    std::map<std::wstring, variable_info> _variable_map;

  private:
    size_t _current_child_id;
    const std::wstring _name;
  };
}
}
