#pragma once

#include "parameter_declaration.h"

#include <type.h>

#include <core/intrinsic.h>


namespace sltl
{
namespace syntax
{
  const wchar_t* to_intrinsic_string(core::intrinsic id);

  class intrinsic_declaration : public declaration, public node
  {
  public:
    typedef std::unique_ptr<intrinsic_declaration> ptr;

    intrinsic_declaration(const intrinsic_declaration&) = delete;
    intrinsic_declaration(core::intrinsic i, parameter_list&& parameters, const language::type& type_return) : declaration(to_intrinsic_string(i)), _intrinsic(i), _type_return(type_return), _parameters(std::move(parameters)) {}

    bool apply_action(action& act) override
    {
      return apply_action_impl(act, *this, &_parameters);
    }

    bool apply_action(const_action& cact) const override
    {
      return apply_action_impl(cact, *this, &_parameters);
    }

    language::type get_type() const override
    {
      return _type_return;
    }

    friend bool operator<(const intrinsic_declaration& id1, const intrinsic_declaration& id2)
    {
      return detail::less(id1._intrinsic, id1._parameters, id2._intrinsic, id2._parameters);
    }

    friend bool operator>(const intrinsic_declaration& id1, const intrinsic_declaration& id2)
    {
      return id2 < id1;
    }

    friend bool operator<=(const intrinsic_declaration& id1, const intrinsic_declaration& id2)
    {
      return !(id1 > id2);
    }

    friend bool operator>=(const intrinsic_declaration& id1, const intrinsic_declaration& id2)
    {
      return !(id1 < id2);
    }

    const core::intrinsic _intrinsic;

  private:
    language::type _type_return;
    parameter_list _parameters;
  };
}
}
