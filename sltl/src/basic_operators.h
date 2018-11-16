#pragma once

#include "scalar.h"
#include "vector.h"
#include "matrix.h"

#include "syntax/operator.h"

#include "detail/conditional_traits.h"


namespace sltl
{
  // Unary Operators

  namespace detail
  {
    // ++scalar, ++vector, ++matrix
    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_inc_pre(expression::expression<V, T, D...>&& exp) -> expression::expression<V, T, D...>
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: prefix increment and decrement operators are not valid if template parameter T is type bool");

      return expression::expression<V, T, D...>(syntax::expression::make<syntax::operator_unary>(language::id_increment_pre, exp.move()));
    }

    // --scalar, --vector, --matrix
    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_dec_pre(expression::expression<V, T, D...>&& exp) -> expression::expression<V, T, D...>
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: prefix increment and decrement operators are not valid if template parameter T is type bool");

      return expression::expression<V, T, D...>(syntax::expression::make<syntax::operator_unary>(language::id_decrement_pre, exp.move()));
    }

    // scalar++, vector++, matrix++
    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_inc_post(expression::expression<V, T, D...>&& exp) -> expression::expression<V, T, D...>
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: postfix increment and decrement operators are not valid if template parameter T is type bool");

      return expression::expression<V, T, D...>(syntax::expression::make<syntax::operator_unary>(language::id_increment_post, exp.move()));
    }

    // scalar--, vector--, matrix--
    template<template<typename, size_t...> class V, typename T, size_t ...D>
    auto as_proxy_dec_post(expression::expression<V, T, D...>&& exp) -> expression::expression<V, T, D...>
    {
      static_assert(detail::negate<std::is_same<T, bool>>::value, "sltl::basic: postfix increment and decrement operators are not valid if template parameter T is type bool");

      return expression::expression<V, T, D...>(syntax::expression::make<syntax::operator_unary>(language::id_decrement_post, exp.move()));
    }
  }

  template<typename T>
  auto operator++(T&& t) -> decltype(detail::as_proxy_inc_pre(expression::as_expression(std::forward<T>(t))))
  {
    return detail::as_proxy_inc_pre(expression::as_expression(std::forward<T>(t)));
  }

  template<typename T>
  auto operator--(T&& t) -> decltype(detail::as_proxy_dec_pre(expression::as_expression(std::forward<T>(t))))
  {
    return detail::as_proxy_dec_pre(expression::as_expression(std::forward<T>(t)));
  }

  template<typename T>
  auto operator++(T&& t, int) -> decltype(detail::as_proxy_inc_post(expression::as_expression(std::forward<T>(t))))
  {
    return detail::as_proxy_inc_post(expression::as_expression(std::forward<T>(t)));
  }

  template<typename T>
  auto operator--(T&& t, int) -> decltype(detail::as_proxy_dec_post(expression::as_expression(std::forward<T>(t))))
  {
    return detail::as_proxy_dec_post(expression::as_expression(std::forward<T>(t)));
  }

  // Binary Operators

  namespace detail
  {
    // Arithmetic operator behaviour - '+', '-', '*', '/' ('@' denotes all operators)

    // scalar @ scalar -> results in scalar arithmetic
    // scalar @ vector/matrix, vector/matrix @ scalar -> the scalar operation is applied independently to each element of the vector or matrix
    // vector + vector, vector - vector -> the operation is performed element-wise (vector operands must be the same size)
    // vector * matrix -> linear algebraic multiply between a row-vector and matrix, yielding a row-vector with size equal to the matrix columns (vector size and matrix rows must be equal)
    // matrix * vector -> linear algebraic multiply between a matrix and column-vector, yielding a column-vector with size equal to the matrix rows (matrix columns and vector size must be equal)
    // matrix * matrix -> linear algebraic multiply between a matrices, yielding a matrix with lhs matrix rows and rhs matrix columns (lhs matrix columns and rhs matrix rows must be equal)

    // Use sltl::element_wise to perform other element-wise arithmetic on matrix and vector operands

    // Addition operators

    // scalar + scalar
    template<typename T>
    auto as_proxy_add(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_addition, lhs.move(), rhs.move()));
    }

    // scalar += scalar
    template<typename T>
    auto as_proxy_assignment_add(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_assignment_addition, lhs.move(), rhs.move()));
    }

    // vector + vector
    template<typename T, size_t D>
    auto as_proxy_add(expression::expression<vector, T, D>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_addition, lhs.move(), rhs.move()));
    }

    // vector += vector
    template<typename T, size_t D>
    auto as_proxy_assignment_add(expression::expression<vector, T, D>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_assignment_addition, lhs.move(), rhs.move()));
    }

    // Subtraction operators

    // scalar - scalar
    template<typename T>
    auto as_proxy_sub(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_subtraction, lhs.move(), rhs.move()));
    }

    // scalar -= scalar
    template<typename T>
    auto as_proxy_assignment_sub(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_assignment_subtraction, lhs.move(), rhs.move()));
    }

    // vector - vector
    template<typename T, size_t D>
    auto as_proxy_sub(expression::expression<vector, T, D>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_subtraction, lhs.move(), rhs.move()));
    }

    // vector -= vector
    template<typename T, size_t D>
    auto as_proxy_assignment_sub(expression::expression<vector, T, D>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_assignment_subtraction, lhs.move(), rhs.move()));
    }

    // Multiplication operators

    // scalar * scalar
    template<typename T>
    auto as_proxy_mul(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_multiplication, lhs.move(), rhs.move()));
    }

    // scalar * vector
    template<typename T, size_t D>
    auto as_proxy_mul(expression::expression<scalar, T>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_scalar_vector_multiplication, lhs.move(), rhs.move()));
    }

    // vector * scalar
    template<typename T, size_t D>
    auto as_proxy_mul(expression::expression<vector, T, D>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_vector_scalar_multiplication, lhs.move(), rhs.move()));
    }

    // scalar * matrix
    template<typename T, size_t M, size_t N>
    auto as_proxy_mul(expression::expression<scalar, T>&& lhs, expression::expression<matrix, T, M, N>&& rhs) -> expression::expression<matrix, T, M, N>
    {
      return expression::expression<matrix, T, M, N>(syntax::expression::make<syntax::operator_binary>(language::id_scalar_matrix_multiplication, lhs.move(), rhs.move()));
    }

    // matrix * scalar
    template<typename T, size_t M, size_t N>
    auto as_proxy_mul(expression::expression<matrix, T, M, N>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<matrix, T, M, N>
    {
      return expression::expression<matrix, T, M, N>(syntax::expression::make<syntax::operator_binary>(language::id_matrix_scalar_multiplication, lhs.move(), rhs.move()));
    }

    //TODO: disable if lhs operand is a column-vector
    // vector * matrix
    template<typename T, size_t M, size_t N>
    auto as_proxy_mul(expression::expression<vector, T, M>&& lhs, expression::expression<matrix, T, M, N>&& rhs) -> expression::expression<vector, T, N>
    {
      return expression::expression<vector, T, N>(syntax::expression::make<syntax::operator_binary>(language::id_matrix_multiplication, lhs.move(), rhs.move()));
    }

    //TODO: disable if rhs operand is a row-vector
    // matrix * vector
    template<typename T, size_t M, size_t N>
    auto as_proxy_mul(expression::expression<matrix, T, M, N>&& lhs, expression::expression<vector, T, N>&& rhs) -> expression::expression<vector, T, M>
    {
      return expression::expression<vector, T, N>(syntax::expression::make<syntax::operator_binary>(language::id_matrix_multiplication, lhs.move(), rhs.move()));
    }

    // matrix * matrix
    template<typename T, size_t M, size_t N, size_t P>
    auto as_proxy_mul(expression::expression<matrix, T, M, N>&& lhs, expression::expression<matrix, T, N, P>&& rhs) -> expression::expression<matrix, T, M, P>
    {
      return expression::expression<matrix, T, M, P>(syntax::expression::make<syntax::operator_binary>(language::id_matrix_multiplication, lhs.move(), rhs.move()));
    }

    // Division operators

    // scalar / scalar
    template<typename T>
    auto as_proxy_div(expression::expression<scalar, T>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<scalar, T>
    {
      return expression::expression<scalar, T>(syntax::expression::make<syntax::operator_binary>(language::id_division, lhs.move(), rhs.move()));
    }

    // scalar / vector
    template<typename T, size_t D>
    auto as_proxy_div(expression::expression<scalar, T>&& lhs, expression::expression<vector, T, D>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_scalar_vector_division, lhs.move(), rhs.move()));
    }

    // vector / scalar
    template<typename T, size_t D>
    auto as_proxy_div(expression::expression<vector, T, D>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<vector, T, D>
    {
      return expression::expression<vector, T, D>(syntax::expression::make<syntax::operator_binary>(language::id_vector_scalar_division, lhs.move(), rhs.move()));
    }

    // scalar / matrix
    template<typename T, size_t M, size_t N>
    auto as_proxy_div(expression::expression<scalar, T>&& lhs, expression::expression<matrix, T, M, N>&& rhs) -> expression::expression<matrix, T, M, N>
    {
      return expression::expression<matrix, T, M, N>(syntax::expression::make<syntax::operator_binary>(language::id_scalar_matrix_division, lhs.move(), rhs.move()));
    }

    // matrix / scalar
    template<typename T, size_t M, size_t N>
    auto as_proxy_div(expression::expression<matrix, T, M, N>&& lhs, expression::expression<scalar, T>&& rhs) -> expression::expression<matrix, T, M, N>
    {
      return expression::expression<matrix, T, M, N>(syntax::expression::make<syntax::operator_binary>(language::id_matrix_scalar_division, lhs.move(), rhs.move()));
    }
  }

  template<typename T1, typename T2>
  auto operator+(T1&& lhs, T2&& rhs) -> decltype(detail::as_proxy_add(expression::as_expression(std::forward<T1>(lhs)), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(std::forward<T1>(lhs));
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_add(std::move(lhs_proxy), std::move(rhs_proxy));
  }

  // lhs operand is an l-value reference to match the behaviour of the += operator for fundamental arithmetic types
  template<typename T1, typename T2>
  auto operator+=(T1& lhs, T2&& rhs) -> decltype(detail::as_proxy_assignment_add(expression::as_expression(lhs), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(lhs);
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_assignment_add(std::move(lhs_proxy), std::move(rhs_proxy));
  }

  template<typename T1, typename T2>
  auto operator-(T1&& lhs, T2&& rhs) -> decltype(detail::as_proxy_sub(expression::as_expression(std::forward<T1>(lhs)), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(std::forward<T1>(lhs));
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_sub(std::move(lhs_proxy), std::move(rhs_proxy));
  }

  // lhs operand is an l-value reference to match the behaviour of the -= operator for fundamental arithmetic types
  template<typename T1, typename T2>
  auto operator-=(T1& lhs, T2&& rhs) -> decltype(detail::as_proxy_assignment_sub(expression::as_expression(lhs), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(lhs);
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_assignment_sub(std::move(lhs_proxy), std::move(rhs_proxy));
  }

  template<typename T1, typename T2>
  auto operator*(T1&& lhs, T2&& rhs) -> decltype(detail::as_proxy_mul(expression::as_expression(std::forward<T1>(lhs)), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(std::forward<T1>(lhs));
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_mul(std::move(lhs_proxy), std::move(rhs_proxy));
  }

  template<typename T1, typename T2>
  auto operator/(T1&& lhs, T2&& rhs) -> decltype(detail::as_proxy_div(expression::as_expression(std::forward<T1>(lhs)), expression::as_expression(std::forward<T2>(rhs))))
  {
    auto lhs_proxy = expression::as_expression(std::forward<T1>(lhs));
    auto rhs_proxy = expression::as_expression(std::forward<T2>(rhs));

    return detail::as_proxy_div(std::move(lhs_proxy), std::move(rhs_proxy));
  }
}
