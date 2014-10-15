#pragma once

#include "traits.h"
#include "variable.h"

#include "syntax/operator.h"


namespace sltl
{
  template<template<typename, size_t> class V, typename T, size_t D>
  class basic : public variable
  {
    typedef V<T, D> derived_t;

  public:
    static_assert(is_scalar<T>::value, "sltl::basic: Type T is not a valid template parameter type");

    class proxy
    {
    public:
      //TODO: this is probably not suitable for vector?
      proxy(T t) : proxy(syntax::make_node_ptr<syntax::constant_declaration<T>>(t)) {}

      proxy(proxy&& p) : proxy(std::move(p.n)) {}
      proxy(syntax::node::ptr&& n) : _n(std::move(n)) {}
      proxy(const derived_t& d) : proxy(d.make_reference()) {}

      proxy(derived_t&& d) : proxy(syntax::get_current_block().remove(d._vd))
      {
        d._vd->make_rvalue();
      }

      ~proxy()
      {
        if(_n)
        {
          syntax::get_current_block().add(std::move(_n));
        }
      }

      syntax::node::ptr&& move()
      {
        return std::move(_n);
      }

      proxy(const proxy& p) = delete;
      proxy& operator=(const proxy&) = delete;

    private:
      syntax::node::ptr _n;
    };

    proxy operator+=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment_addition, make_reference(), p.move());
    }

    friend proxy operator+(proxy&& lhs, proxy&& rhs)
    {
      return make_proxy<syntax::binary_operator>(language::id_addition, lhs.move(), rhs.move());
    }

    syntax::node::ptr make_reference() const
    {
      return syntax::node::ptr(new syntax::variable_reference(*_vd));
    }

  protected:
    basic(language::type_id id) : variable(id) {}

    template<typename N, typename ...T>
    static proxy make_proxy(T&& ...t)
    {
      return syntax::node::ptr(new N(std::forward<T>(t)...));
    }
  };
}
