#pragma once

#include <string>


namespace sltl
{
namespace language
{
  enum type_id
  {
    id_unknown,
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

    type& operator=(const type&) = delete;
  };

  template<typename T, size_t D>
  struct type_helper : public type
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

  std::wstring to_type_string(const type& t);
  std::wstring to_prefix_string(const type& t);

  const wchar_t* to_operator_string(operator_id id);
  const wchar_t* to_assignment_operator_string(assignment_operator_id id);
  const wchar_t* to_conditional_string(conditional_id id);
}
}
