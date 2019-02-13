#include <gtest/gtest.h>

#include "detail/scoped_singleton.h"

#include <atomic>
#include <future>


namespace
{
  template<typename T>
  struct test_singleton
  {
    test_singleton() : _instance(++count_created)
    {
      ++count_current;
    }

    ~test_singleton()
    {
      --count_current;
    }

    const size_t _instance;

    static std::atomic<size_t> count_current;
    static std::atomic<size_t> count_created;
  };

  template<typename T>
  std::atomic<size_t> test_singleton<T>::count_current;

  template<typename T>
  std::atomic<size_t> test_singleton<T>::count_created;
}

TEST(scoped_singleton, process)
{
  // Provides a unique singleton type for this test which ensures static
  // counts are zero, regardless of whether other tests have run or not
  struct tag;

  using test_singleton_guard = sltl::detail::scoped_singleton<test_singleton<tag>, sltl::detail::scope_t::process>;

  ASSERT_EQ(0U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager1;

    auto address1 = manager1.get();

    ASSERT_EQ(1U, manager1->_instance);
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);

    {
      test_singleton_guard manager1_sub;

      ASSERT_EQ(1U, manager1_sub->_instance);
      ASSERT_EQ(1U, test_singleton<tag>::count_created);
      ASSERT_EQ(1U, test_singleton<tag>::count_current);

      auto address1_sub = manager1_sub.get();

      ASSERT_TRUE(address1);
      ASSERT_TRUE(address1_sub);
      ASSERT_EQ(address1, address1_sub);
    }

    ASSERT_EQ(1U, manager1->_instance);
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);
  }

  ASSERT_EQ(1U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager2;

    ASSERT_EQ(2U, manager2->_instance);
    ASSERT_EQ(2U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);
  }

  ASSERT_EQ(2U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);
}

TEST(scoped_singleton, process_parallel)
{
  // Provides a unique singleton type for this test which ensures static
  // counts are zero, regardless of whether other tests have run or not
  struct tag;

  using test_singleton_guard = sltl::detail::scoped_singleton<test_singleton<tag>, sltl::detail::scope_t::process>;

  auto fn = []()
  {
    test_singleton_guard manager_thread;

    const size_t count_pre  = test_singleton<tag>::count_current;
    std::this_thread::yield();
    const size_t count_post = test_singleton<tag>::count_current;

    return std::make_pair(count_pre, count_post);
  };

  {
    std::vector<std::future<std::pair<size_t, size_t>>> futures;

    for(size_t i = {}; i < 100; ++i)
    {
      futures.push_back(std::async(std::launch::async, fn));
    }

    for(auto& future : futures)
    {
      const std::pair<size_t, size_t> counts = future.get();

      EXPECT_EQ(1U, std::get<0>(counts));
      EXPECT_EQ(1U, std::get<1>(counts));
    }
  }

  ASSERT_EQ(0U, test_singleton<tag>::count_current);
}

TEST(scoped_singleton, process_parallel_is_creator)
{
  // Provides a unique singleton type for this test which ensures static
  // counts are zero, regardless of whether other tests have run or not
  struct tag;

  using test_singleton_guard = sltl::detail::scoped_singleton<test_singleton<tag>, sltl::detail::scope_t::process>;

  auto fn = []()
  {
    return test_singleton_guard().is_creator();
  };

  ASSERT_EQ(0U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager;

    ASSERT_TRUE(manager.is_creator());
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);

    std::vector<std::future<bool>> futures;

    for(size_t i = {}; i < 100; ++i)
    {
      futures.push_back(std::async(std::launch::async, fn));
    }

    for(auto& future : futures)
    {
      EXPECT_FALSE(future.get());
    }

    futures.clear();

    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);
  }

  ASSERT_EQ(1U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);
}

TEST(scoped_singleton, thread)
{
  // Provides a unique singleton type for this test which ensures static
  // counts are zero, regardless of whether other tests have run or not
  struct tag;

  using test_singleton_guard = sltl::detail::scoped_singleton<test_singleton<tag>, sltl::detail::scope_t::thread>;

  ASSERT_EQ(0U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager1;

    auto address1 = manager1.get();

    ASSERT_EQ(1U, manager1->_instance);
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);

    {
      test_singleton_guard manager1_sub;

      ASSERT_EQ(1U, manager1_sub->_instance);
      ASSERT_EQ(1U, test_singleton<tag>::count_created);
      ASSERT_EQ(1U, test_singleton<tag>::count_current);

      auto address1_sub = manager1_sub.get();

      ASSERT_TRUE(address1);
      ASSERT_TRUE(address1_sub);
      ASSERT_EQ(address1, address1_sub);
    }

    ASSERT_EQ(1U, manager1->_instance);
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);
  }

  ASSERT_EQ(1U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager2;

    ASSERT_EQ(2U, manager2->_instance);
    ASSERT_EQ(2U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);
  }

  ASSERT_EQ(2U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);
}

TEST(scoped_singleton, thread_parallel_is_creator)
{
  // Provides a unique singleton type for this test which ensures static
  // counts are zero, regardless of whether other tests have run or not
  struct tag;

  using test_singleton_guard = sltl::detail::scoped_singleton<test_singleton<tag>, sltl::detail::scope_t::thread>;

  auto fn = []()
  {
    return test_singleton_guard().is_creator();
  };

  ASSERT_EQ(0U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U, test_singleton<tag>::count_current);

  {
    test_singleton_guard manager;

    ASSERT_TRUE(manager.is_creator());
    ASSERT_EQ(1U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U, test_singleton<tag>::count_current);

    std::vector<std::future<bool>> futures;

    for(size_t i = {}; i < 100; ++i)
    {
      futures.push_back(std::async(std::launch::async, fn));
    }

    for(auto& future : futures)
    {
      EXPECT_TRUE(future.get());
    }

    futures.clear();

    ASSERT_EQ(101U, test_singleton<tag>::count_created);
    ASSERT_EQ(1U,   test_singleton<tag>::count_current);
  }

  ASSERT_EQ(101U, test_singleton<tag>::count_created);
  ASSERT_EQ(0U,   test_singleton<tag>::count_current);
}
