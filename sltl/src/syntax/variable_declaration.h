#pragma once

#include "block.h"
#include "block_manager.h"

#include "../output.h"
#include "../language.h"


namespace sltl
{
namespace syntax
{
  class variable_declaration : public node
  {
  public:
    variable_declaration(language::type_id id) : _id(id), _name(block_manager::get().get_block().get_child_name()), _is_rvalue(false) {}

    //TODO: need a virtual destructor?

    virtual void traverse(output& out) const
    {
      out(*this);

      if(_expression)
      {
        _expression->traverse(out);
      }

      out(*this, false);
    }

    bool is_rvalue() const
    {
      return _is_rvalue;
    }

    void make_rvalue()
    {
      _is_rvalue = true;
    }

    void add(node::ptr&& node)
    {
      _expression = std::move(node);
    }

    node::ptr remove()
    {
      return std::move(_expression);
    }

    const node* get_expression() const
    {
      return _expression.get();
    }

    const language::type_id _id;
    const std::wstring _name;

  private:
    node::ptr _expression;//TODO: better name
    bool _is_rvalue;
  };

  //TODO: limit this to the allowed types (i.e. int, uint, float, double, bool etc.)
  //TODO: this should be called literal_declaration instead?
  template<typename T>
  class constant_declaration : public node
  {
  public:
    constant_declaration(T t) : _t(t) {}

    virtual void traverse(output& out) const
    {
      out(*this);
    }

    const T _t;
  };

  class variable_reference : public node
  {
  public:
    variable_reference(const variable_declaration& vd) : _vd(vd) {}

    //TODO: need a virtual destructor?

    virtual void traverse(output& out) const
    {
      out(*this);
    }

    const variable_declaration& _vd;
  };
}
}
