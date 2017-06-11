#pragma once

#include "permutation.h"


namespace sltl
{
  template<template<typename, size_t> class P, typename T, size_t D, size_t = D>
  class permutation_group;

  // Partial specialization for zero dimensions
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 0>
  {
  protected:
    typedef typename P<T, D> permutable_t;
  };

  // Partial specialization for 1D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 1> : public permutation_group<P, T, D, 0>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 0>(), x(p) {}

    union
    {
      permutation<P, T, D, 0>          x;
      permutation<P, T, D, 0, 0>       xx;
      permutation<P, T, D, 0, 0, 0>    xxx;
      permutation<P, T, D, 0, 0, 0, 0> xxxx;
    };
  };

  // Partial specialization for 2D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 2> : public permutation_group<P, T, D, 1>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 1>(p), y(p) {}

    union
    {
      permutation<P, T, D, 1>          y;
      permutation<P, T, D, 0, 1>       xy;
      permutation<P, T, D, 1, 0>       yx;
      permutation<P, T, D, 1, 1>       yy;
      permutation<P, T, D, 0, 0, 1>    xxy;
      permutation<P, T, D, 0, 1, 0>    xyx;
      permutation<P, T, D, 0, 1, 1>    xyy;
      permutation<P, T, D, 1, 0, 0>    yxx;
      permutation<P, T, D, 1, 0, 1>    yxy;
      permutation<P, T, D, 1, 1, 0>    yyx;
      permutation<P, T, D, 1, 1, 1>    yyy;
      permutation<P, T, D, 0, 0, 0, 1> xxxy;
      permutation<P, T, D, 0, 0, 1, 0> xxyx;
      permutation<P, T, D, 0, 0, 1, 1> xxyy;
      permutation<P, T, D, 0, 1, 0, 0> xyxx;
      permutation<P, T, D, 0, 1, 0, 1> xyxy;
      permutation<P, T, D, 0, 1, 1, 0> xyyx;
      permutation<P, T, D, 0, 1, 1, 1> xyyy;
      permutation<P, T, D, 1, 0, 0, 0> yxxx;
      permutation<P, T, D, 1, 0, 0, 1> yxxy;
      permutation<P, T, D, 1, 0, 1, 0> yxyx;
      permutation<P, T, D, 1, 0, 1, 1> yxyy;
      permutation<P, T, D, 1, 1, 0, 0> yyxx;
      permutation<P, T, D, 1, 1, 0, 1> yyxy;
      permutation<P, T, D, 1, 1, 1, 0> yyyx;
      permutation<P, T, D, 1, 1, 1, 1> yyyy;
    };
  };

  // Partial specialization for 3D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 3> : public permutation_group<P, T, D, 2>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 2>(p), z(p) {}

    union
    {
      permutation<P, T, D, 2>          z;
      permutation<P, T, D, 0, 2>       xz;
      permutation<P, T, D, 1, 2>       yz;
      permutation<P, T, D, 2, 0>       zx;
      permutation<P, T, D, 2, 1>       zy;
      permutation<P, T, D, 2, 2>       zz;
      permutation<P, T, D, 0, 0, 2>    xxz;
      permutation<P, T, D, 0, 1, 2>    xyz;
      permutation<P, T, D, 0, 2, 0>    xzx;
      permutation<P, T, D, 0, 2, 1>    xzy;
      permutation<P, T, D, 0, 2, 2>    xzz;
      permutation<P, T, D, 1, 0, 2>    yxz;
      permutation<P, T, D, 1, 1, 2>    yyz;
      permutation<P, T, D, 1, 2, 0>    yzx;
      permutation<P, T, D, 1, 2, 1>    yzy;
      permutation<P, T, D, 1, 2, 2>    yzz;
      permutation<P, T, D, 2, 0, 0>    zxx;
      permutation<P, T, D, 2, 0, 1>    zxy;
      permutation<P, T, D, 2, 0, 2>    zxz;
      permutation<P, T, D, 2, 1, 0>    zyx;
      permutation<P, T, D, 2, 1, 1>    zyy;
      permutation<P, T, D, 2, 1, 2>    zyz;
      permutation<P, T, D, 2, 2, 0>    zzx;
      permutation<P, T, D, 2, 2, 1>    zzy;
      permutation<P, T, D, 2, 2, 2>    zzz;
      permutation<P, T, D, 0, 0, 0, 2> xxxz;
      permutation<P, T, D, 0, 0, 1, 2> xxyz;
      permutation<P, T, D, 0, 0, 2, 0> xxzx;
      permutation<P, T, D, 0, 0, 2, 1> xxzy;
      permutation<P, T, D, 0, 0, 2, 2> xxzz;
      permutation<P, T, D, 0, 1, 0, 2> xyxz;
      permutation<P, T, D, 0, 1, 1, 2> xyyz;
      permutation<P, T, D, 0, 1, 2, 0> xyzx;
      permutation<P, T, D, 0, 1, 2, 1> xyzy;
      permutation<P, T, D, 0, 1, 2, 2> xyzz;
      permutation<P, T, D, 0, 2, 0, 0> xzxx;
      permutation<P, T, D, 0, 2, 0, 1> xzxy;
      permutation<P, T, D, 0, 2, 0, 2> xzxz;
      permutation<P, T, D, 0, 2, 1, 0> xzyx;
      permutation<P, T, D, 0, 2, 1, 1> xzyy;
      permutation<P, T, D, 0, 2, 1, 2> xzyz;
      permutation<P, T, D, 0, 2, 2, 0> xzzx;
      permutation<P, T, D, 0, 2, 2, 1> xzzy;
      permutation<P, T, D, 0, 2, 2, 2> xzzz;
      permutation<P, T, D, 1, 0, 0, 2> yxxz;
      permutation<P, T, D, 1, 0, 1, 2> yxyz;
      permutation<P, T, D, 1, 0, 2, 0> yxzx;
      permutation<P, T, D, 1, 0, 2, 1> yxzy;
      permutation<P, T, D, 1, 0, 2, 2> yxzz;
      permutation<P, T, D, 1, 1, 0, 2> yyxz;
      permutation<P, T, D, 1, 1, 1, 2> yyyz;
      permutation<P, T, D, 1, 1, 2, 0> yyzx;
      permutation<P, T, D, 1, 1, 2, 1> yyzy;
      permutation<P, T, D, 1, 1, 2, 2> yyzz;
      permutation<P, T, D, 1, 2, 0, 0> yzxx;
      permutation<P, T, D, 1, 2, 0, 1> yzxy;
      permutation<P, T, D, 1, 2, 0, 2> yzxz;
      permutation<P, T, D, 1, 2, 1, 0> yzyx;
      permutation<P, T, D, 1, 2, 1, 1> yzyy;
      permutation<P, T, D, 1, 2, 1, 2> yzyz;
      permutation<P, T, D, 1, 2, 2, 0> yzzx;
      permutation<P, T, D, 1, 2, 2, 1> yzzy;
      permutation<P, T, D, 1, 2, 2, 2> yzzz;
      permutation<P, T, D, 2, 0, 0, 0> zxxx;
      permutation<P, T, D, 2, 0, 0, 1> zxxy;
      permutation<P, T, D, 2, 0, 0, 2> zxxz;
      permutation<P, T, D, 2, 0, 1, 0> zxyx;
      permutation<P, T, D, 2, 0, 1, 1> zxyy;
      permutation<P, T, D, 2, 0, 1, 2> zxyz;
      permutation<P, T, D, 2, 0, 2, 0> zxzx;
      permutation<P, T, D, 2, 0, 2, 1> zxzy;
      permutation<P, T, D, 2, 0, 2, 2> zxzz;
      permutation<P, T, D, 2, 1, 0, 0> zyxx;
      permutation<P, T, D, 2, 1, 0, 1> zyxy;
      permutation<P, T, D, 2, 1, 0, 2> zyxz;
      permutation<P, T, D, 2, 1, 1, 0> zyyx;
      permutation<P, T, D, 2, 1, 1, 1> zyyy;
      permutation<P, T, D, 2, 1, 1, 2> zyyz;
      permutation<P, T, D, 2, 1, 2, 0> zyzx;
      permutation<P, T, D, 2, 1, 2, 1> zyzy;
      permutation<P, T, D, 2, 1, 2, 2> zyzz;
      permutation<P, T, D, 2, 2, 0, 0> zzxx;
      permutation<P, T, D, 2, 2, 0, 1> zzxy;
      permutation<P, T, D, 2, 2, 0, 2> zzxz;
      permutation<P, T, D, 2, 2, 1, 0> zzyx;
      permutation<P, T, D, 2, 2, 1, 1> zzyy;
      permutation<P, T, D, 2, 2, 1, 2> zzyz;
      permutation<P, T, D, 2, 2, 2, 0> zzzx;
      permutation<P, T, D, 2, 2, 2, 1> zzzy;
      permutation<P, T, D, 2, 2, 2, 2> zzzz;
    };
  };

  // Partial specialization for 4D permutations
  template<template<typename, size_t> class P, typename T, size_t D>
  class permutation_group<P, T, D, 4> : public permutation_group<P, T, D, 3>
  {
  public:
    permutation_group(permutable_t& p) : permutation_group<P, T, D, 3>(p), w(p) {}

    union
    {
      permutation<P, T, D, 3>          w;
      permutation<P, T, D, 0, 3>       xw;
      permutation<P, T, D, 1, 3>       yw;
      permutation<P, T, D, 2, 3>       zw;
      permutation<P, T, D, 3, 0>       wx;
      permutation<P, T, D, 3, 1>       wy;
      permutation<P, T, D, 3, 2>       wz;
      permutation<P, T, D, 3, 3>       ww;
      permutation<P, T, D, 0, 0, 3>    xxw;
      permutation<P, T, D, 0, 1, 3>    xyw;
      permutation<P, T, D, 0, 2, 3>    xzw;
      permutation<P, T, D, 0, 3, 0>    xwx;
      permutation<P, T, D, 0, 3, 1>    xwy;
      permutation<P, T, D, 0, 3, 2>    xwz;
      permutation<P, T, D, 0, 3, 3>    xww;
      permutation<P, T, D, 1, 0, 3>    yxw;
      permutation<P, T, D, 1, 1, 3>    yyw;
      permutation<P, T, D, 1, 2, 3>    yzw;
      permutation<P, T, D, 1, 3, 0>    ywx;
      permutation<P, T, D, 1, 3, 1>    ywy;
      permutation<P, T, D, 1, 3, 2>    ywz;
      permutation<P, T, D, 1, 3, 3>    yww;
      permutation<P, T, D, 2, 0, 3>    zxw;
      permutation<P, T, D, 2, 1, 3>    zyw;
      permutation<P, T, D, 2, 2, 3>    zzw;
      permutation<P, T, D, 2, 3, 0>    zwx;
      permutation<P, T, D, 2, 3, 1>    zwy;
      permutation<P, T, D, 2, 3, 2>    zwz;
      permutation<P, T, D, 2, 3, 3>    zww;
      permutation<P, T, D, 3, 0, 0>    wxx;
      permutation<P, T, D, 3, 0, 1>    wxy;
      permutation<P, T, D, 3, 0, 2>    wxz;
      permutation<P, T, D, 3, 0, 3>    wxw;
      permutation<P, T, D, 3, 1, 0>    wyx;
      permutation<P, T, D, 3, 1, 1>    wyy;
      permutation<P, T, D, 3, 1, 2>    wyz;
      permutation<P, T, D, 3, 1, 3>    wyw;
      permutation<P, T, D, 3, 2, 0>    wzx;
      permutation<P, T, D, 3, 2, 1>    wzy;
      permutation<P, T, D, 3, 2, 2>    wzz;
      permutation<P, T, D, 3, 2, 3>    wzw;
      permutation<P, T, D, 3, 3, 0>    wwx;
      permutation<P, T, D, 3, 3, 1>    wwy;
      permutation<P, T, D, 3, 3, 2>    wwz;
      permutation<P, T, D, 3, 3, 3>    www;
      permutation<P, T, D, 0, 0, 0, 3> xxxw;
      permutation<P, T, D, 0, 0, 1, 3> xxyw;
      permutation<P, T, D, 0, 0, 2, 3> xxzw;
      permutation<P, T, D, 0, 0, 3, 0> xxwx;
      permutation<P, T, D, 0, 0, 3, 1> xxwy;
      permutation<P, T, D, 0, 0, 3, 2> xxwz;
      permutation<P, T, D, 0, 0, 3, 3> xxww;
      permutation<P, T, D, 0, 1, 0, 3> xyxw;
      permutation<P, T, D, 0, 1, 1, 3> xyyw;
      permutation<P, T, D, 0, 1, 2, 3> xyzw;
      permutation<P, T, D, 0, 1, 3, 0> xywx;
      permutation<P, T, D, 0, 1, 3, 1> xywy;
      permutation<P, T, D, 0, 1, 3, 2> xywz;
      permutation<P, T, D, 0, 1, 3, 3> xyww;
      permutation<P, T, D, 0, 2, 0, 3> xzxw;
      permutation<P, T, D, 0, 2, 1, 3> xzyw;
      permutation<P, T, D, 0, 2, 2, 3> xzzw;
      permutation<P, T, D, 0, 2, 3, 0> xzwx;
      permutation<P, T, D, 0, 2, 3, 1> xzwy;
      permutation<P, T, D, 0, 2, 3, 2> xzwz;
      permutation<P, T, D, 0, 2, 3, 3> xzww;
      permutation<P, T, D, 0, 3, 0, 0> xwxx;
      permutation<P, T, D, 0, 3, 0, 1> xwxy;
      permutation<P, T, D, 0, 3, 0, 2> xwxz;
      permutation<P, T, D, 0, 3, 0, 3> xwxw;
      permutation<P, T, D, 0, 3, 1, 0> xwyx;
      permutation<P, T, D, 0, 3, 1, 1> xwyy;
      permutation<P, T, D, 0, 3, 1, 2> xwyz;
      permutation<P, T, D, 0, 3, 1, 3> xwyw;
      permutation<P, T, D, 0, 3, 2, 0> xwzx;
      permutation<P, T, D, 0, 3, 2, 1> xwzy;
      permutation<P, T, D, 0, 3, 2, 2> xwzz;
      permutation<P, T, D, 0, 3, 2, 3> xwzw;
      permutation<P, T, D, 0, 3, 3, 0> xwwx;
      permutation<P, T, D, 0, 3, 3, 1> xwwy;
      permutation<P, T, D, 0, 3, 3, 2> xwwz;
      permutation<P, T, D, 0, 3, 3, 3> xwww;
      permutation<P, T, D, 1, 0, 0, 3> yxxw;
      permutation<P, T, D, 1, 0, 1, 3> yxyw;
      permutation<P, T, D, 1, 0, 2, 3> yxzw;
      permutation<P, T, D, 1, 0, 3, 0> yxwx;
      permutation<P, T, D, 1, 0, 3, 1> yxwy;
      permutation<P, T, D, 1, 0, 3, 2> yxwz;
      permutation<P, T, D, 1, 0, 3, 3> yxww;
      permutation<P, T, D, 1, 1, 0, 3> yyxw;
      permutation<P, T, D, 1, 1, 1, 3> yyyw;
      permutation<P, T, D, 1, 1, 2, 3> yyzw;
      permutation<P, T, D, 1, 1, 3, 0> yywx;
      permutation<P, T, D, 1, 1, 3, 1> yywy;
      permutation<P, T, D, 1, 1, 3, 2> yywz;
      permutation<P, T, D, 1, 1, 3, 3> yyww;
      permutation<P, T, D, 1, 2, 0, 3> yzxw;
      permutation<P, T, D, 1, 2, 1, 3> yzyw;
      permutation<P, T, D, 1, 2, 2, 3> yzzw;
      permutation<P, T, D, 1, 2, 3, 0> yzwx;
      permutation<P, T, D, 1, 2, 3, 1> yzwy;
      permutation<P, T, D, 1, 2, 3, 2> yzwz;
      permutation<P, T, D, 1, 2, 3, 3> yzww;
      permutation<P, T, D, 1, 3, 0, 0> ywxx;
      permutation<P, T, D, 1, 3, 0, 1> ywxy;
      permutation<P, T, D, 1, 3, 0, 2> ywxz;
      permutation<P, T, D, 1, 3, 0, 3> ywxw;
      permutation<P, T, D, 1, 3, 1, 0> ywyx;
      permutation<P, T, D, 1, 3, 1, 1> ywyy;
      permutation<P, T, D, 1, 3, 1, 2> ywyz;
      permutation<P, T, D, 1, 3, 1, 3> ywyw;
      permutation<P, T, D, 1, 3, 2, 0> ywzx;
      permutation<P, T, D, 1, 3, 2, 1> ywzy;
      permutation<P, T, D, 1, 3, 2, 2> ywzz;
      permutation<P, T, D, 1, 3, 2, 3> ywzw;
      permutation<P, T, D, 1, 3, 3, 0> ywwx;
      permutation<P, T, D, 1, 3, 3, 1> ywwy;
      permutation<P, T, D, 1, 3, 3, 2> ywwz;
      permutation<P, T, D, 1, 3, 3, 3> ywww;
      permutation<P, T, D, 2, 0, 0, 3> zxxw;
      permutation<P, T, D, 2, 0, 1, 3> zxyw;
      permutation<P, T, D, 2, 0, 2, 3> zxzw;
      permutation<P, T, D, 2, 0, 3, 0> zxwx;
      permutation<P, T, D, 2, 0, 3, 1> zxwy;
      permutation<P, T, D, 2, 0, 3, 2> zxwz;
      permutation<P, T, D, 2, 0, 3, 3> zxww;
      permutation<P, T, D, 2, 1, 0, 3> zyxw;
      permutation<P, T, D, 2, 1, 1, 3> zyyw;
      permutation<P, T, D, 2, 1, 2, 3> zyzw;
      permutation<P, T, D, 2, 1, 3, 0> zywx;
      permutation<P, T, D, 2, 1, 3, 1> zywy;
      permutation<P, T, D, 2, 1, 3, 2> zywz;
      permutation<P, T, D, 2, 1, 3, 3> zyww;
      permutation<P, T, D, 2, 2, 0, 3> zzxw;
      permutation<P, T, D, 2, 2, 1, 3> zzyw;
      permutation<P, T, D, 2, 2, 2, 3> zzzw;
      permutation<P, T, D, 2, 2, 3, 0> zzwx;
      permutation<P, T, D, 2, 2, 3, 1> zzwy;
      permutation<P, T, D, 2, 2, 3, 2> zzwz;
      permutation<P, T, D, 2, 2, 3, 3> zzww;
      permutation<P, T, D, 2, 3, 0, 0> zwxx;
      permutation<P, T, D, 2, 3, 0, 1> zwxy;
      permutation<P, T, D, 2, 3, 0, 2> zwxz;
      permutation<P, T, D, 2, 3, 0, 3> zwxw;
      permutation<P, T, D, 2, 3, 1, 0> zwyx;
      permutation<P, T, D, 2, 3, 1, 1> zwyy;
      permutation<P, T, D, 2, 3, 1, 2> zwyz;
      permutation<P, T, D, 2, 3, 1, 3> zwyw;
      permutation<P, T, D, 2, 3, 2, 0> zwzx;
      permutation<P, T, D, 2, 3, 2, 1> zwzy;
      permutation<P, T, D, 2, 3, 2, 2> zwzz;
      permutation<P, T, D, 2, 3, 2, 3> zwzw;
      permutation<P, T, D, 2, 3, 3, 0> zwwx;
      permutation<P, T, D, 2, 3, 3, 1> zwwy;
      permutation<P, T, D, 2, 3, 3, 2> zwwz;
      permutation<P, T, D, 2, 3, 3, 3> zwww;
      permutation<P, T, D, 3, 0, 0, 0> wxxx;
      permutation<P, T, D, 3, 0, 0, 1> wxxy;
      permutation<P, T, D, 3, 0, 0, 2> wxxz;
      permutation<P, T, D, 3, 0, 0, 3> wxxw;
      permutation<P, T, D, 3, 0, 1, 0> wxyx;
      permutation<P, T, D, 3, 0, 1, 1> wxyy;
      permutation<P, T, D, 3, 0, 1, 2> wxyz;
      permutation<P, T, D, 3, 0, 1, 3> wxyw;
      permutation<P, T, D, 3, 0, 2, 0> wxzx;
      permutation<P, T, D, 3, 0, 2, 1> wxzy;
      permutation<P, T, D, 3, 0, 2, 2> wxzz;
      permutation<P, T, D, 3, 0, 2, 3> wxzw;
      permutation<P, T, D, 3, 0, 3, 0> wxwx;
      permutation<P, T, D, 3, 0, 3, 1> wxwy;
      permutation<P, T, D, 3, 0, 3, 2> wxwz;
      permutation<P, T, D, 3, 0, 3, 3> wxww;
      permutation<P, T, D, 3, 1, 0, 0> wyxx;
      permutation<P, T, D, 3, 1, 0, 1> wyxy;
      permutation<P, T, D, 3, 1, 0, 2> wyxz;
      permutation<P, T, D, 3, 1, 0, 3> wyxw;
      permutation<P, T, D, 3, 1, 1, 0> wyyx;
      permutation<P, T, D, 3, 1, 1, 1> wyyy;
      permutation<P, T, D, 3, 1, 1, 2> wyyz;
      permutation<P, T, D, 3, 1, 1, 3> wyyw;
      permutation<P, T, D, 3, 1, 2, 0> wyzx;
      permutation<P, T, D, 3, 1, 2, 1> wyzy;
      permutation<P, T, D, 3, 1, 2, 2> wyzz;
      permutation<P, T, D, 3, 1, 2, 3> wyzw;
      permutation<P, T, D, 3, 1, 3, 0> wywx;
      permutation<P, T, D, 3, 1, 3, 1> wywy;
      permutation<P, T, D, 3, 1, 3, 2> wywz;
      permutation<P, T, D, 3, 1, 3, 3> wyww;
      permutation<P, T, D, 3, 2, 0, 0> wzxx;
      permutation<P, T, D, 3, 2, 0, 1> wzxy;
      permutation<P, T, D, 3, 2, 0, 2> wzxz;
      permutation<P, T, D, 3, 2, 0, 3> wzxw;
      permutation<P, T, D, 3, 2, 1, 0> wzyx;
      permutation<P, T, D, 3, 2, 1, 1> wzyy;
      permutation<P, T, D, 3, 2, 1, 2> wzyz;
      permutation<P, T, D, 3, 2, 1, 3> wzyw;
      permutation<P, T, D, 3, 2, 2, 0> wzzx;
      permutation<P, T, D, 3, 2, 2, 1> wzzy;
      permutation<P, T, D, 3, 2, 2, 2> wzzz;
      permutation<P, T, D, 3, 2, 2, 3> wzzw;
      permutation<P, T, D, 3, 2, 3, 0> wzwx;
      permutation<P, T, D, 3, 2, 3, 1> wzwy;
      permutation<P, T, D, 3, 2, 3, 2> wzwz;
      permutation<P, T, D, 3, 2, 3, 3> wzww;
      permutation<P, T, D, 3, 3, 0, 0> wwxx;
      permutation<P, T, D, 3, 3, 0, 1> wwxy;
      permutation<P, T, D, 3, 3, 0, 2> wwxz;
      permutation<P, T, D, 3, 3, 0, 3> wwxw;
      permutation<P, T, D, 3, 3, 1, 0> wwyx;
      permutation<P, T, D, 3, 3, 1, 1> wwyy;
      permutation<P, T, D, 3, 3, 1, 2> wwyz;
      permutation<P, T, D, 3, 3, 1, 3> wwyw;
      permutation<P, T, D, 3, 3, 2, 0> wwzx;
      permutation<P, T, D, 3, 3, 2, 1> wwzy;
      permutation<P, T, D, 3, 3, 2, 2> wwzz;
      permutation<P, T, D, 3, 3, 2, 3> wwzw;
      permutation<P, T, D, 3, 3, 3, 0> wwwx;
      permutation<P, T, D, 3, 3, 3, 1> wwwy;
      permutation<P, T, D, 3, 3, 3, 2> wwwz;
      permutation<P, T, D, 3, 3, 3, 3> wwww;
    };
  };
}
