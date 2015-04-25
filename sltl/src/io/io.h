#pragma once

#include "syntax/io_block.h"
#include "syntax/io_block_manager.h"

#include "detail/variadic_algorithm.h"

#include <map>
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

  class variable_key
  {
  public:
    variable_key(semantic s, size_t index) : _s(s), _idx(index) {}

    bool operator<(const variable_key& rhs) const
    {
      return ((_s < rhs._s) || (!(rhs._s < _s) && (_idx < rhs._idx)));
    }

  private:
    semantic _s;
    size_t _idx;
  };

  // Types deriving from sltl::variable don't define virtual destructors so
  // shouldn't be used in a heterogeneous collection. The variable_wrapper
  // class uses type erasure to wrap these types and ensure they are destructed
  // correctly
  class variable_wrapper
  {
  public:
    variable_wrapper(variable_wrapper&& wrapper) : _impl(wrapper._impl)
    {
      wrapper._impl = nullptr;
    }

    // Non-copyable and non-assignable
    variable_wrapper(const variable_wrapper&) = delete;
    variable_wrapper& operator=(variable_wrapper&&) = delete;
    variable_wrapper& operator=(const variable_wrapper&) = delete;

    ~variable_wrapper()
    {
      delete _impl;
    }

    template<typename T>
    static variable_wrapper make()
    {
      return variable_wrapper(new impl<T>());
    }

    template<typename T>
    const T& get() const
    {
      return *static_cast<T*>(_impl->get());
    }

  private:
    struct impl_base
    {
      virtual ~impl_base()
      {
      }

      virtual void* get() = 0;
    };

    template<typename T>
    struct impl : impl_base
    {
      virtual void* get() override
      {
        return &t;
      }

      T t;
    };

    variable_wrapper(impl_base* ib) : _impl(ib)
    {
    }

    impl_base* _impl;
  };

  template<typename T, semantic S = semantic::none, size_t N = 0U>
  struct variable
  {
    typedef T type;

    static const semantic _semantic = S;
    static const size_t _index = N;

    static variable_key create_key()
    {
      return variable_key(S, N);
    }
  };

  template<typename ...A>
  class block
  {
  public:
    block(block&& b) : _variable_map(std::move(b._variable_map)) {}

    block(qualifier q = qualifier::out)
    {
      auto& io_block_manager = syntax::io_block_manager::get();
      auto& io_block = io_block_manager.add((q == qualifier::in) ? language::id_in : language::id_out);

      // Default construct each of the io_variables (i.e. the template parameters) type. This
      // then adds a new variable_declaration node to the current block (i.e. this io_block)
      init();

      // Remove the block from the top of the block_manager's stack
      io_block.pop();
    }

    // Non-copyable and non-assignable
    block(const block&) = delete;
    block& operator=(block&&) = delete;
    block& operator=(const block&) = delete;

    template<semantic S, size_t N = 0U>
    auto get() -> decltype(get_impl<S, N, A...>())
    {
      return get_impl<S, N, A...>();
    }

  private:
    enum variable_none_t {};

    template<semantic S, size_t N, typename T, typename ...A2>
    auto get_impl() -> typename std::enable_if<(S == T::_semantic) && (N == T::_index), typename T::type::proxy>::type
    {
      return T::type::proxy(_variable_map.at(T::create_key()).get<T::type>());
    }

    template<semantic S, size_t N, typename T, typename ...A2>
    auto get_impl() -> typename std::enable_if<(S != T::_semantic) || (N != T::_index), decltype(get_impl<S, N, A2...>())>::type
    {
      return get_impl<S, N, A2...>();
    }

    template<semantic S, size_t N>
    auto get_impl() -> variable_none_t
    {
      static_assert(false, "sltl::io::block::get: the specified semantic and/or index is not valid for this io::block");
    }

    void init()
    {
      init_impl(detail::variadic_guard<A...>());
    }

    template<typename ...A2>
    void init_impl(detail::variadic_guard<>) {}

    template<typename T, typename ...A2>
    void init_impl(detail::variadic_guard<T, A2...>)
    {
      init_type<T>();
      init_impl(detail::variadic_guard<A2...>());
    }

    template<typename T>
    auto init_type() -> typename std::enable_if<T::_semantic == semantic::none>::type
    {
      T::type();
    }

    template<typename T>
    auto init_type() -> typename std::enable_if<T::_semantic != semantic::none>::type
    {
      // Throw an exception if the insertion failed as this means the
      // block has more than one variable bound to the same semantic.
      if(!_variable_map.emplace(T::create_key(), variable_wrapper::make<T::type>()).second)
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    std::map<variable_key, variable_wrapper> _variable_map;
  };
}
}
