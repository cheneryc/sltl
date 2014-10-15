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
      syntax::get_current_block().add(syntax::make_node_ptr<syntax::conditional>(language::id_else));

      {
        scope s(scope::block);
        fn();
      }
    }

    template<typename Fn>
    else_statement else_if(scalar<bool>::proxy&& condition, Fn fn)
    {
      syntax::get_current_block().add(syntax::make_node_ptr<syntax::conditional>(language::id_else, condition.move()));

      {
        scope s(scope::block);
        fn();
      }

      return else_statement();
    }
  };

  template<typename Fn>
  else_statement if_(scalar<bool>::proxy&& condition, Fn fn)
  {
    syntax::get_current_block().add(syntax::make_node_ptr<syntax::conditional>(language::id_if, condition.move()));

    {
      scope s(scope::block);
      fn();
    }

    return else_statement();
  }
}
