#pragma once

#include "scalar.h"

#include "syntax/expression_list.h"


namespace sltl
{
  template<typename T, size_t D>
  struct vector_id
  {
    static const language::type_id value = language::id_unknown;
  };

  template<>
  struct vector_id<float, 2>
  {
    static const language::type_id value = language::id_float2;
  };

  template<>
  struct vector_id<float, 3>
  {
    static const language::type_id value = language::id_float3;
  };

  template<>
  struct vector_id<float, 4>
  {
    static const language::type_id value = language::id_float4;
  };

  template<>
  struct vector_id<double, 2>
  {
    static const language::type_id value = language::id_double2;
  };

  template<>
  struct vector_id<double, 3>
  {
    static const language::type_id value = language::id_double3;
  };

  template<>
  struct vector_id<double, 4>
  {
    static const language::type_id value = language::id_double4;
  };

  template<>
  struct vector_id<int, 2>
  {
    static const language::type_id value = language::id_int2;
  };

  template<>
  struct vector_id<int, 3>
  {
    static const language::type_id value = language::id_int3;
  };

  template<>
  struct vector_id<int, 4>
  {
    static const language::type_id value = language::id_int4;
  };

  template<>
  struct vector_id<unsigned int, 2>
  {
    static const language::type_id value = language::id_uint2;
  };

  template<>
  struct vector_id<unsigned int, 3>
  {
    static const language::type_id value = language::id_uint3;
  };

  template<>
  struct vector_id<unsigned int, 4>
  {
    static const language::type_id value = language::id_uint4;
  };

  template<>
  struct vector_id<bool, 2>
  {
    static const language::type_id value = language::id_bool2;
  };

  template<>
  struct vector_id<bool, 3>
  {
    static const language::type_id value = language::id_bool3;
  };

  template<>
  struct vector_id<bool, 4>
  {
    static const language::type_id value = language::id_bool4;
  };

  template<typename T, size_t D>
  class vector : public basic<sltl::vector, T, D>
  {
    static_assert((D >=2) && (D <= 4), "sltl::vector: template parameter D is only valid for values of 2, 3 and 4");

  public:
    //TODO: sort out the double default constructor warning...
    vector() : basic(vector_id<T, D>::value) {}
    vector(proxy&& p) : basic(vector_id<T, D>::value, p.move()) {}

    vector(vector&& v) : vector(proxy(std::move(v))) {}
    vector(const vector& v) : vector(proxy(v)) {}

    template<typename ...A>
    vector(A&&... a) : vector(proxy(syntax::expression::make<syntax::expression_list>(unpack<D>(std::forward<A>(a)...)))) {}

  private:
    template<int N, typename ...A>
    syntax::expression_list unpack(typename scalar<T>::proxy&& p, A&&... a)
    {
      return syntax::expression_list(p.move(), unpack<N - 1>(std::forward<A>(a)...));
    }

    template<int N, size_t D2, typename ...A>
    syntax::expression_list unpack(vector<T, D2>&& v, A&&... a)
    {
      return syntax::expression_list(vector<T, D2>::proxy(std::move(v)).move(), unpack<N - D2>(std::forward<A>(a)...));
    }

    template<int N, size_t D2, typename ...A>
    syntax::expression_list unpack(const vector<T, D2>& v, A&&... a)
    {
      return syntax::expression_list(v.make_reference(), unpack<N - D2>(std::forward<A>(a)...));
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
