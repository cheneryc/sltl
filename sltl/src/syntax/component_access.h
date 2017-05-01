#pragma once

#include "../language.h"


namespace sltl
{
namespace syntax
{
  struct component_access
  {
    enum class mode
    {
      scalar,
      vector,
      matrix
    };

    static const language::type_dimension_t _idx_default = std::numeric_limits<language::type_dimension_t>::max();

    virtual ~component_access()
    {
    }

    virtual language::type get_type(const language::type& type_operand) const = 0;
  };

  struct component_access_scalar : component_access
  {
    component_access_scalar(language::type_dimension_t count) : _count(count)
    {
      assert((_count > 0) && (_count <= language::type_dimensions::max_dimensions));
    }

    language::type get_type(const language::type& type_operand) const override
    {
      //TODO: this doesn't work if column vectors/matrices are being used as there isn't a way
      // to deduce from the operand whether the returned type should be a row or column vector
      return language::type(type_operand.get_id(), 1U, _count);
    }

    const language::type_dimension_t _count;
  };

  struct component_access_vector : component_access
  {
    component_access_vector(language::type_dimension_t idx0,
                            language::type_dimension_t idx1,
                            language::type_dimension_t idx2,
                            language::type_dimension_t idx3) : _indices{ idx0, idx1, idx2, idx3 }
    {
      assert(std::all_of(std::begin(_indices), std::end(_indices), [](language::type_dimension_t idx){ return (idx == _idx_default) || (idx < language::type_dimensions::max_dimensions); }));
    }

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
  };

  struct component_access_matrix : component_access
  {
    component_access_matrix(language::type_dimension_t idx_m, language::type_dimension_t idx_n) : _idx_m(idx_m), _idx_n(idx_n)
    {
      assert((idx_m != _idx_default) || (idx_n != _idx_default));
    }

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
  };
}
}
