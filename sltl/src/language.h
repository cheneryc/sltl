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

  typedef size_t type_index_t;

  class type
  {
  public:
    type(type_id id, std::vector<type_index_t>&& dimensions) : _id(id), _dimensions(std::move(dimensions))
    {
      assert(get_dimension_count() > 0U);
      assert(std::all_of(cbegin(), cend(), [](type_index_t d){ return (d > 0U) && (d <= 4U); }));
    }

    type& operator=(const type&) = delete;

    typedef std::vector<type_index_t>::const_iterator const_iterator;
    typedef std::vector<type_index_t>::const_reverse_iterator const_reverse_iterator;

    const_iterator cbegin() const
    {
      return _dimensions.cbegin();
    }

    const_iterator cend() const
    {
      return _dimensions.cend();
    }

    const_reverse_iterator crbegin() const
    {
      return _dimensions.crbegin();
    }

    const_reverse_iterator crend() const
    {
      return _dimensions.crend();
    }

    type_index_t front() const
    {
      return _dimensions.front();
    }

    type_index_t back() const
    {
      return _dimensions.back();
    }

    size_t get_dimension_count() const
    {
      return _dimensions.size();
    }

    friend bool operator==(const type& t1, const type& t2)
    {
      return ((t1._id == t2._id) && (t1._dimensions.size() == t2._dimensions.size()) && std::equal(t1._dimensions.begin(), t1._dimensions.end(), t2._dimensions.begin()));
    }

    friend bool operator!=(const type& t1, const type& t2)
    {
      return !(t1 == t2);
    }

    const type_id _id;

  private:
    //TODO: the vector isn't ideal, would prefer dynarray when (if?) it becomes available
    const std::vector<type_index_t> _dimensions;
  };

  template<typename T>
  struct type_helper : public type
  {
    type_helper(std::initializer_list<type_index_t> l = {1U}) : type(type_id_helper<T>::value, std::vector<type_index_t>(l.begin(), l.end())) {}
  };

  // Specialization for void type, stops vectors and matrices of void type from being defined
  template<>
  struct type_helper<void> : public type
  {
    type_helper() : type(type_id_helper<void>::value, {1U}) {}
  };

  // Specialization for scalar types derived from sltl::basic
  template<template<typename> class V, typename T>
  struct type_helper<V<T>> : public type_helper<T>
  {
    type_helper() : type_helper<T>() {}
  };

  // Specialization for vector and matrix types derived from sltl::basic
  template<template<typename, type_index_t...> class V, typename T, type_index_t ...D>
  struct type_helper<V<T, D...>> : public type_helper<T>
  {
    type_helper() : type_helper<T>({D...}) {}
  };

  enum operator_id
  {
    id_addition,
    id_subtraction,
    id_multiplication,
    id_division
  };

  enum assignment_operator_id
  {
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

  const wchar_t* to_operator_string(operator_id id);
  const wchar_t* to_assignment_operator_string(assignment_operator_id id);
  const wchar_t* to_conditional_string(conditional_id id);
  const wchar_t* to_keyword_string(keyword_id id);
  const wchar_t* to_qualifier_string(core::qualifier_storage id);
  const wchar_t* to_qualifier_prefix_string(core::qualifier_storage id);
}
}
