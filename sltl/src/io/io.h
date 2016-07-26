#pragma once

#include "syntax/io_block.h"
#include "syntax/io_block_manager.h"

#include "core/qualifier.h"
#include "core/semantic.h"

#include "detail/variadic_algorithm.h"

#include <map>
#include <type_traits>


namespace sltl
{
namespace io
{
  class variable_key
  {
  public:
    variable_key(core::semantic s, core::semantic_index_t index) : _s(s), _idx(index) {}

    // Non-assignable
    variable_key& operator=(variable_key&&) = delete;
    variable_key& operator=(const variable_key&) = delete;

    bool operator<(const variable_key& rhs) const
    {
      return ((_s < rhs._s) || (!(rhs._s < _s) && (_idx < rhs._idx)));
    }

  private:
    const core::semantic _s;
    const core::semantic_index_t _idx;
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

    template<typename T, typename ...A>
    static variable_wrapper make(A&&... a)
    {
      return variable_wrapper(new impl<T>(std::forward<A>(a)...));
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
      template<typename ...A>
      impl(A&&... a) : t(std::forward<A>(a)...) {}

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

  template<typename T, core::semantic S = core::semantic::none, core::semantic_index_t N = 0U>
  struct variable
  {
    typedef T type;

    static const core::semantic _semantic = S;
    static const core::semantic_index_t _index = N;

    static core::semantic_pair create_semantic_pair()
    {
      return core::semantic_pair(_semantic, _index);
    }

    static variable_key create_key()
    {
      return variable_key(S, N);
    }
  };

  namespace detail
  {
    template<typename V>
    struct is_variable
    {
      static const bool value = false;
    };

    template<typename T, core::semantic S, core::semantic_index_t N>
    struct is_variable<variable<T, S, N>>
    {
      static const bool value = true;
    };
  }

  template<typename ...A>
  class block
  {
  public:
    block(block&& b) : _variable_map(std::move(b._variable_map)) {}

    block(core::qualifier_storage qualifier = core::qualifier_storage::out)
    {
      auto& io_block_manager = syntax::io_block_manager::get();
      auto& io_block = io_block_manager.add(qualifier);

      // Default construct each of the io_variables (i.e. the template parameters) type. This
      // then adds a new variable_declaration node to the current block (i.e. this io_block)
      init(qualifier);

      // Remove the block from the top of the block_manager's stack
      io_block.pop();
    }

    // Non-copyable and non-assignable
    block(const block&) = delete;
    block& operator=(block&&) = delete;
    block& operator=(const block&) = delete;

    template<core::semantic S, core::semantic_index_t N = 0U>
    auto get() -> decltype(get_impl<S, N, A...>())
    {
      return get_impl<S, N, A...>();
    }

    template<typename T>
    auto get() -> typename std::enable_if<detail::is_variable<T>::value, typename T::type::proxy>::type
    {
      return get_impl<T::_semantic, T::_index, A...>();
    }

  private:
    enum variable_none_t {};

    template<core::semantic S, core::semantic_index_t N, typename T, typename ...A2>
    auto get_impl() -> typename std::enable_if<(S == T::_semantic) && (N == T::_index), typename T::type::proxy>::type
    {
      return T::type::proxy(_variable_map.at(T::create_key()).get<T::type>());
    }

    template<core::semantic S, core::semantic_index_t N, typename T, typename ...A2>
    auto get_impl() -> typename std::enable_if<(S != T::_semantic) || (N != T::_index), decltype(get_impl<S, N, A2...>())>::type
    {
      return get_impl<S, N, A2...>();
    }

    template<core::semantic S, core::semantic_index_t N>
    auto get_impl() -> variable_none_t
    {
      static_assert(false, "sltl::io::block::get: the specified semantic and/or index is not valid for this io::block");
    }

    void init(core::qualifier_storage qualifier)
    {
      init_impl(qualifier, sltl::detail::variadic_guard<A...>());
    }

    template<typename ...A2>
    void init_impl(core::qualifier_storage, sltl::detail::variadic_guard<>) {}

    template<typename T, typename ...A2>
    void init_impl(core::qualifier_storage qualifier, sltl::detail::variadic_guard<T, A2...>)
    {
      init_type<T>(qualifier);
      init_impl(qualifier, sltl::detail::variadic_guard<A2...>());
    }

    template<typename T>
    auto init_type(core::qualifier_storage qualifier) -> typename std::enable_if<T::_semantic == core::semantic::none>::type
    {
      T::type t(qualifier);
    }

    template<typename T>
    auto init_type(core::qualifier_storage qualifier) -> typename std::enable_if<T::_semantic != core::semantic::none>::type
    {
      // Throw an exception if the insertion failed as this means the
      // block has more than one variable bound to the same semantic.
      if(!_variable_map.emplace(T::create_key(), variable_wrapper::make<T::type>(qualifier, T::create_semantic_pair())).second)
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    std::map<variable_key, variable_wrapper> _variable_map;
  };
}
}
