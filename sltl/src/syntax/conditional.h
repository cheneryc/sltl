#pragma once

#include "statement.h"
#include "parentheses.h"
#include "../output.h"
#include "../language.h"

#include <cassert>


namespace sltl
{
namespace syntax
{
  //TODO: rename this class selection?
  class conditional : public statement
  {
  public:
    conditional(language::conditional_id id) : _id(id)
    {
      assert(_id == language::id_else);
    }

    conditional(language::conditional_id id, expression::ptr&& condition) : _id(id), _condition(expression::make<parentheses>(std::move(condition)))
    {
      assert((_id == language::id_if) || (_id == language::id_else_if));
    }

    template<typename T, typename ...A>
    T& add(A&&... a)
    {
      return static_cast<T&>(*(_statement = statement::make<T>(std::forward<A>(a)...)));
    }

    template<typename T, typename ...A>
    T& add_else(A&&... a)
    {
      return static_cast<T&>(*(_statement_else = statement::make<T>(std::forward<A>(a)...)));
    }

    //TODO: move into a cpp file (along with #includes)
    virtual void traverse(output& out) const
    {
      out(*this);

      assert(((_id == language::id_else) && !_condition) ||
             ((_id != language::id_else) &&  _condition));

      if(_condition)
      {
        _condition->traverse(out);
      }

      out(*this, false);

      assert( _statement);
      assert(!_statement_else || (_id != language::id_else));

      _statement->traverse(out);

      if(_statement_else)
      {
        _statement_else->traverse(out);
      }
    }

    const language::conditional_id _id;

  private:
    const expression::ptr _condition;

    statement::ptr  _statement;
    statement::ptr  _statement_else;
  };
}
}
