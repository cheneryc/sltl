#pragma once

#include "statement.h"

#include <string>
#include <vector>


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

  private:
    type _t;

    size_t _current_child_id;
    const std::wstring _name;

    std::vector<statement::ptr> _statements;
  };
}
}
