#pragma once

#include "scalar.h"


namespace sltl
{
  template<typename T, size_t D>
  class vector : public basic<sltl::vector, T, D>
  {
    static_assert((D >=2) && (D <= 4), "sltl::vector: template parameter D is only valid for values of 2, 3 and 4");

  public:
    vector() : basic() {}
    vector(proxy&& p) : basic(p.move()) {}

    vector(vector&& v) : vector(proxy(std::move(v))) {}
    vector(const vector& v) : vector(proxy(v)) {}

    // The extra T2 argument stops this conflicting with the default constructor
    template<typename T2, typename ...A>
    explicit vector(T2&& t, A&&... a) : vector(proxy(syntax::expression::make<syntax::expression_list>(unpack<D>(std::forward<T2>(t), std::forward<A>(a)...)))) {}

    proxy operator=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment, make_reference(), p.move());
    }

    proxy operator=(vector&& v)
    {
      return this->operator=(proxy(std::move(v)));
    }

    proxy operator=(const vector& v)
    {
      return this->operator=(proxy(v));
    }

  private:
    template<int N, typename ...A>
    syntax::expression_list unpack(typename scalar<T>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 1>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename sltl::vector<T, 2>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 2>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename sltl::vector<T, 3>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 3>(std::forward<A>(a)...));
    }

    template<int N, typename ...A>
    syntax::expression_list unpack(typename sltl::vector<T, 4>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 4>(std::forward<A>(a)...));
    }

    template<int N>
    syntax::expression_list unpack()
    {
      static_assert(!(N > 0), "sltl::vector: too few vector components provided as arguments");
      static_assert(!(N < 0), "sltl::vector: too many vector components provided as arguments");

      return syntax::expression_list();
    }
  };
}
