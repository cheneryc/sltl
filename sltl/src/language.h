#pragma once

#include <string>


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

  struct type
  {
    type(type_id id, size_t components) : _components(components), _id(id) {}

    const size_t _components;
    const type_id _id;

    friend bool operator==(const type& t1, const type& t2)
    {
      return ((t1._id == t2._id) && (t1._components == t2._components));
    }

    friend bool operator!=(const type& t1, const type& t2)
    {
      return !(t1 == t2);
    }

    type& operator=(const type&) = delete;
  };

  template<typename T, size_t D = 1>
  struct type_helper : public type
  {
    type_helper() : type(type_id_helper<T>::value, D) {}
  };

  // Specialization for void type, stops vectors of void type being defined
  template<size_t D>
  struct type_helper<void, D> : public type
  {
    static_assert(D == 1, "sltl::language::type_helper: invalid template parameter D for type void, this parameter must be 1");

    type_helper() : type(type_id_helper<void>::value, D) {}
  };

  // Specialization for scalar types derived from sltl::basic
  template<template<typename> class V, typename T>
  struct type_helper<V<T>> : public type
  {
    type_helper() : type(type_id_helper<T>::value, 1) {}
  };

  // Specialization for vector types derived from sltl::basic
  template<template<typename, size_t> class V, typename T, size_t D>
  struct type_helper<V<T, D>> : public type
  {
    type_helper() : type(type_id_helper<T>::value, D) {}
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
  std::wstring to_prefix_string(const type& t);

  const wchar_t* to_operator_string(operator_id id);
  const wchar_t* to_assignment_operator_string(assignment_operator_id id);
  const wchar_t* to_conditional_string(conditional_id id);
  const wchar_t* to_keyword_string(keyword_id id);
}
}
