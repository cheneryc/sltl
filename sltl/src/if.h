#pragma once

#include "scalar.h"
#include "scope.h"

#include "syntax/conditional.h"


namespace sltl
{
  class else_statement
  {
  public:
    template<typename Fn>
    void else_(Fn fn)
    {
      auto& selection = _c.add_else<syntax::conditional>(language::id_else);

      {
        scope s(scope::block, selection);
        fn();
      }
    }

    template<typename Fn>
    else_statement else_if(scalar<bool>::proxy&& condition, Fn fn)
    {
      auto& selection = _c.add_else<syntax::conditional>(language::id_else_if, condition.move());

      {
        scope s(scope::block, selection);
        fn();
      }

      return else_statement(selection);
    }

    else_statement& operator=(const else_statement&) = delete;

  private:
    else_statement(syntax::conditional& c) : _c(c) {}

    template<typename Fn>
    friend else_statement if_(scalar<bool>::proxy&&, Fn);

    syntax::conditional& _c;
  };

  template<typename Fn>
  else_statement if_(scalar<bool>::proxy&& condition, Fn fn)
  {
    auto& selection = syntax::get_current_block().add<syntax::conditional>(language::id_if, condition.move());

    {
      scope s(scope::block, selection);
      fn();
    }

    return else_statement(selection);
  }
}
