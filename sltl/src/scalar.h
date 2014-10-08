#pragma once

#include "element.h"
#include "syntax/block_manager.h"//remove somehow
#include "syntax/variable_declaration.h"
#include "syntax/operator.h"
#include "language.h"//remove if possible


namespace sltl
{
  //TODO: move the is_*** types into a 'type_traits' sub-namespace/header
  template<typename T>
  struct is_integer
  {
    static const bool value = (std::is_same<int, T>::value || std::is_same<unsigned int, T>::value);
  };

  template<typename T>
  struct is_real
  {
    // Don't use std::is_floating_point as it accepts long double types
    static const bool value = (std::is_same<float, T>::value || std::is_same<double, T>::value);
  };

  template<typename T>
  struct is_scalar
  {
    static const bool value = (is_integer<T>::value || is_real<T>::value || std::is_same<bool, T>::value);
  };

  class variable : public element
  {
  public:
    const syntax::variable_declaration& get_declaration() const
    {
      return *_vd;
    }

  protected:
    variable(language::type_id id) : _vd(nullptr)
    {
      auto vd = std::make_unique<syntax::variable_declaration>(id);
      _vd = vd.get();
      syntax::get_current_block().add(std::move(vd));
    }

    //TODO: check for duplicate names being declared in the same scope?
    //void set_name(const std::wstring& name);

    syntax::variable_declaration* _vd;
  };

  //TODO: move type generic operators into this class (i.e. addition, assignment (might be tricky) etc.)?
  class basic : public variable
  {
  public:
    basic(language::type_id id) : variable(id) {}
  };

  //TODO: what about constant scalar variables??
  //eg.
  // shader::const_scalar<float> my_const = 1.0f;
  // my_const += /*expression*/; // Error, this is not allowed

  template<typename T>
  struct scalar_id
  {
    static const language::type_id value = language::id_unknown;
  };

  template<>
  struct scalar_id<float>
  {
    static const language::type_id value = language::id_float;
  };

  template<>
  struct scalar_id<double>
  {
    static const language::type_id value = language::id_double;
  };

  template<>
  struct scalar_id<int>
  {
    static const language::type_id value = language::id_int;
  };

  template<>
  struct scalar_id<unsigned int>
  {
    static const language::type_id value = language::id_uint;
  };

  template<>
  struct scalar_id<bool>
  {
    static const language::type_id value = language::id_bool;
  };

  template<typename T>
  class scalar : public basic
  {
    static_assert(is_scalar<T>::value, "Type T is not a valid template parameter for sltl::scalar type");

  public:
    class proxy
    {
    public:
      //TODO: have all the constructors call the proxy(syntax::node::ptr&& n) version, rather than set _n themselves
      proxy(T t) : _n(std::make_unique<syntax::constant_declaration<T>>(t)) {}
      proxy(proxy&& p) : _n(std::move(p.n)) {}
      proxy(syntax::node::ptr&& n) : _n(std::move(n)) {}
      proxy(const scalar& s) : _n(s.make_reference()) {}

      proxy(scalar&& s) : _n(syntax::get_current_block().remove(s._vd))
      {
        s._vd->make_rvalue();
      }

      ~proxy()
      {
        if(_n)
        {
          syntax::get_current_block().add(std::move(_n));
        }
      }

      //TODO: rename this move?
      syntax::node::ptr&& take()
      {
        return std::move(_n);
      }

    private:
      proxy(const proxy& p);
      proxy& operator=(const proxy&);

      syntax::node::ptr _n;
    };

    scalar() : basic(scalar_id<T>::value) {}
    scalar(scalar&& s) : scalar(proxy(std::move(s))) {}
    scalar(const scalar& s) : scalar(proxy(s)) {}

    scalar(proxy p) : scalar()
    {
      _vd->add(p.take());
    }

    proxy operator=(proxy p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment, make_reference(), p.take());
    }

    proxy operator=(scalar&& s)
    {
      return this->operator=(proxy(std::move(s)));
    }

    proxy operator=(const scalar& s)
    {
      return this->operator=(proxy(s));
    }

    proxy operator+=(proxy p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment_addition, make_reference(), p.take());
    }

    friend proxy operator+(proxy lhs, proxy rhs)
    {
      return make_proxy<syntax::binary_operator>(language::id_addition, lhs.take(), rhs.take());
    }

  private:
    template<typename N, typename ...T>
    static proxy make_proxy(T&& ...t)
    {
      return syntax::node::ptr(new N(std::forward<T>(t)...));
    }

    syntax::node::ptr make_reference() const
    {
      return syntax::node::ptr(new syntax::variable_reference(*_vd));
    }
  };
}
