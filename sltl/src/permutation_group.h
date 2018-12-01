#pragma once

#include "permutation.h"


namespace sltl
{
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D, size_t = D>
  class permutation_group;

  // Partial specialization for zero dimensions
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D>
  class permutation_group<P, S, T, D, 0>
  {
  protected:
    typedef P<T, D> permutable_t;
  };

  // Partial specialization for 1D permutations
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D>
  class permutation_group<P, S, T, D, 1> : public permutation_group<P, S, T, D, 0>
  {
  protected:
    using typename permutation_group<P, S, T, D, 0>::permutable_t;

  public:
    permutation_group(permutable_t& p) : permutation_group<P, S, T, D, 0>(), x(p) {}

    union
    {
      permutation<P, S, T, D, 0>          x;
      permutation<P, S, T, D, 0, 0>       xx;
      permutation<P, S, T, D, 0, 0, 0>    xxx;
      permutation<P, S, T, D, 0, 0, 0, 0> xxxx;
    };
  };

  // Partial specialization for 2D permutations
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D>
  class permutation_group<P, S, T, D, 2> : public permutation_group<P, S, T, D, 1>
  {
  protected:
    using typename permutation_group<P, S, T, D, 1>::permutable_t;

  public:
    permutation_group(permutable_t& p) : permutation_group<P, S, T, D, 1>(p), y(p) {}

    union
    {
      permutation<P, S, T, D, 1>          y;
      permutation<P, S, T, D, 0, 1>       xy;
      permutation<P, S, T, D, 1, 0>       yx;
      permutation<P, S, T, D, 1, 1>       yy;
      permutation<P, S, T, D, 0, 0, 1>    xxy;
      permutation<P, S, T, D, 0, 1, 0>    xyx;
      permutation<P, S, T, D, 0, 1, 1>    xyy;
      permutation<P, S, T, D, 1, 0, 0>    yxx;
      permutation<P, S, T, D, 1, 0, 1>    yxy;
      permutation<P, S, T, D, 1, 1, 0>    yyx;
      permutation<P, S, T, D, 1, 1, 1>    yyy;
      permutation<P, S, T, D, 0, 0, 0, 1> xxxy;
      permutation<P, S, T, D, 0, 0, 1, 0> xxyx;
      permutation<P, S, T, D, 0, 0, 1, 1> xxyy;
      permutation<P, S, T, D, 0, 1, 0, 0> xyxx;
      permutation<P, S, T, D, 0, 1, 0, 1> xyxy;
      permutation<P, S, T, D, 0, 1, 1, 0> xyyx;
      permutation<P, S, T, D, 0, 1, 1, 1> xyyy;
      permutation<P, S, T, D, 1, 0, 0, 0> yxxx;
      permutation<P, S, T, D, 1, 0, 0, 1> yxxy;
      permutation<P, S, T, D, 1, 0, 1, 0> yxyx;
      permutation<P, S, T, D, 1, 0, 1, 1> yxyy;
      permutation<P, S, T, D, 1, 1, 0, 0> yyxx;
      permutation<P, S, T, D, 1, 1, 0, 1> yyxy;
      permutation<P, S, T, D, 1, 1, 1, 0> yyyx;
      permutation<P, S, T, D, 1, 1, 1, 1> yyyy;
    };
  };

  // Partial specialization for 3D permutations
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D>
  class permutation_group<P, S, T, D, 3> : public permutation_group<P, S, T, D, 2>
  {
  protected:
    using typename permutation_group<P, S, T, D, 2>::permutable_t;

  public:
    permutation_group(permutable_t& p) : permutation_group<P, S, T, D, 2>(p), z(p) {}

    union
    {
      permutation<P, S, T, D, 2>          z;
      permutation<P, S, T, D, 0, 2>       xz;
      permutation<P, S, T, D, 1, 2>       yz;
      permutation<P, S, T, D, 2, 0>       zx;
      permutation<P, S, T, D, 2, 1>       zy;
      permutation<P, S, T, D, 2, 2>       zz;
      permutation<P, S, T, D, 0, 0, 2>    xxz;
      permutation<P, S, T, D, 0, 1, 2>    xyz;
      permutation<P, S, T, D, 0, 2, 0>    xzx;
      permutation<P, S, T, D, 0, 2, 1>    xzy;
      permutation<P, S, T, D, 0, 2, 2>    xzz;
      permutation<P, S, T, D, 1, 0, 2>    yxz;
      permutation<P, S, T, D, 1, 1, 2>    yyz;
      permutation<P, S, T, D, 1, 2, 0>    yzx;
      permutation<P, S, T, D, 1, 2, 1>    yzy;
      permutation<P, S, T, D, 1, 2, 2>    yzz;
      permutation<P, S, T, D, 2, 0, 0>    zxx;
      permutation<P, S, T, D, 2, 0, 1>    zxy;
      permutation<P, S, T, D, 2, 0, 2>    zxz;
      permutation<P, S, T, D, 2, 1, 0>    zyx;
      permutation<P, S, T, D, 2, 1, 1>    zyy;
      permutation<P, S, T, D, 2, 1, 2>    zyz;
      permutation<P, S, T, D, 2, 2, 0>    zzx;
      permutation<P, S, T, D, 2, 2, 1>    zzy;
      permutation<P, S, T, D, 2, 2, 2>    zzz;
      permutation<P, S, T, D, 0, 0, 0, 2> xxxz;
      permutation<P, S, T, D, 0, 0, 1, 2> xxyz;
      permutation<P, S, T, D, 0, 0, 2, 0> xxzx;
      permutation<P, S, T, D, 0, 0, 2, 1> xxzy;
      permutation<P, S, T, D, 0, 0, 2, 2> xxzz;
      permutation<P, S, T, D, 0, 1, 0, 2> xyxz;
      permutation<P, S, T, D, 0, 1, 1, 2> xyyz;
      permutation<P, S, T, D, 0, 1, 2, 0> xyzx;
      permutation<P, S, T, D, 0, 1, 2, 1> xyzy;
      permutation<P, S, T, D, 0, 1, 2, 2> xyzz;
      permutation<P, S, T, D, 0, 2, 0, 0> xzxx;
      permutation<P, S, T, D, 0, 2, 0, 1> xzxy;
      permutation<P, S, T, D, 0, 2, 0, 2> xzxz;
      permutation<P, S, T, D, 0, 2, 1, 0> xzyx;
      permutation<P, S, T, D, 0, 2, 1, 1> xzyy;
      permutation<P, S, T, D, 0, 2, 1, 2> xzyz;
      permutation<P, S, T, D, 0, 2, 2, 0> xzzx;
      permutation<P, S, T, D, 0, 2, 2, 1> xzzy;
      permutation<P, S, T, D, 0, 2, 2, 2> xzzz;
      permutation<P, S, T, D, 1, 0, 0, 2> yxxz;
      permutation<P, S, T, D, 1, 0, 1, 2> yxyz;
      permutation<P, S, T, D, 1, 0, 2, 0> yxzx;
      permutation<P, S, T, D, 1, 0, 2, 1> yxzy;
      permutation<P, S, T, D, 1, 0, 2, 2> yxzz;
      permutation<P, S, T, D, 1, 1, 0, 2> yyxz;
      permutation<P, S, T, D, 1, 1, 1, 2> yyyz;
      permutation<P, S, T, D, 1, 1, 2, 0> yyzx;
      permutation<P, S, T, D, 1, 1, 2, 1> yyzy;
      permutation<P, S, T, D, 1, 1, 2, 2> yyzz;
      permutation<P, S, T, D, 1, 2, 0, 0> yzxx;
      permutation<P, S, T, D, 1, 2, 0, 1> yzxy;
      permutation<P, S, T, D, 1, 2, 0, 2> yzxz;
      permutation<P, S, T, D, 1, 2, 1, 0> yzyx;
      permutation<P, S, T, D, 1, 2, 1, 1> yzyy;
      permutation<P, S, T, D, 1, 2, 1, 2> yzyz;
      permutation<P, S, T, D, 1, 2, 2, 0> yzzx;
      permutation<P, S, T, D, 1, 2, 2, 1> yzzy;
      permutation<P, S, T, D, 1, 2, 2, 2> yzzz;
      permutation<P, S, T, D, 2, 0, 0, 0> zxxx;
      permutation<P, S, T, D, 2, 0, 0, 1> zxxy;
      permutation<P, S, T, D, 2, 0, 0, 2> zxxz;
      permutation<P, S, T, D, 2, 0, 1, 0> zxyx;
      permutation<P, S, T, D, 2, 0, 1, 1> zxyy;
      permutation<P, S, T, D, 2, 0, 1, 2> zxyz;
      permutation<P, S, T, D, 2, 0, 2, 0> zxzx;
      permutation<P, S, T, D, 2, 0, 2, 1> zxzy;
      permutation<P, S, T, D, 2, 0, 2, 2> zxzz;
      permutation<P, S, T, D, 2, 1, 0, 0> zyxx;
      permutation<P, S, T, D, 2, 1, 0, 1> zyxy;
      permutation<P, S, T, D, 2, 1, 0, 2> zyxz;
      permutation<P, S, T, D, 2, 1, 1, 0> zyyx;
      permutation<P, S, T, D, 2, 1, 1, 1> zyyy;
      permutation<P, S, T, D, 2, 1, 1, 2> zyyz;
      permutation<P, S, T, D, 2, 1, 2, 0> zyzx;
      permutation<P, S, T, D, 2, 1, 2, 1> zyzy;
      permutation<P, S, T, D, 2, 1, 2, 2> zyzz;
      permutation<P, S, T, D, 2, 2, 0, 0> zzxx;
      permutation<P, S, T, D, 2, 2, 0, 1> zzxy;
      permutation<P, S, T, D, 2, 2, 0, 2> zzxz;
      permutation<P, S, T, D, 2, 2, 1, 0> zzyx;
      permutation<P, S, T, D, 2, 2, 1, 1> zzyy;
      permutation<P, S, T, D, 2, 2, 1, 2> zzyz;
      permutation<P, S, T, D, 2, 2, 2, 0> zzzx;
      permutation<P, S, T, D, 2, 2, 2, 1> zzzy;
      permutation<P, S, T, D, 2, 2, 2, 2> zzzz;
    };
  };

  // Partial specialization for 4D permutations
  template<template<typename, size_t> class P, template<typename> class S, typename T, size_t D>
  class permutation_group<P, S, T, D, 4> : public permutation_group<P, S, T, D, 3>
  {
  protected:
    using typename permutation_group<P, S, T, D, 3>::permutable_t;

  public:
    permutation_group(permutable_t& p) : permutation_group<P, S, T, D, 3>(p), w(p) {}

    union
    {
      permutation<P, S, T, D, 3>          w;
      permutation<P, S, T, D, 0, 3>       xw;
      permutation<P, S, T, D, 1, 3>       yw;
      permutation<P, S, T, D, 2, 3>       zw;
      permutation<P, S, T, D, 3, 0>       wx;
      permutation<P, S, T, D, 3, 1>       wy;
      permutation<P, S, T, D, 3, 2>       wz;
      permutation<P, S, T, D, 3, 3>       ww;
      permutation<P, S, T, D, 0, 0, 3>    xxw;
      permutation<P, S, T, D, 0, 1, 3>    xyw;
      permutation<P, S, T, D, 0, 2, 3>    xzw;
      permutation<P, S, T, D, 0, 3, 0>    xwx;
      permutation<P, S, T, D, 0, 3, 1>    xwy;
      permutation<P, S, T, D, 0, 3, 2>    xwz;
      permutation<P, S, T, D, 0, 3, 3>    xww;
      permutation<P, S, T, D, 1, 0, 3>    yxw;
      permutation<P, S, T, D, 1, 1, 3>    yyw;
      permutation<P, S, T, D, 1, 2, 3>    yzw;
      permutation<P, S, T, D, 1, 3, 0>    ywx;
      permutation<P, S, T, D, 1, 3, 1>    ywy;
      permutation<P, S, T, D, 1, 3, 2>    ywz;
      permutation<P, S, T, D, 1, 3, 3>    yww;
      permutation<P, S, T, D, 2, 0, 3>    zxw;
      permutation<P, S, T, D, 2, 1, 3>    zyw;
      permutation<P, S, T, D, 2, 2, 3>    zzw;
      permutation<P, S, T, D, 2, 3, 0>    zwx;
      permutation<P, S, T, D, 2, 3, 1>    zwy;
      permutation<P, S, T, D, 2, 3, 2>    zwz;
      permutation<P, S, T, D, 2, 3, 3>    zww;
      permutation<P, S, T, D, 3, 0, 0>    wxx;
      permutation<P, S, T, D, 3, 0, 1>    wxy;
      permutation<P, S, T, D, 3, 0, 2>    wxz;
      permutation<P, S, T, D, 3, 0, 3>    wxw;
      permutation<P, S, T, D, 3, 1, 0>    wyx;
      permutation<P, S, T, D, 3, 1, 1>    wyy;
      permutation<P, S, T, D, 3, 1, 2>    wyz;
      permutation<P, S, T, D, 3, 1, 3>    wyw;
      permutation<P, S, T, D, 3, 2, 0>    wzx;
      permutation<P, S, T, D, 3, 2, 1>    wzy;
      permutation<P, S, T, D, 3, 2, 2>    wzz;
      permutation<P, S, T, D, 3, 2, 3>    wzw;
      permutation<P, S, T, D, 3, 3, 0>    wwx;
      permutation<P, S, T, D, 3, 3, 1>    wwy;
      permutation<P, S, T, D, 3, 3, 2>    wwz;
      permutation<P, S, T, D, 3, 3, 3>    www;
      permutation<P, S, T, D, 0, 0, 0, 3> xxxw;
      permutation<P, S, T, D, 0, 0, 1, 3> xxyw;
      permutation<P, S, T, D, 0, 0, 2, 3> xxzw;
      permutation<P, S, T, D, 0, 0, 3, 0> xxwx;
      permutation<P, S, T, D, 0, 0, 3, 1> xxwy;
      permutation<P, S, T, D, 0, 0, 3, 2> xxwz;
      permutation<P, S, T, D, 0, 0, 3, 3> xxww;
      permutation<P, S, T, D, 0, 1, 0, 3> xyxw;
      permutation<P, S, T, D, 0, 1, 1, 3> xyyw;
      permutation<P, S, T, D, 0, 1, 2, 3> xyzw;
      permutation<P, S, T, D, 0, 1, 3, 0> xywx;
      permutation<P, S, T, D, 0, 1, 3, 1> xywy;
      permutation<P, S, T, D, 0, 1, 3, 2> xywz;
      permutation<P, S, T, D, 0, 1, 3, 3> xyww;
      permutation<P, S, T, D, 0, 2, 0, 3> xzxw;
      permutation<P, S, T, D, 0, 2, 1, 3> xzyw;
      permutation<P, S, T, D, 0, 2, 2, 3> xzzw;
      permutation<P, S, T, D, 0, 2, 3, 0> xzwx;
      permutation<P, S, T, D, 0, 2, 3, 1> xzwy;
      permutation<P, S, T, D, 0, 2, 3, 2> xzwz;
      permutation<P, S, T, D, 0, 2, 3, 3> xzww;
      permutation<P, S, T, D, 0, 3, 0, 0> xwxx;
      permutation<P, S, T, D, 0, 3, 0, 1> xwxy;
      permutation<P, S, T, D, 0, 3, 0, 2> xwxz;
      permutation<P, S, T, D, 0, 3, 0, 3> xwxw;
      permutation<P, S, T, D, 0, 3, 1, 0> xwyx;
      permutation<P, S, T, D, 0, 3, 1, 1> xwyy;
      permutation<P, S, T, D, 0, 3, 1, 2> xwyz;
      permutation<P, S, T, D, 0, 3, 1, 3> xwyw;
      permutation<P, S, T, D, 0, 3, 2, 0> xwzx;
      permutation<P, S, T, D, 0, 3, 2, 1> xwzy;
      permutation<P, S, T, D, 0, 3, 2, 2> xwzz;
      permutation<P, S, T, D, 0, 3, 2, 3> xwzw;
      permutation<P, S, T, D, 0, 3, 3, 0> xwwx;
      permutation<P, S, T, D, 0, 3, 3, 1> xwwy;
      permutation<P, S, T, D, 0, 3, 3, 2> xwwz;
      permutation<P, S, T, D, 0, 3, 3, 3> xwww;
      permutation<P, S, T, D, 1, 0, 0, 3> yxxw;
      permutation<P, S, T, D, 1, 0, 1, 3> yxyw;
      permutation<P, S, T, D, 1, 0, 2, 3> yxzw;
      permutation<P, S, T, D, 1, 0, 3, 0> yxwx;
      permutation<P, S, T, D, 1, 0, 3, 1> yxwy;
      permutation<P, S, T, D, 1, 0, 3, 2> yxwz;
      permutation<P, S, T, D, 1, 0, 3, 3> yxww;
      permutation<P, S, T, D, 1, 1, 0, 3> yyxw;
      permutation<P, S, T, D, 1, 1, 1, 3> yyyw;
      permutation<P, S, T, D, 1, 1, 2, 3> yyzw;
      permutation<P, S, T, D, 1, 1, 3, 0> yywx;
      permutation<P, S, T, D, 1, 1, 3, 1> yywy;
      permutation<P, S, T, D, 1, 1, 3, 2> yywz;
      permutation<P, S, T, D, 1, 1, 3, 3> yyww;
      permutation<P, S, T, D, 1, 2, 0, 3> yzxw;
      permutation<P, S, T, D, 1, 2, 1, 3> yzyw;
      permutation<P, S, T, D, 1, 2, 2, 3> yzzw;
      permutation<P, S, T, D, 1, 2, 3, 0> yzwx;
      permutation<P, S, T, D, 1, 2, 3, 1> yzwy;
      permutation<P, S, T, D, 1, 2, 3, 2> yzwz;
      permutation<P, S, T, D, 1, 2, 3, 3> yzww;
      permutation<P, S, T, D, 1, 3, 0, 0> ywxx;
      permutation<P, S, T, D, 1, 3, 0, 1> ywxy;
      permutation<P, S, T, D, 1, 3, 0, 2> ywxz;
      permutation<P, S, T, D, 1, 3, 0, 3> ywxw;
      permutation<P, S, T, D, 1, 3, 1, 0> ywyx;
      permutation<P, S, T, D, 1, 3, 1, 1> ywyy;
      permutation<P, S, T, D, 1, 3, 1, 2> ywyz;
      permutation<P, S, T, D, 1, 3, 1, 3> ywyw;
      permutation<P, S, T, D, 1, 3, 2, 0> ywzx;
      permutation<P, S, T, D, 1, 3, 2, 1> ywzy;
      permutation<P, S, T, D, 1, 3, 2, 2> ywzz;
      permutation<P, S, T, D, 1, 3, 2, 3> ywzw;
      permutation<P, S, T, D, 1, 3, 3, 0> ywwx;
      permutation<P, S, T, D, 1, 3, 3, 1> ywwy;
      permutation<P, S, T, D, 1, 3, 3, 2> ywwz;
      permutation<P, S, T, D, 1, 3, 3, 3> ywww;
      permutation<P, S, T, D, 2, 0, 0, 3> zxxw;
      permutation<P, S, T, D, 2, 0, 1, 3> zxyw;
      permutation<P, S, T, D, 2, 0, 2, 3> zxzw;
      permutation<P, S, T, D, 2, 0, 3, 0> zxwx;
      permutation<P, S, T, D, 2, 0, 3, 1> zxwy;
      permutation<P, S, T, D, 2, 0, 3, 2> zxwz;
      permutation<P, S, T, D, 2, 0, 3, 3> zxww;
      permutation<P, S, T, D, 2, 1, 0, 3> zyxw;
      permutation<P, S, T, D, 2, 1, 1, 3> zyyw;
      permutation<P, S, T, D, 2, 1, 2, 3> zyzw;
      permutation<P, S, T, D, 2, 1, 3, 0> zywx;
      permutation<P, S, T, D, 2, 1, 3, 1> zywy;
      permutation<P, S, T, D, 2, 1, 3, 2> zywz;
      permutation<P, S, T, D, 2, 1, 3, 3> zyww;
      permutation<P, S, T, D, 2, 2, 0, 3> zzxw;
      permutation<P, S, T, D, 2, 2, 1, 3> zzyw;
      permutation<P, S, T, D, 2, 2, 2, 3> zzzw;
      permutation<P, S, T, D, 2, 2, 3, 0> zzwx;
      permutation<P, S, T, D, 2, 2, 3, 1> zzwy;
      permutation<P, S, T, D, 2, 2, 3, 2> zzwz;
      permutation<P, S, T, D, 2, 2, 3, 3> zzww;
      permutation<P, S, T, D, 2, 3, 0, 0> zwxx;
      permutation<P, S, T, D, 2, 3, 0, 1> zwxy;
      permutation<P, S, T, D, 2, 3, 0, 2> zwxz;
      permutation<P, S, T, D, 2, 3, 0, 3> zwxw;
      permutation<P, S, T, D, 2, 3, 1, 0> zwyx;
      permutation<P, S, T, D, 2, 3, 1, 1> zwyy;
      permutation<P, S, T, D, 2, 3, 1, 2> zwyz;
      permutation<P, S, T, D, 2, 3, 1, 3> zwyw;
      permutation<P, S, T, D, 2, 3, 2, 0> zwzx;
      permutation<P, S, T, D, 2, 3, 2, 1> zwzy;
      permutation<P, S, T, D, 2, 3, 2, 2> zwzz;
      permutation<P, S, T, D, 2, 3, 2, 3> zwzw;
      permutation<P, S, T, D, 2, 3, 3, 0> zwwx;
      permutation<P, S, T, D, 2, 3, 3, 1> zwwy;
      permutation<P, S, T, D, 2, 3, 3, 2> zwwz;
      permutation<P, S, T, D, 2, 3, 3, 3> zwww;
      permutation<P, S, T, D, 3, 0, 0, 0> wxxx;
      permutation<P, S, T, D, 3, 0, 0, 1> wxxy;
      permutation<P, S, T, D, 3, 0, 0, 2> wxxz;
      permutation<P, S, T, D, 3, 0, 0, 3> wxxw;
      permutation<P, S, T, D, 3, 0, 1, 0> wxyx;
      permutation<P, S, T, D, 3, 0, 1, 1> wxyy;
      permutation<P, S, T, D, 3, 0, 1, 2> wxyz;
      permutation<P, S, T, D, 3, 0, 1, 3> wxyw;
      permutation<P, S, T, D, 3, 0, 2, 0> wxzx;
      permutation<P, S, T, D, 3, 0, 2, 1> wxzy;
      permutation<P, S, T, D, 3, 0, 2, 2> wxzz;
      permutation<P, S, T, D, 3, 0, 2, 3> wxzw;
      permutation<P, S, T, D, 3, 0, 3, 0> wxwx;
      permutation<P, S, T, D, 3, 0, 3, 1> wxwy;
      permutation<P, S, T, D, 3, 0, 3, 2> wxwz;
      permutation<P, S, T, D, 3, 0, 3, 3> wxww;
      permutation<P, S, T, D, 3, 1, 0, 0> wyxx;
      permutation<P, S, T, D, 3, 1, 0, 1> wyxy;
      permutation<P, S, T, D, 3, 1, 0, 2> wyxz;
      permutation<P, S, T, D, 3, 1, 0, 3> wyxw;
      permutation<P, S, T, D, 3, 1, 1, 0> wyyx;
      permutation<P, S, T, D, 3, 1, 1, 1> wyyy;
      permutation<P, S, T, D, 3, 1, 1, 2> wyyz;
      permutation<P, S, T, D, 3, 1, 1, 3> wyyw;
      permutation<P, S, T, D, 3, 1, 2, 0> wyzx;
      permutation<P, S, T, D, 3, 1, 2, 1> wyzy;
      permutation<P, S, T, D, 3, 1, 2, 2> wyzz;
      permutation<P, S, T, D, 3, 1, 2, 3> wyzw;
      permutation<P, S, T, D, 3, 1, 3, 0> wywx;
      permutation<P, S, T, D, 3, 1, 3, 1> wywy;
      permutation<P, S, T, D, 3, 1, 3, 2> wywz;
      permutation<P, S, T, D, 3, 1, 3, 3> wyww;
      permutation<P, S, T, D, 3, 2, 0, 0> wzxx;
      permutation<P, S, T, D, 3, 2, 0, 1> wzxy;
      permutation<P, S, T, D, 3, 2, 0, 2> wzxz;
      permutation<P, S, T, D, 3, 2, 0, 3> wzxw;
      permutation<P, S, T, D, 3, 2, 1, 0> wzyx;
      permutation<P, S, T, D, 3, 2, 1, 1> wzyy;
      permutation<P, S, T, D, 3, 2, 1, 2> wzyz;
      permutation<P, S, T, D, 3, 2, 1, 3> wzyw;
      permutation<P, S, T, D, 3, 2, 2, 0> wzzx;
      permutation<P, S, T, D, 3, 2, 2, 1> wzzy;
      permutation<P, S, T, D, 3, 2, 2, 2> wzzz;
      permutation<P, S, T, D, 3, 2, 2, 3> wzzw;
      permutation<P, S, T, D, 3, 2, 3, 0> wzwx;
      permutation<P, S, T, D, 3, 2, 3, 1> wzwy;
      permutation<P, S, T, D, 3, 2, 3, 2> wzwz;
      permutation<P, S, T, D, 3, 2, 3, 3> wzww;
      permutation<P, S, T, D, 3, 3, 0, 0> wwxx;
      permutation<P, S, T, D, 3, 3, 0, 1> wwxy;
      permutation<P, S, T, D, 3, 3, 0, 2> wwxz;
      permutation<P, S, T, D, 3, 3, 0, 3> wwxw;
      permutation<P, S, T, D, 3, 3, 1, 0> wwyx;
      permutation<P, S, T, D, 3, 3, 1, 1> wwyy;
      permutation<P, S, T, D, 3, 3, 1, 2> wwyz;
      permutation<P, S, T, D, 3, 3, 1, 3> wwyw;
      permutation<P, S, T, D, 3, 3, 2, 0> wwzx;
      permutation<P, S, T, D, 3, 3, 2, 1> wwzy;
      permutation<P, S, T, D, 3, 3, 2, 2> wwzz;
      permutation<P, S, T, D, 3, 3, 2, 3> wwzw;
      permutation<P, S, T, D, 3, 3, 3, 0> wwwx;
      permutation<P, S, T, D, 3, 3, 3, 1> wwwy;
      permutation<P, S, T, D, 3, 3, 3, 2> wwwz;
      permutation<P, S, T, D, 3, 3, 3, 3> wwww;
    };
  };
}
