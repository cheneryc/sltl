#pragma once

#include "syntax/function_call.h"
#include "syntax/function_definition.h"

#include "detail/function_traits.h"


namespace sltl
{
  namespace detail
  {
    template<typename T, typename ...A>
    syntax::expression_list get_args(T&& t, A&& ...a)
    {
      return syntax::expression_list(expression::as_expression(std::forward<T>(t)).move(), get_args(std::forward<A>(a)...));
    }

    syntax::expression_list get_args()
    {
      return {};
    }
  }

  template<typename Fn, typename ...T>
  auto call(Fn fn, T&& ...t) -> typename std::enable_if< std::is_same<typename sltl::detail::function_traits<Fn>::return_t, void>::value, void>::type
  {
    syntax::get_current_block().add<syntax::expression_statement>(syntax::expression::make<syntax::function_call>(syntax::function_manager_guard()->emplace(fn), detail::get_args(std::forward<T>(t)...)));
  }

  template<typename Fn, typename ...T>
  auto call(Fn fn, T&& ...t) -> typename std::enable_if<!std::is_same<typename sltl::detail::function_traits<Fn>::return_t, void>::value, typename sltl::detail::function_traits<Fn>::return_t::proxy>::type
  {
    return sltl::detail::function_traits<Fn>::return_t::proxy(syntax::expression::make<syntax::function_call>(syntax::function_manager_guard()->emplace(fn), detail::get_args(std::forward<T>(t)...)));
  }
}
