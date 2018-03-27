#pragma once

#include <mutex>
#include <memory>


namespace sltl
{
namespace detail
{
  enum class scope_t
  {
    process = 0,
    thread
  };

  template<typename T, scope_t S>
  class scoped_singleton;

  template<typename T>
  class scoped_singleton<T, scope_t::process> final
  {
  public:
    scoped_singleton() : _singleton_owner(), _is_creator(false)
    {
      std::lock_guard<std::mutex> lock(_singleton_mutex);

      if(auto owner = _singleton.lock())
      {
        _singleton_owner = std::move(owner);
      }
      else
      {
        // Avoid std::make_shared here as it uses a single 'fused' allocation that
        // cannot be freed until all references (both strong and weak) have been
        // released. This is an issue here as, once assigned to, _singleton will not
        // be reset to an 'empty' state
        _singleton_owner.reset(new T);
        _singleton = _singleton_owner;

        _is_creator = true;
      }
    }

    // Non-copyable, non-movable and non-assignable
    scoped_singleton(scoped_singleton&&) = delete;
    scoped_singleton(const scoped_singleton&) = delete;
    scoped_singleton& operator=(scoped_singleton&&) = delete;
    scoped_singleton& operator=(const scoped_singleton&) = delete;

    bool is_creator() const
    {
      return _is_creator;
    }

    T& operator*() const
    {
      return *_singleton_owner;
    }

    T* operator->() const
    {
      return _singleton_owner.get();
    }

    T* get() const
    {
      return _singleton_owner.get();
    }

  private:
    std::shared_ptr<T> _singleton_owner;

    static std::weak_ptr<T> _singleton;
    static std::mutex _singleton_mutex;

    bool _is_creator;
  };

  template<typename T>
  std::weak_ptr<T> sltl::detail::scoped_singleton<T, scope_t::process>::_singleton;

  template<typename T>
  std::mutex sltl::detail::scoped_singleton<T, scope_t::process>::_singleton_mutex;

  template<typename T>
  class scoped_singleton<T, scope_t::thread> final
  {
  public:
    scoped_singleton() : _is_creator(!_singleton)
    {
      if(_is_creator)
      {
        _singleton = std::make_unique<T>();
      }
    }

    ~scoped_singleton()
    {
      if(_is_creator)
      {
        _singleton.reset();
      }
    }

    // Non-copyable, non-movable and non-assignable
    scoped_singleton(scoped_singleton&&) = delete;
    scoped_singleton(const scoped_singleton&) = delete;
    scoped_singleton& operator=(scoped_singleton&&) = delete;
    scoped_singleton& operator=(const scoped_singleton&) = delete;

    bool is_creator() const
    {
      return _is_creator;
    }

    T& operator*() const
    {
      return *_singleton;
    }

    T* operator->() const
    {
      return _singleton.get();
    }

    T* get() const
    {
      return _singleton.get();
    }

    const bool _is_creator;

  private:
    thread_local static std::unique_ptr<T> _singleton;
  };

  template<typename T>
  thread_local std::unique_ptr<T> sltl::detail::scoped_singleton<T, scope_t::thread>::_singleton;
}
}
