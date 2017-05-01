#pragma once

#include "component_access.h"

#include "../detail/function_traits.h"


namespace sltl
{
namespace syntax
{
  class component_accessor
  {
  public:
    //TODO: will probably need an overload for when the visitor has no return type
    //TODO: this should be moved into the accessor_base class as a virtual member function? This might not be possible if the visit() member function needs to be templated
    //TODO: return type could be auto-deduced using decltype(auto)
    //TODO: make static?
    template<typename Fn>
    //auto visit(Fn fn) const -> typename detail::function_traits<Fn>::return_t
    decltype(auto) visit(Fn fn) const
    {
      switch(_mode)
      {
        case component_access::mode::scalar:
          return fn(static_cast<const component_access_scalar&>(*_access));
          break;
        case component_access::mode::vector:
          return fn(static_cast<const component_access_vector&>(*_access));
          break;
        case component_access::mode::matrix:
          return fn(static_cast<const component_access_matrix&>(*_access));
          break;
        default:
          throw std::exception();//TODO: exception type and message
      }
    }

    language::type get_type(const language::type& type_operand) const
    {
      bool is_valid = false;

      switch(_mode)
      {
        case component_access::mode::scalar:
          is_valid = type_operand.get_dimensions().is_scalar();
          break;
        case component_access::mode::vector:
          is_valid = type_operand.get_dimensions().is_vector();
          break;
        case component_access::mode::matrix:
          is_valid = type_operand.get_dimensions().is_matrix();
          break;
        default:
          assert(_mode == component_access::mode::scalar ||
                 _mode == component_access::mode::vector ||
                 _mode == component_access::mode::matrix);
      }

      if(is_valid)
      {
        return _access->get_type(type_operand);
      }
      else
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    template<component_access::mode T, typename ...A>
    static component_accessor make(A&&... a)
    {
      return make_impl(tag<T>(), std::forward<A>(a)...);
    }

    const component_access::mode _mode;

  private:
    component_accessor(component_access::mode mode, std::unique_ptr<component_access>&& access) : _mode(mode), _access(std::move(access)) {}

    template<component_access::mode>
    struct tag {};

    static component_accessor make_impl(tag<component_access::mode::scalar>, language::type_dimension_t count)
    {
      return component_accessor(component_access::mode::scalar, std::make_unique<component_access_scalar>(count));
    }

    static component_accessor make_impl(tag<component_access::mode::vector>,
                                        language::type_dimension_t i0 = component_access::_idx_default,
                                        language::type_dimension_t i1 = component_access::_idx_default,
                                        language::type_dimension_t i2 = component_access::_idx_default,
                                        language::type_dimension_t i3 = component_access::_idx_default)
    {
      return component_accessor(component_access::mode::vector, std::make_unique<component_access_vector>(i0, i1, i2, i3));
    }

    static component_accessor make_impl(tag<component_access::mode::matrix>,
                                        language::type_dimension_t idx_m,
                                        language::type_dimension_t idx_n)
    {
      return component_accessor(component_access::mode::matrix, std::make_unique<component_access_matrix>(idx_m, idx_n));
    }

    std::unique_ptr<component_access> _access;
  };
}
}
