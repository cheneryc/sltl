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
    void make_else(Fn fn)
    {
      syntax::get_current_block().add(std::make_unique<syntax::conditional>(language::id_else));

      {
        scope s(scope::block);
        fn();
      }
    }

    //TODO: should accept a scalar_ref/proxy instead?
    template<typename Fn>
    else_statement make_else_if(const scalar<bool>& condition, Fn fn)
    {
      auto n = std::make_unique<syntax::variable_reference>(condition.get_declaration());
      syntax::get_current_block().add(std::make_unique<syntax::conditional>(language::id_else, std::move(n)));

      {
        scope s(scope::block);
        fn();
      }

      return else_statement();
    }
  };

  //TODO: should accept a scalar_ref/proxy instead?
  template<typename Fn>
  else_statement make_if(const scalar<bool>& condition, Fn fn)
  {
    auto n = std::make_unique<syntax::variable_reference>(condition.get_declaration());
    syntax::get_current_block().add(std::make_unique<syntax::conditional>(language::id_if, std::move(n)));

    {
      scope s(scope::block);
      fn();
    }

    return else_statement();
  }
}
