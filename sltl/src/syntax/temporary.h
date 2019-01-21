#pragma once

#include "expression.h"

#include <type.h>


namespace sltl
{
namespace syntax
{
  class temporary : public expression
  {
  public:
    temporary(const language::type& type) : _type(std::make_unique<language::type>(type)), _initializer() {}
    temporary(expression::ptr&& initializer) : _type(), _initializer(std::move(initializer)) {}

    bool has_type() const
    {
      return static_cast<bool>(_type);
    }

    bool has_initializer() const
    {
      return static_cast<bool>(_initializer);
    }

    expression::ptr&& move()//TODO: all functions like this (e.g. variable_declaration.move()) should only be callable for r-value references?
    {
      return std::move(_initializer);
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, _initializer.get());
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, _initializer.get());
    }

    void set_type(const language::type& type)
    {
      if(_initializer)
      {
        throw std::exception();//TODO: exception type and message
      }

      *_type = type;
    }

    virtual language::type get_type() const override
    {
      return (_type ? *_type : _initializer->get_type());
    }

    const expression* get_initializer() const
    {
      return _initializer.get();
    }

  private:
    //TODO: make this std::optional once available (C++17)
    std::unique_ptr<language::type> _type;//TODO: const (ptr or expression or both)?
    expression::ptr _initializer;//TODO: const (ptr or expression or both)?
  };
}
}
