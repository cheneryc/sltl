#pragma once

#include "basic.h"
#include "scalar.h"
#include "vector.h"


namespace sltl
{
  //TODO: add bool template parameter for switching from row-major/vector to column-major/vector?

  template<typename T, size_t M, size_t N = M>
  class matrix : basic<sltl::matrix, T, M, N>
  {
    static_assert(is_real<T>::value, "sltl::matrix: Type T is not a valid template parameter type");
    static_assert((M >=2) && (M <= 4), "sltl::matrix: template parameter M is only valid for values of 2, 3 and 4");
    static_assert((N >=2) && (N <= 4), "sltl::matrix: template parameter N is only valid for values of 2, 3 and 4");

  public:
    matrix(proxy&& p) : basic(core::qualifier::make<core::storage_qualifier>(core::qualifier_storage::default), core::semantic_pair::none, p.move()) {}
    matrix(core::qualifier_storage qualifier = core::qualifier_storage::default, core::semantic_pair semantic = core::semantic_pair::none) : basic(core::qualifier::make<core::storage_qualifier>(qualifier), semantic) {}

    matrix(matrix&& m) : matrix(proxy(std::move(m))) {}
    matrix(const matrix& m) : matrix(proxy(m)) {}

    // The extra T2 argument stops this conflicting with the default constructor
    template<typename T2, typename ...A>
    explicit matrix(T2&& t, A&&... a) : matrix(proxy(syntax::expression::make<syntax::expression_list>(unpack<elements>(std::forward<T2>(t), std::forward<A>(a)...)))) {}

    proxy operator=(proxy&& p)
    {
      return make_proxy<syntax::assignment_operator>(language::id_assignment, make_reference(), p.move());
    }

    proxy operator=(matrix&& m)
    {
      return this->operator=(proxy(std::move(m)));
    }

    proxy operator=(const matrix& m)
    {
      return this->operator=(proxy(m));
    }

    //TODO: add a static const variable (or static function call) for getting the identity matrix?

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
      static_assert(!(N > 0), "sltl::matrix: too few components provided as arguments");
      static_assert(!(N < 0), "sltl::matrix: too many components provided as arguments");

      return syntax::expression_list();
    }

    static const size_t elements = M * N;
  };
}
