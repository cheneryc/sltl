#pragma once

#include "syntax/io_block.h"
#include "syntax/io_block_manager.h"

#include <type_traits>


namespace sltl
{
namespace io
{
  enum class qualifier
  {
    in,
    out
  };

  enum class semantic
  {
    none,
    position,
    depth,
    colour,
    normal,
    texcoord,
    target
  };

  template<typename T, semantic S = semantic::none, size_t N = 0U>
  struct variable
  {
    typedef T type;

    static const semantic _semantic = S;
    static const size_t _index = N;
  };

  //TODO: validate the template parameters. Need to ensure that:
  //1. the semantic/index pairs are unique (i.e. not repeated in the parameter pack)
  //2. each pack member is a variable<> type
  //3. each variable has acceptable type and index template parameters based on the specified semantic
  template<typename ...A>
  class block
  {
  public:
    block(qualifier q = qualifier::out)
    {
      auto& io_block_manager = syntax::io_block_manager::get();
      auto& io_block = io_block_manager.add((q == qualifier::in) ? language::id_in : language::id_out);

      // Default construct each of the io_variables (i.e. the template parameters) type. This
      // then adds a new variable_declaration node to the current block (i.e. this io_block)
      helper<A...>::create();

      // Remove the block from the top of the block_manager's stack
      io_block.pop();
    }

    template<semantic S, size_t N = 0U>
    auto get() -> decltype(get_impl<S, N, A...>())
    {
      return get_impl<S, N, A...>();
    }

  private:
    enum variable_none_t {};

    template<semantic S, size_t N, typename T, typename ...A2>
    static auto get_impl() -> typename std::enable_if<(S == T::_semantic) && (N == T::_index), typename T::type>::type
    {
      return T::type();//TODO: the return type should actually be a proxy of some sort...
    }

    template<semantic S, size_t N, typename T, typename ...A2>
    static auto get_impl() -> typename std::enable_if<(S != T::_semantic) || (N != T::_index), decltype(get_impl<S, N, A2...>())>::type
    {
      return get_impl<S, N, A2...>();
    }

    template<semantic S, size_t N>
    static auto get_impl() -> variable_none_t
    {
      static_assert(false, "sltl::io::block::get: the specified semantic and/or index is not valid for this io::block");
    }

    // Ideally we would use detail::pass(A::type()...); instead of this
    // class, however function parameter evaluation order is undefined
    template<typename ...A2>
    struct helper;

    template<typename T, typename ...A2>
    struct helper<T, A2...>
    {
      static void create() { T::type(); helper<A2...>::create(); }
    };

    template<>
    struct helper<>
    {
      static void create() {}
    };
  };
}
}
