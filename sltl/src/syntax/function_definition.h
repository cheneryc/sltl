#pragma once

#include "node.h"
#include "block.h"
#include "block_manager.h"
#include "elide.h"
#include "action.h"
#include "declaration.h"
#include "parameter_declaration.h"
#include "return_statement.h"

#include "../language.h"

#include "detail/function_traits.h"


namespace sltl
{
namespace syntax
{
  class function_definition : public declaration, public node
  {
  public:
    typedef std::unique_ptr<function_definition> ptr;

    template<typename Fn>
    function_definition(Fn fn, std::wstring&& name, const language::type& type_return) : declaration(std::move(name)), _type_return(type_return), _function_body(block::global)
    {
      call_fn(fn);
      _function_body.pop();
    }

    virtual bool apply_action(action& act) override
    {
      return apply_action(act, *this);
    }

    virtual bool apply_action(const_action& cact) const override
    {
      return apply_action(cact, *this);
    }

    void set_type(const language::type& type)
    {
      _type_return = type;
    }

    virtual language::type get_type() const override
    {
      return _type_return;
    }

  private:
    template<typename A, typename T>
    static auto apply_action(A& act, T& type) -> typename std::enable_if<std::is_same<typename std::remove_const<T>::type, function_definition>::value, bool>::type
    {
      return (apply_action_impl(act, type, &(type._parameters)) && type._function_body.apply_action(act));
    }

    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if< std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      fn();
    }

    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if<!std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      syntax::get_current_block().add<syntax::return_statement>(elide(detail::function_traits<Fn>::return_t::proxy(fn()).move(), _type_return));
    }

    language::type _type_return;
    parameter_list _parameters;
    block _function_body;
  };
}
}
