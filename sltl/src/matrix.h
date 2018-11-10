#pragma once

#include "basic.h"
#include "scalar.h"
#include "vector.h"

#include "syntax/constructor_call.h"

#include "detail/variadic_traits.h"
#include "detail/conditional_traits.h"


namespace sltl
{
  //TODO: add bool template parameter for switching from row-major/vector to column-major/vector?

  template<typename T, size_t M, size_t N = M>
  class matrix : public basic<sltl::matrix, T, M, N>
  {
    static_assert(is_real<T>::value, "sltl::matrix: Type T is not a valid template parameter type");
    static_assert((M >=2) && (M <= 4), "sltl::matrix: template parameter M is only valid for values of 2, 3 and 4");
    static_assert((N >=2) && (N <= 4), "sltl::matrix: template parameter N is only valid for values of 2, 3 and 4");

    typedef basic<sltl::matrix, T, M, N> super_t;

  public:
    typedef typename super_t::proxy proxy;

    matrix(proxy&& p) : super_t(p.move()) {}
    matrix(core::semantic_pair semantic = core::semantic_pair::none) : super_t(semantic) {}

    matrix(matrix&& m) : matrix(proxy(std::move(m))) {}
    matrix(const matrix& m) : matrix(proxy(m)) {}

    explicit matrix(syntax::parameter_declaration* pd) : super_t(pd) {}

    // The TArg argument stops this conflicting with the default constructor
    // The disable_if is necessary to stop conflicts with the copy constructor. Ensuring is_empty is false is a good enough requirement as sltl doesn't allow 1x1 matrices
    template<typename TArg, typename ...A, detail::disable_if<detail::is_empty<A...>> = detail::default_tag>
    explicit matrix(TArg&& t, A&&... a) : matrix(proxy(syntax::expression::make<syntax::constructor_call>(language::type_helper<matrix>(), unpack<elements>(std::forward<TArg>(t), std::forward<A>(a)...)))) {}

    proxy operator=(proxy&& p)
    {
      return super_t::template make_proxy<syntax::operator_binary>(language::id_assignment, variable::make_reference(), p.move());
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
    template<int NArgs, typename ...A>
    syntax::expression_list unpack(typename scalar<T>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<NArgs - 1>(std::forward<A>(a)...));
    }

    template<int NArgs, typename ...A>
    syntax::expression_list unpack(typename vector<T, 2>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<NArgs - 2>(std::forward<A>(a)...));
    }

    template<int NArgs, typename ...A>
    syntax::expression_list unpack(typename vector<T, 3>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<NArgs - 3>(std::forward<A>(a)...));
    }

    template<int NArgs, typename ...A>
    syntax::expression_list unpack(typename vector<T, 4>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<NArgs - 4>(std::forward<A>(a)...));
    }

    template<int NArgs>
    syntax::expression_list unpack()
    {
      static_assert(!(NArgs > 0), "sltl::matrix: too few components provided as arguments");
      static_assert(!(NArgs < 0), "sltl::matrix: too many components provided as arguments");

      return syntax::expression_list();
    }

    static const size_t elements = M * N;
  };
}
