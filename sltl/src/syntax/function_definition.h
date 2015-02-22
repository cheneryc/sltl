#pragma once

#include "node.h"
#include "block.h"
#include "declaration.h"
#include "parameter_declaration.h"
#include "return_statement.h"

#include "../output.h"
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

    virtual void traverse(output& out) const
    {
      out(*this);
      _parameters.traverse(out);
      out(*this, false);

      _function_body.traverse(out);
    }

    const language::type _type_return;

  private:
    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if< std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      fn();
    }

    template<typename Fn>
    auto call_fn(Fn fn) -> typename std::enable_if<!std::is_same<typename detail::function_traits<Fn>::return_t, void>::value>::type
    {
      syntax::get_current_block().add<syntax::return_statement>(detail::function_traits<Fn>::return_t::proxy(fn()).move());
    }

    parameter_list _parameters;
    block _function_body;
  };
}
}
