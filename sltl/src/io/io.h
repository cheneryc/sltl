#pragma once

#include "syntax/io_block.h"
#include "syntax/io_block_manager.h"

#include "syntax/block_guard.h"

#include "core/qualifier.h"
#include "core/semantic.h"

// These includes are required as variable_system and variable_transform have known
// types (vector & scalar for system variables and matrix for transform variables)
//TODO:  as io.h is now dependent on these headers it should no longer be in a separate 'io' folder
#include "../vector.h"
#include "../matrix.h"

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
    variable_key(const variable_key& key) : _s(key._s), _idx(key._idx) {}

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

  template<core::semantic_system S = core::semantic_system::none, core::semantic_index_t N = 0U, typename T = float>
  struct variable_system : variable<vector<T, 4>, core::semantic::system, core::detail::to_semantic_index(S, N)>
  {
    static_assert(is_real<T>::value, "sltl::io::variable_system: Type T is not a valid template parameter type");
  };

  // Partial specialization for core::semantic_system::none - variable semantic mapped to core::semantic::none
  template<core::semantic_index_t N, typename T>
  struct variable_system<core::semantic_system::none, N, T> : variable<vector<T, 4>, core::semantic::none, N> {}; //TODO: is there a more suitable type than vector<T, 4>?

  // Partial specialization for core::semantic_system::depth - variable type must be scalar
  template<core::semantic_index_t N, typename T>
  struct variable_system<core::semantic_system::depth, N, T> : variable<scalar<T>, core::semantic::system, core::detail::to_semantic_index(core::semantic_system::depth, N)> {};

  template<core::semantic_transform S = core::semantic_transform::none, typename T = float>
  struct variable_transform : variable<matrix<T, 4, 4>, core::semantic::transform, core::detail::to_semantic_index(S)>
  {
    static_assert(is_real<T>::value, "sltl::io::variable_transform: Type T is not a valid template parameter type");
  };

  // Partial specialization for core::semantic_transform::none - variable semantic mapped to core::semantic::none
  template<typename T>
  struct variable_transform<core::semantic_transform::none, T> : variable<matrix<T, 4>, core::semantic::none> {}; //TODO: is there a more suitable type and index than matrix<T, 4> and zero respectively?

  // Partial specialization for core::semantic_transform::normal - variable type is a 3x3 matrix
  template<typename T>
  struct variable_transform<core::semantic_transform::normal, T> : variable<matrix<T, 3>, core::semantic::transform, core::detail::to_semantic_index(core::semantic_transform::normal)> {};

  namespace detail
  {
    enum variable_none_t {};

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

    template<core::semantic_system S, core::semantic_index_t N>
    struct is_variable<variable_system<S, N>>
    {
      static const bool value = true;
    };

    template<core::semantic_transform S>
    struct is_variable<variable_transform<S>>
    {
      static const bool value = true;
    };

    template<typename V, core::semantic S, core::semantic_index_t N>
    constexpr bool is_variable_match()
    {
      return (S == V::_semantic) && (N == V::_index);
    };
  }

  template<typename ...A>
  class block
  {
  public:
    block(block&& b) : _variable_map(std::move(b._variable_map)) {}

    block(core::qualifier_storage qualifier = core::qualifier_storage::out)
    {
      syntax::io_block_manager_guard io_block_manager;
      syntax::io_block& io_block = io_block_manager->add(qualifier);

      // The guard will call push() and pop() on this io_block instance. These function calls
      // override the 'current block' with this io_block for the duration of the guard object
      syntax::block_guard guard(io_block, [this](){ init(); });
    }

    // Non-copyable and non-assignable
    block(const block&) = delete;
    block& operator=(block&&) = delete;
    block& operator=(const block&) = delete;

    template<typename T>
    auto get() -> typename std::enable_if<detail::is_variable<T>::value, typename T::type::proxy>::type
    {
      return get_impl<T::_semantic, T::_index, A...>();
    }

    template<core::semantic S, core::semantic_index_t N = 0U>
    auto get() -> decltype(get_impl<S, N, A...>())
    {
      return get_impl<S, N, A...>();
    }

    template<core::semantic_system S, core::semantic_index_t N = 0U>
    auto get() -> decltype(get<variable_system<S, N>>())
    {
      return get<variable_system<S, N>>();
    }

    template<core::semantic_transform S>
    auto get() -> decltype(get<variable_transform<S>>())
    {
      return get<variable_transform<S>>();
    }

  private:
    template<core::semantic S, core::semantic_index_t N, typename V, typename ...A2>
    auto get_impl() -> typename std::enable_if< detail::is_variable_match<V, S, N>(), typename V::type::proxy>::type
    {
      return V::type::proxy(_variable_map.at(V::create_key()).get<V::type>());
    }

    template<core::semantic S, core::semantic_index_t N, typename V, typename ...A2>
    auto get_impl() -> typename std::enable_if<!detail::is_variable_match<V, S, N>(), decltype(get_impl<S, N, A2...>())>::type
    {
      return get_impl<S, N, A2...>();
    }

    template<core::semantic S, core::semantic_index_t N>
    auto get_impl() -> detail::variable_none_t
    {
      static_assert(false, "sltl::io::block::get: the specified semantic and/or index is not valid for this io::block");
    }

    void init()
    {
      // Default construct each io variable's (i.e. the template parameters) type. This
      // will add a new variable_declaration node to the current block (i.e. this io_block)
      init_impl(sltl::detail::variadic_guard<A...>());
    }

    template<typename ...A2>
    void init_impl(sltl::detail::variadic_guard<>) {}

    template<typename T, typename ...A2>
    void init_impl(sltl::detail::variadic_guard<T, A2...>)
    {
      init_type<T>();
      init_impl(sltl::detail::variadic_guard<A2...>());
    }

    template<typename T>
    auto init_type() -> typename std::enable_if<T::_semantic == core::semantic::none>::type
    {
      T::type t;
    }

    template<typename T>
    auto init_type() -> typename std::enable_if<T::_semantic != core::semantic::none>::type
    {
      // Throw an exception if the insertion failed as this means the
      // block has more than one variable bound to the same semantic.
      if(!_variable_map.emplace(T::create_key(), variable_wrapper::make<T::type>(T::create_semantic_pair())).second)
      {
        throw std::exception();//TODO: exception type and message
      }
    }

    std::map<variable_key, variable_wrapper> _variable_map;
  };
}
}
