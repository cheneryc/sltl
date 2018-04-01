#pragma once

#include "expression.h"
#include "intrinsic_declaration.h"
#include "intrinsic_manager.h"

#include "../language.h"

#include "../core/intrinsic.h"


namespace sltl
{
namespace syntax
{
  class intrinsic_call : public expression
  {
  public:
    intrinsic_call(const intrinsic_declaration& id, expression_list&& args) : _id(id), _args(std::move(args))
    {
    }

    bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, &_args);
    }

    bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, &_args);
    }

    language::type get_type() const override
    {
      return _id.get_type();
    }

    core::intrinsic get_intrinsic() const
    {
      return _id._intrinsic;
    }

  private:
    const intrinsic_declaration& _id;
    expression_list _args;
  };

  inline expression::ptr call_intrinsic_dot(expression::ptr&& exp_x, expression::ptr&& exp_y)
  {
    language::type type_x = exp_x->get_type();

    if(type_x.get_id() != language::type_id::id_float &&
       type_x.get_id() != language::type_id::id_double)
    {
      throw std::exception();//TODO: exception type and message
    }

    if(!(type_x.get_dimensions().is_scalar() ||
         type_x.get_dimensions().is_vector()))
    {
      throw std::exception();//TODO: exception type and message
    }

    language::type type_y = exp_y->get_type();

    if(type_y.get_id() != language::type_id::id_float &&
       type_y.get_id() != language::type_id::id_double)
    {
      throw std::exception();//TODO: exception type and message
    }

    if(!(type_y.get_dimensions().is_scalar() ||
         type_y.get_dimensions().is_vector()))
    {
      throw std::exception();//TODO: exception type and message
    }

    if(type_x != type_y)
    {
      throw std::exception();//TODO: exception type and message
    }

    parameter_list parameters;

    parameters.add(std::make_unique<parameter_declaration>(L"x", exp_x->get_type(), core::qualifier_param::in));
    parameters.add(std::make_unique<parameter_declaration>(L"y", exp_y->get_type(), core::qualifier_param::in));

    expression_list args;

    args.add(std::move(exp_x));
    args.add(std::move(exp_y));

    return expression::make<syntax::intrinsic_call>(intrinsic_manager_guard()->emplace(core::intrinsic::dot, std::move(parameters), language::type(type_x.get_id(), 1U, 1U)), std::move(args));
  }
}
}
