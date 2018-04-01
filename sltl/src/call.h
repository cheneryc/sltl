#pragma once

#include "syntax/function_call.h"
#include "syntax/function_definition.h"

#include "detail/function_traits.h"


namespace sltl
{
  template<typename Fn>
  auto call(Fn fn) -> typename std::enable_if< std::is_same<typename sltl::detail::function_traits<Fn>::return_t, void>::value, void>::type
  {
    syntax::get_current_block().add<syntax::expression_statement>(syntax::expression::make<syntax::function_call>(syntax::function_manager_guard()->emplace(fn)));
  }

  template<typename Fn>
  auto call(Fn fn) -> typename std::enable_if<!std::is_same<typename sltl::detail::function_traits<Fn>::return_t, void>::value, typename sltl::detail::function_traits<Fn>::return_t::proxy>::type
  {
    return sltl::detail::function_traits<Fn>::return_t::proxy(syntax::expression::make<syntax::function_call>(syntax::function_manager_guard()->emplace(fn)));
  }
}
