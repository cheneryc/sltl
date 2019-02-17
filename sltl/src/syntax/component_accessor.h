#pragma once

#include <type.h>

#include <memory>
#include <algorithm>


namespace sltl
{
namespace syntax
{
  class component_accessor
  {
  public:
    typedef std::unique_ptr<component_accessor> ptr;

    enum class mode
    {
      scalar,
      vector,
      matrix
    };

    static constexpr language::type_dimension_t _idx_default = std::numeric_limits<language::type_dimension_t>::max();

    virtual language::type get_type(const language::type& type_operand) const = 0;

    template<mode M, typename ...A>
    static component_accessor::ptr make(A&&... a)
    {
      return make_impl(tag<M>(), std::forward<A>(a)...);
    }

    const mode _mode;

  protected:
    component_accessor(mode mode) : _mode(mode) {}

    template<mode>
    struct tag {};

    static component_accessor::ptr make_impl(tag<mode::scalar>, language::type_dimension_t count);

    static component_accessor::ptr make_impl(tag<mode::vector>,
                                             language::type_dimension_t i0 = _idx_default,
                                             language::type_dimension_t i1 = _idx_default,
                                             language::type_dimension_t i2 = _idx_default,
                                             language::type_dimension_t i3 = _idx_default);

    static component_accessor::ptr make_impl(tag<mode::matrix>,
                                             language::type_dimension_t idx_m,
                                             language::type_dimension_t idx_n);
  };

  class component_accessor_scalar : public component_accessor
  {
    friend component_accessor::ptr component_accessor::make_impl(tag<mode::scalar>, language::type_dimension_t);

  public:
    language::type get_type(const language::type& type_operand) const override
    {
      //TODO: this doesn't work if column vectors/matrices are being used as there isn't a way
      // to deduce from the operand whether the returned type should be a row or column vector
      return language::type(type_operand.get_id(), 1U, _count);
    }

    const language::type_dimension_t _count;

  private:
    component_accessor_scalar(language::type_dimension_t count) : component_accessor(mode::scalar), _count(count)
    {
      assert((_count > 0) && (_count <= language::type_dimensions::max_dimensions));
    }
  };

  class component_accessor_vector : public component_accessor
  {
    friend component_accessor::ptr component_accessor::make_impl(tag<mode::vector>,
                                                                 language::type_dimension_t,
                                                                 language::type_dimension_t,
                                                                 language::type_dimension_t,
                                                                 language::type_dimension_t);

  public:
    language::type get_type(const language::type& type_operand) const override
    {
      const auto it_begin = std::begin(_indices);

      if(const auto count = std::distance(it_begin, std::find(it_begin, std::end(_indices), _idx_default)))
      {
        return (language::is_row_vector(type_operand.get_dimensions()) ?
                language::type(type_operand.get_id(), 1U, count) :
                language::type(type_operand.get_id(), count, 1U));
      }
      else
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    const language::type_dimension_t _indices[language::type_dimensions::max_dimensions];

    auto begin() const -> decltype(std::begin(_indices))
    {
      return std::begin(_indices);
    }

    auto end() const -> decltype(std::end(_indices))
    {
      return std::end(_indices);
    }

  private:
    component_accessor_vector(language::type_dimension_t idx0,
                              language::type_dimension_t idx1,
                              language::type_dimension_t idx2,
                              language::type_dimension_t idx3) : component_accessor(mode::vector), _indices{ idx0, idx1, idx2, idx3 }
    {
      assert(std::all_of(std::begin(_indices), std::end(_indices), [](language::type_dimension_t idx) { return (idx == _idx_default) || (idx < language::type_dimensions::max_dimensions); }));
    }
  };

  class component_accessor_matrix : public component_accessor
  {
    friend component_accessor::ptr component_accessor::make_impl(tag<mode::matrix>,
                                                                 language::type_dimension_t,
                                                                 language::type_dimension_t);

  public:
    language::type get_type(const language::type& type_operand) const override
    {
      const auto op_dimensions = type_operand.get_dimensions();

      assert((_idx_m == _idx_default) || (_idx_m < op_dimensions.m()));
      assert((_idx_n == _idx_default) || (_idx_n < op_dimensions.n()));

      const language::type_dimension_t m = ((_idx_m != _idx_default) ? 1U : op_dimensions.m());
      const language::type_dimension_t n = ((_idx_n != _idx_default) ? 1U : op_dimensions.n());

      return language::type(type_operand.get_id(), m, n);
    }

    const language::type_dimension_t _idx_m;
    const language::type_dimension_t _idx_n;

  private:
    component_accessor_matrix(language::type_dimension_t idx_m,
                              language::type_dimension_t idx_n) : component_accessor(mode::matrix),
      _idx_m(idx_m),
      _idx_n(idx_n)
    {
      assert((idx_m != _idx_default) || (idx_n != _idx_default));
    }
  };

  template<typename Fn>
  decltype(auto) visit(const component_accessor& accessor, Fn fn)
  {
    switch(accessor._mode)
    {
      case component_accessor::mode::scalar:
        return fn(static_cast<const component_accessor_scalar&>(accessor));
      case component_accessor::mode::vector:
        return fn(static_cast<const component_accessor_vector&>(accessor));
      case component_accessor::mode::matrix:
        return fn(static_cast<const component_accessor_matrix&>(accessor));
      default:
        throw std::exception();//TODO: exception type and message
    }
  }
}
}
