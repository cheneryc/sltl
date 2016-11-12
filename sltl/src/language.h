#pragma once

#include "core/qualifier.h"

#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>

#include <cassert>


namespace sltl
{
namespace language
{
  enum type_id
  {
    id_unknown,
    id_void,
    id_float,
    id_double,
    id_int,
    id_uint,
    id_bool,
  };

  template<typename T>
  struct type_id_helper
  {
    static const language::type_id value = language::id_unknown;
  };

  template<>
  struct type_id_helper<void>
  {
    static const language::type_id value = language::id_void;
  };

  template<>
  struct type_id_helper<float>
  {
    static const language::type_id value = language::id_float;
  };

  template<>
  struct type_id_helper<double>
  {
    static const language::type_id value = language::id_double;
  };

  template<>
  struct type_id_helper<int>
  {
    static const language::type_id value = language::id_int;
  };

  template<>
  struct type_id_helper<unsigned int>
  {
    static const language::type_id value = language::id_uint;
  };

  template<>
  struct type_id_helper<bool>
  {
    static const language::type_id value = language::id_bool;
  };

  typedef size_t type_dimension_t;

  class type_dimensions
  {
  public:
    type_dimensions(type_dimension_t m, type_dimension_t n) : _m(m), _n(n)
    {
      assert(is_void() || is_scalar() || is_vector() || is_matrix());
      assert(m <= max_dimensions);
      assert(n <= max_dimensions);
    }

    static const type_dimension_t max_dimensions = 4U;

    bool is_void() const
    {
      // m and n must both be zero
      return ((_m == 0U) && (_n == 0U));
    }

    bool is_scalar() const
    {
      // m and n must both be one
      return ((_m == 1U) && (_n == 1U));
    }

    bool is_vector() const
    {
      // m or n, but not both, must be one.
      // m or n must not be zero
      // Using != operator here for logical XOR equivalent behaviour
      return ((_m == 1U) != (_n == 1U)) &&
             ((_m != 0U) && (_n != 0U));
    }

    bool is_matrix() const
    {
      // m and n must both be greater than one
      return ((_m >= 2U) && (_n >= 2U));
    }

    type_dimension_t m() const
    {
      return _m;
    }

    type_dimension_t n() const
    {
      return _n;
    }

    friend bool operator==(const type_dimensions& td1, const type_dimensions& td2)
    {
      return ((td1._m == td2._m) && (td1._n == td2._n));
    }

    friend bool operator!=(const type_dimensions& td1, const type_dimensions& td2)
    {
      return !(td1 == td2);
    }

  private:
    // m - number of rows
    // n - number of columns
    type_dimension_t _m;
    type_dimension_t _n;
  };

  bool is_row_vector(const type_dimensions& td);
  bool is_column_vector(const type_dimensions& td);

  class type
  {
  public:
    type(type_id id, type_dimension_t m, type_dimension_t n) : _id(id), _dimensions(m, n) {}

    type transpose() const
    {
      return type(_id, _dimensions.n(), _dimensions.m());
    }

    type_id get_id() const
    {
      return _id;
    }

    const type_dimensions& get_dimensions() const
    {
      return _dimensions;
    }

    friend bool operator==(const type& t1, const type& t2)
    {
      return ((t1._id == t2._id) && (t1._dimensions == t2._dimensions));
    }

    friend bool operator!=(const type& t1, const type& t2)
    {
      return !(t1 == t2);
    }

    //TODO: add support for array types by adding a std::dynarray member (turns out dynarray will not be standardized so create something similar) to store each array dimension

  private:
    type_id _id;
    type_dimensions _dimensions;
  };

  template<typename T>
  struct type_helper : type
  {
    type_helper() : type(type_id_helper<T>::value, 1U, 1U) {}
  };

  // Specialization for void type
  template<>
  struct type_helper<void> : type
  {
    type_helper() : type(type_id_helper<void>::value, 0U, 0U) {}
  };

  // Specialization used by scalar types derived from sltl::basic
  //TODO: check/ensure that template parameter V is derived from sltl::basic
  template<template<typename> class V, typename T>
  struct type_helper<V<T>> : type
  {
    type_helper() : type(type_id_helper<T>::value, 1U, 1U) {}
  };

  // Specialization used by vector types derived from sltl::basic
  //TODO: check/ensure that template parameter V is derived from sltl::basic
  template<template<typename, type_dimension_t> class V, typename T, type_dimension_t D>
  struct type_helper<V<T, D>> : type
  {
    type_helper() : type(type_id_helper<T>::value, 1U, D) {}
  };

  // Specialization used by matrix types derived from sltl::basic
  //TODO: check/ensure that template parameter V is derived from sltl::basic
  template<template<typename, type_dimension_t, type_dimension_t> class V, typename T, type_dimension_t M, type_dimension_t N>
  struct type_helper<V<T, M, N>> : type
  {
    type_helper() : type(type_id_helper<T>::value, M, N) {}
  };

  enum operator_unary_id
  {
    id_increment_pre,
    id_increment_post,
    id_decrement_pre,
    id_decrement_post
  };

  bool is_prefix_operator(operator_unary_id id);
  bool is_postfix_operator(operator_unary_id id);

  enum operator_binary_id
  {
    id_addition,
    id_subtraction,
    id_multiplication,
    id_division,
    id_assignment,
    id_assignment_addition,
    id_assignment_subtraction,
    id_assignment_multiplication,
    id_assignment_division
  };

  enum conditional_id
  {
    id_if,
    id_else,
    id_else_if
  };

  enum keyword_id
  {
    id_return
  };

  std::wstring to_type_string(const type& t);
  std::wstring to_type_prefix_string(const type& t);

  const wchar_t* to_operator_unary_string(operator_unary_id id);
  const wchar_t* to_operator_binary_string(operator_binary_id id);
  const wchar_t* to_conditional_string(conditional_id id);
  const wchar_t* to_keyword_string(keyword_id id);
  const wchar_t* to_qualifier_string(core::qualifier_storage id);
  const wchar_t* to_qualifier_prefix_string(core::qualifier_storage id);
}
}
