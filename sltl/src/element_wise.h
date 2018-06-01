#pragma once

#include "vector.h"
#include "matrix.h"

#include "syntax/operator.h"


namespace sltl
{
  template<typename T>
  class element_wise_ops;

  // element-wise operators - specialization for vector types ('*' and '/' operators defined)

  template<typename T, size_t D>
  class element_wise_ops<vector<T, D>>
  {
  public:
    explicit element_wise_ops(syntax::expression::ptr&& exp) : _exp(std::move(exp)) {}

    friend typename vector<T, D>::proxy operator*(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return vector<T, D>::make_proxy<syntax::operator_binary>(language::id_element_wise_multiplication, std::move(lhs._exp), std::move(rhs._exp));
    }

    friend typename vector<T, D>::proxy operator/(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return vector<T, D>::make_proxy<syntax::operator_binary>(language::id_element_wise_division, std::move(lhs._exp), std::move(rhs._exp));
    }

  private:
    syntax::expression::ptr _exp;
  };

  // element-wise operators - specialization for matrix types ('+', '-', '*' and '/' operators defined)

  template<typename T, size_t M, size_t N>
  class element_wise_ops<matrix<T, M, N>>
  {
  public:
    explicit element_wise_ops(syntax::expression::ptr&& exp) : _exp(std::move(exp)) {}

    friend typename matrix<T, M, N>::proxy operator+(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return matrix<T, M, N>::make_proxy<syntax::operator_binary>(language::id_element_wise_addition, std::move(lhs._exp), std::move(rhs._exp));
    }

    friend typename matrix<T, M, N>::proxy operator-(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return matrix<T, M, N>::make_proxy<syntax::operator_binary>(language::id_element_wise_subtraction, std::move(lhs._exp), std::move(rhs._exp));
    }

    friend typename matrix<T, M, N>::proxy operator*(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return matrix<T, M, N>::make_proxy<syntax::operator_binary>(language::id_element_wise_multiplication, std::move(lhs._exp), std::move(rhs._exp));
    }

    friend typename matrix<T, M, N>::proxy operator/(element_wise_ops&& lhs, element_wise_ops&& rhs)
    {
      return matrix<T, M, N>::make_proxy<syntax::operator_binary>(language::id_element_wise_division, std::move(lhs._exp), std::move(rhs._exp));
    }

  private:
    syntax::expression::ptr _exp;
  };

  template<typename T, size_t D>
  auto element_wise(expression::expression<vector, T, D>&& exp) -> element_wise_ops<vector<T, D>>
  {
    return element_wise_ops<vector<T, D>>(exp.move());
  }

  template<typename T, size_t M, size_t N>
  auto element_wise(expression::expression<matrix, T, M, N>&& exp) -> element_wise_ops<matrix<T, M, N>>
  {
    return element_wise_ops<matrix<T, M, N>>(exp.move());
  }

  template<typename T>
  auto element_wise(T&& t) -> decltype(element_wise(expression::as_expression(std::forward<T>(t))))
  {
    return element_wise(expression::as_expression(std::forward<T>(t)));
  }
}
