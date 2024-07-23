//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <experimental/simd>
//
// [simd.math]

#include "../test_utils.h"
#include <cassert>
#include <experimental/simd>

#include <cmath>
#include<cfloat>
#include <iostream>
#include <iomanip>
 
using namespace std;

namespace ex = std::experimental::parallelism_v2;

#define LLVM_NAN_32F (float)(0.0 / 0.0)
#define LLVM_NAN_64F (double)(0.0 / 0.0)
#define LLVM_NAN_128F (long double)(0.0 / 0.0)

#define LLVM_INF_32F (float)(1.0 / 0.0)
#define LLVM_INF_64F (double)(1.0 / 0.0)
#define LLVM_INF_128F (long double)(1.0 / 0.0)

template <typename T> inline void write(T x){//还要写T是因为x不确定
  printf("\n*****************\n");
  static int sta[45];//定义一个局部静态数组，存储数位，用int是因为每个下标上只存一个数，用于模拟栈
  int cnt = 0;//定义一个变量，用作索引，用int理由同上
  do{
      sta[cnt++] = x%10, x/= 10;//用去尾巴法把目标数每一位所对应的数都丢进栈里
  }while(x);//x没了，不存了
  while(cnt) putchar(sta[--cnt] + '0');//倒序输出，因为sta为栈，即为正序
  printf("\n*****************\n");
}


struct CheckSimdMathArithmetic {
  template <class _Tp, int _Np>
  using fixed_size_simd = ex::simd<_Tp, ex::simd_abi::fixed_size<_Np>>;
  template <class T, class V> using samesize = fixed_size_simd<T, V::size()>;




  template <class _Tp, class SimdAbi>
  void operator()() {
    using scharv = ex::simd<signed char, SimdAbi>;
    using shortv = ex::simd<short, SimdAbi>;
    using intv = ex::simd<int, SimdAbi>;
    using longv = ex::simd<long int, SimdAbi>;
    using llongv = ex::simd<long long int, SimdAbi>;
    using floatv = ex::simd<float, SimdAbi>; 
    using doublev = ex::simd<double, SimdAbi>;
    using ldoublev = ex::simd<long double, SimdAbi>;
    using uintv = ex::simd<unsigned int, SimdAbi>;
    using intfv = samesize<int, floatv>;
    using longdv = samesize<int, doublev>;
    using llongdv = samesize<int, ldoublev>;
    using tpv = ex::simd<_Tp, SimdAbi>;

    {
      cout << "int is " << sizeof(int) << " bits." << endl;
      cout << "long is " << sizeof(long) << " bits." << endl;
      cout << "long long is " << sizeof(long long) << " bits." << endl;
      cout << "float is " << sizeof(float) << " bits." << endl;
      cout << "double is " << sizeof(double) << " bits." << endl;
      cout << "long double is " << sizeof(long double) << " bits." << endl;
      // printf(" *** %d %d %d \n", FLT_MANT_DIG,DBL_MANT_DIG,LDBL_MANT_DIG);
    }

    // erfc
    // {
    //   floatv fvx([](auto i) { return -1.12348f + i*(0.3f); });
    //   floatv fvy = ex::erfc(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = erfc(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }
    // {
    //   // doublev fvx([](auto i) { return -1.12348 + i*(0.3); });
    //   doublev fvx(0.99609396575818465180);
      
    //   // doublev fvx([](auto i) { return LLVM_INF_64F+i; });

    //   doublev fvy = ex::erfc(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = erfc(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }

    // erf
    // {
    //   floatv fvx([](auto i) { return -2.12348f + i*(-0.9f); });
    //   floatv fvy = ex::erf(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = erf(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }
    // {
    //   doublev fvx([](auto i) { return 2.12348 + i*(-0.9); });
    //   // doublev fvx([](auto i) { return LLVM_INF_64F+i; });

    //   doublev fvy = ex::erf(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = erf(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }

    // hypot
    // {
    //   floatv fvx([](auto i) { return -2.12348f + i*(-0.9f); });
    //   floatv fvn([](auto i) { return -2.12348f + i*(-0.1f); });
    //   floatv fvy = ex::hypot(fvx, fvn);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = hypot(fvx[i], fvn[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvn[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }

    // {
    //   doublev fvx([](auto i) { return -2.12348 + i*(-0.9); });
    //   doublev fvn([](auto i) { return -2.12348 + i*(-0.1); });
    //   doublev fvy = ex::hypot(fvx, fvn);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     double fy = hypot(fvx[i], fvn[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvn[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }

    // scalbn
    // {
    //   floatv fvx([](auto i) { return 2.12348f + i*(-0.9f); });
    //   intfv fvn([](auto i) { return (int)i; });
    //   floatv fvy = ex::scalbn(fvx, fvn);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = scalbn(fvx[i], fvn[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvn[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // {
    //   doublev fvx([](auto i) { return 2.12348 + i*(-0.9); });
    //   longdv fvn([](auto i) { return (int)i; });
    //   doublev fvy = ex::scalbn(fvx, fvn);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     double fy = scalbn(fvx[i], fvn[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvn[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // {
    //   ldoublev fvx([](auto i) { return 2.12348 + i*(-0.9); });
    //   llongdv fvn([](auto i) { return (int)i; });
    //   ldoublev fvy = ex::scalbn(fvx, fvn);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double fy = scalbn(fvx[i], fvn[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvn[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // cbrt
    // {
    //   floatv fvx([](auto i) { return LLVM_INF_32F-i; });
    //   floatv fvy = ex::cbrt(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = cbrt(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }
    // {
    //   // doublev fvx([](auto i) { return 2.12348f + i*(-0.9f); });
    //   doublev fvx([](auto i) { return LLVM_INF_64F+i; });

    //   doublev fvy = ex::cbrt(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = cbrt(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     // assert(fvy[i] == (fy));
    //   }
    //   assert(true);
    // }

    // {
    //   // ldoublev fvx([](auto i) { return 1.11232348 + i*0.84312315; });
    //   ldoublev fvx([](auto i) { return 0 + 0.0000001*i*i; });

    //   ldoublev fvy = ex::cbrt(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double fy = cbrt(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(true);
    //   }
    // }

    // asin
    // {
    //   ldoublev fvx([](auto i) { return 1.11232348 + i*0.84312315; });
    //   ldoublev fvy = ex::asin(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double fy = asin(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(true);
    //   }
    // }


    // isless
    // {
    //   floatv fvx1([](auto i) { return (-2.f+i);});
    //   floatv fvx2(LLVM_NAN_32F);

    //   // floatv fvx2([](auto i) { return (3.f-i) ;});

    //   // fvx2[3] = LLVM_NAN_32F;
    //   // fvx1[6] = LLVM_INF_32F;
    //   auto fvy = ex::isless(fvx1, fvx2);
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     // int fy = remainder(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy[i] << "   " << '\n';
    //     assert(true);
    //   }
    // }

    // signbit
    // {
    //   // floatv fvx(LLVM_INF_32F);
    //   // floatv fvx(LLVM_NAN_32F);
    //   // floatv fvx(11232.31231231f);
    //   // doublev fvx(LLVM_INF_64F);

    //   floatv fvx(-1561.45616f);
    //   // ldoublev fvx(LLVM_NAN_128F);


    //   // ldoublev fvx(1.15648949823165);
    //   auto fvy = ex::signbit(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << '\n';
    //   }
    //   assert(true);
    // }


    // isnan
    // {
    //   // floatv fvx(log(-1.f));
    //   // floatv fvx(1.f);
    //   doublev fvx(LLVM_INF_64F);
    //   // ldoublev fvx(LLVM_NAN_128F);
    //   auto fvy = ex::isnan(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << '\n';
    //   }
    //   assert(true);
    // }

    // isinf
    // {
    //   // floatv fvx(LLVM_INF_32F);
    //   // floatv fvx(LLVM_NAN_32F);
    //   // floatv fvx(1123213213124.31231231f);
    //   // doublev fvx(LLVM_INF_64F);

    //   // floatv fvx(-1561.45616f);
    //   // ldoublev fvx(LLVM_NAN_128F);


    //   // ldoublev fvx(1.15648949823165);
    //   auto fvy = ex::isfinite(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << '\n';
    //   }
    //   assert(true);
    // }

    // div
    // {
    //   scharv fvx1([](auto i) { return (signed char)(-20+i);});
    //   scharv fvx2([](auto i) { return (signed char)(3+i) ;});
    //   auto fvy = ex::div(fvx1, fvx2);
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     auto fy = div(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy.quot[i] << "   " << fvy.rem[i] << "   " << '\n';
    //     assert(fvy.quot[i] == fy.quot);
    //     assert(fvy.rem[i] == fy.rem);
    //   }
    // }
    // {
    //   intv fvx1([](auto i) { return (int)(-20+i);});
    //   intv fvx2([](auto i) { return (int)(3+i) ;});
    //   auto fvy = ex::div(fvx1, fvx2);
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     // int fy = remainder(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy.quot[i] << "   " << fvy.rem[i] << "   " << '\n';
    //     assert(true);
    //   }
    // }

    // abs
    // {
    //   shortv fvx([](auto i) { return (short)(3-i); });
    //   shortv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     short fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   scharv fvx([](auto i) { return (signed char)(3-i); });
    //   scharv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //      signed char fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   intv fvx([](auto i) { return 2 + ( int)i*(-1); });
    //   intv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //      int fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   floatv fvx([](auto i) { return 2.12348f + i*(-0.9f); });
    //   floatv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 2.12348 + i*(-0.9); });
    //   doublev fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     double fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   ldoublev fvx([](auto i) { return 5.12348321321 + i*(-0.9231234324); });
    //   ldoublev fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   shortv fvx([](auto i) { return (short)(3-i); });
    //   shortv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     short fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   scharv fvx([](auto i) { return (signed char)(3-i); });
    //   scharv fvy = ex::abs(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     signed char fy = abs(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // remainder
    // {
    //   ldoublev fvx1([](auto i) { return 2.12342131238 +  i * -0.8431231215; });
    //   ldoublev fvx2([](auto i) { return 1.12312213 +  i * 0.123123123; });
    //   ldoublev fvy = ex::remainder(fvx1, fvx2);
    //   cout<< "start " << endl;
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     long double fy = remainder(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy[i] << "   " << fy << '\n';

    //     assert(fvy[i] == (fy));
    //   }
    //   cout<< "end " << endl;
    // }

    // fmod
    // {
    //   ldoublev fvx1([](auto i) { return 2.12342131238 +  i * -0.8431231215; });
    //   ldoublev fvx2([](auto i) { return 1.12312213 +  i * 0.123123123; });
    //   ldoublev fvy = ex::fmod(fvx1, fvx2);
    //   cout<< "start " << endl;
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     long double fy = fmod(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy[i] << "   " << fy << '\n';

    //     assert(fvy[i] == (fy));
    //   }
    //   cout<< "end " << endl;
    // }

    // fmin
    // {
    //   floatv fvx1([](auto i) { return 2.12348f +  i * -0.84315f; });
    //   floatv fvx2([](auto i) { return 1.12348f +  i * 0.481154f; });
    //   floatv fvy = ex::fmin(fvx1, fvx2);
    //   for (unsigned i = 0; i < fvx1.size(); ++i){
    //     float fy = fmin(fvx1[i], fvx2[i]);
    //     cout<< i << "   " << fvx1[i] << "   " << fvx2[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // ilogb
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   intfv fvy = ex::ilogb(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     int fy = ilogb(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   llongdv fvy = ex::ilogb(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     int fy = ilogb(fvx[i]);
    //     printf("%d   %-.16lf   %-.16ld   %-.16ld\n", (int)i, (float)fvx[i], (long)fvy[i], (long)fy);
    //     // cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // sinh
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   floatv fvy = ex::sinh(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = sinh(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   cout<< " sinh doublev start  " << endl;
    //   doublev fvx([](auto i) { return 0.034834485 + i*0.1915648; });
    //   doublev fvy = ex::sinh(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = sinh(fvx[i]);
    //       cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //       assert(true);
    //   }
    //   cout<< " sinh doublev end  " << endl;
    // }

    // sin
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.84315f; });
    //   floatv fvy = ex::sin(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = sin(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(true);
    //   }
    // }

    // round
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.84315f; });
    //   floatv fvy = ex::round(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = round(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   ldoublev fvx([](auto i) { return 1.12348123123 + i*0.84315123123; });
    //   ldoublev fvy = ex::round(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double fy = round(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }

    // acos
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   floatv fvy = ex::acos(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = acos(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 0.034834485 + i*0.1915648; });
    //   doublev fvy = ex::acos(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = acos(fvx[i]);
    //       cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //       assert(true);
    //   }
    // }

    // // pow
    // {
    //   floatv fvx([](auto i) { return 1.1f + i*0.009f; });
    //   floatv n([](auto i) { return 1.f + i*1.f; });
    //   floatv fvy = ex::pow(fvx, n);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = pow(fvx[i], n[i]);
    //     // assert(fvy[i] == (fy));
    //     cout<< i << "   " << fvx[i] << "   " << n[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(true);
    //   }
    // }

    // {
    //   doublev fvx([](auto i) { return 1.1 + i*0.009; });
    //   doublev n([](auto i) { return 1.0 + i; });
    //   doublev fvy = ex::pow(fvx, n);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     double fy = pow(fvx[i], n[i]);
    //     // assert(fvy[i] == (fy));
    //     cout<< i << "   " << fvx[i] << "   " << n[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(true);
    //   }
    // }


    // exp
    // {
    //   floatv fvx([](auto i) { return 1.12834f + i*0.91548f; });
    //   floatv fvy = ex::exp(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       float fy = exp(fvx[i]);
    //       cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //       // assert(fvy[i] == (fy));
    //       assert(true);
    //   }
    // }

    // log
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   floatv fvy = ex::log(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = log(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 0.034834485 + i*0.1915648; });
    //   doublev fvy = ex::log(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = log(fvx[i]);
    //       cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //       assert(true);
    //   }
    // }
    // // sqrt
    // {
    //   floatv fvx([](auto i) { return 1.12348f + i*0.9f; });
    //   floatv fvy = ex::sqrt(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float fy = sqrt(fvx[i]);
    //     cout<< i << "   " << fvx[i] << "   " << fvy[i] << "   " << fy << '\n';
    //     assert(fvy[i] == (fy));
    //   }
    // }
    // // modf
    // {
    //   floatv fvx([](auto i) { return 1.1f + i*0.009f; });
    //   floatv intpartv;
    //   floatv fvy = ex::modf(fvx, &intpartv);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     float intpart;
    //     float fy = modf(fvx[i], &intpart);
    //     assert(intpartv[i] == (intpart));
    //     assert(fvy[i] == (fy));
    //     // cout<< i << "   " << intpartv[i] << "   "  << intpart << "   " << fvy[i] << "   " << fy << '\n';
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 1.1f + i*0.009f; });
    //   doublev intpartv;
    //   doublev fvy = ex::modf(fvx, &intpartv);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     double intpart;
    //     double fy = modf(fvx[i], &intpart);
    //     assert(intpartv[i] == (intpart));
    //     assert(fvy[i] == (fy));
    //     cout<< i << "   " << intpartv[i] << "   "  << intpart << "   " << fvy[i] << "   " << fy << '\n';
    //   }
    // }
    // {
    //   ldoublev fvx([](auto i) { return 1.12312367341 + i*1.235821009; });
    //   ldoublev intpartv;
    //   ldoublev fvy = ex::modf(fvx, &intpartv);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //     long double intpart;
    //     long double fy = modf(fvx[i], &intpart);
    //     assert(intpartv[i] == (intpart));
    //     assert(fvy[i] == (fy));
    //     cout<< i << "   " << intpartv[i] << "   "  << intpart << "   " << fvy[i] << "   " << fy << '\n';
    //   }
    // }
    // // ldexp
    // {
    //   floatv fvx([](auto i) { return 1.f + i; });
    //   intfv iexp([](auto i) { return (int)i; });
    //   floatv fvy = ex::ldexp(fvx, iexp);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       float fy = ldexp(fvx[i], iexp[i]);
    //       assert(fvy[i] == (fy));
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return 1.5 + i; });
    //   llongdv iexp([](auto i) { return (int)i; });
    //   doublev fvy = ex::ldexp(fvx, iexp);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = ldexp(fvx[i], iexp[i]);
    //       assert(fvy[i] == (fy));
    //   }
    // }
    // // floor
    // {
    //   floatv fvx([](auto i) { return -0.5f + i*0.01f; });
    //    fvx[1] = -0.133336;
    //   fvx[2] = -0.837350;
    //   fvx[3] = 0.182934;
    //   floatv fvy = ex::floor(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       float fy = floor(fvx[i]);
    //       cout << i << ' ' << fvx[i] << ' ' << fy << ' ' << fvy[i] << endl;
    //       assert(fvy[i] == fy);
    //   }
    //   printf("hh\n");
    // }
    // {
    //   doublev fvx([](auto i) { return -0.5 + i*0.01; });
    //   // printf("in\n");
    //   //  fvx[1] = -0.133336;
    //   // fvx[2] = -0.837350;
    //   // fvx[3] = 0.182934;
    //   doublev fvy = ex::floor(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = floor(fvx[i]);
    //       printf("%d, %lf", i, fy);
    //       assert(fvy[i] == fy);
    //   }
    //   // printf("yes\n");
    // }
    // // ceil
    // {
    //   floatv fvx([](auto i) { return -0.5f + i; });
    //   floatv fvy = ex::ceil(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       float fy = ceil(fvx[i]);
    //       assert(fvy[i] == fy);
    //   }
    // }
    // {
    //   doublev fvx([](auto i) { return -0.5 + i; });
    //   doublev fvy = ex::ceil(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = ceil(fvx[i]);
    //       assert(fvy[i] == fy);
    //   }
    // }
    
    // add by me
    // nearbyint
    // {
    //   floatv fvx([](auto i) { return -0.5f + i; });
    //   floatv fvy = ex::nearbyint(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       float fy = nearbyintf(fvx[i]);
    //       assert(fvy[i] == fy);
    //   }
    //   cout << "pass" << endl;
    // }
    // {
    //   doublev fvx([](auto i) { return -0.5 + i; });
    //   doublev fvy = ex::nearbyint(fvx);
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       double fy = nearbyint(fvx[i]);
    //       assert(fvy[i] == fy);
    //   }
    //   cout << "pass" << endl;
    // }
    // rint
    {
      doublev fvx = { 0.986844 }; 
      // fvx[1] = -0.133336;
      // fvx[2] = -0.837350;
      // fvx[3] = 0.182934;
      double tmpx[100] = { (double)-0.268166, (double)-0.133336, (double)-0.837350, (double)0.182934, 0 };
      double tmpy[100] = { (double)-0.000231, (double) -0.838987, (double)-0.790092, (double)0.182934, 0 };
      doublev fvy = { 0.986850};

      fvx.copy_from(&tmpx[0], ex::element_aligned);
      fvy.copy_from(&tmpy[0], ex::element_aligned);
      // std::experimental::Abi_64suf<SimdAbi> atmp;

      // fvy[1] = -0.838987;
      // fvy[2] = -0.790092;
      // fvy[3] = 0.182934;
      //fvy { (double)0.938834, (double)0.210974, (double)-0.412827,(double)0.923260 };
      samesize<int, doublev> fvz([](auto i) { return 1 + (int)i; });
      // intv fint([](auto i) { return -1 + i; });
      doublev res = ex::remquo(fvx, fvy, &fvz);
      int a = 0;
      for (unsigned i = 0; i < fvx.size(); ++i){
          int *tmp = &a;
          // double fyy = ex::remquosss(fvx[i], fvy[i], tmp);
          double fy = ex::remquo(fvx[i], fvy[i], tmp);
          // float fvy = ex::rintfff(fvx[i]);
          cout << "i " <<  i << ' ' << " x " <<  fvx[i] << " y " << fvy[i] << endl;
          cout << std::setprecision(10) <<  fy << ' ' << " " << std::setprecision(10) << res[i] << " \n" << endl;
          assert(res[i] == fy);
      }
      cout << "pass" << endl;
      fdim(
      nexttoward()
      )
    }

    


    // {
    //   ldoublev fvx = { (long double)0.986844 }; 
    //   // fvx[1] = -0.133336;
    //   // fvx[2] = -0.837350;
    //   // fvx[3] = 0.182934;
    //   // long double tmpx[100] = { (long double)-0.249345, (long double)-0.133336, (long double)-0.837350, (long double)0.182934, 0 };
    //   // long double tmpy[100] = { (long double)0.250642, (long double) -0.838987, (long double)-0.790092, (long double)0.182934, 0 };
    //   ldoublev fvy = { (long double)0.986850};

    //   // fvx.copy_from(&tmpx[0], ex::element_aligned);
    //   // fvy.copy_from(&tmpy[0], ex::element_aligned);
    //   // std::experimental::Abi_128suf<SimdAbi> atmp;

    //   // fvy[1] = -0.838987;
    //   // fvy[2] = -0.790092;
    //   // fvy[3] = 0.182934;
    //   //fvy { (double)0.938834, (double)0.210974, (double)-0.412827,(double)0.923260 };
    //   // intv fvz([](auto i) { return 1 + (int)i; });
      
    //   // intv fint([](auto i) { return -1 + i; });
    //   ldoublev res = ex::remquo(fvx, fvy);
    //   int a = 0;
    //   for (unsigned i = 0; i < fvx.size(); ++i){
    //       int *tmp = &a;
    //       // double fyy = ex::remquosss(fvx[i], fvy[i], tmp);
    //       long double fy = remquol(fvx[i], fvy[i], &tmp);
    //       // float fvy = ex::rintfff(fvx[i]);
    //       cout << "i " <<  i << ' ' << " x " <<  fvx[i] << " y " << fvy[i] << endl;
    //       cout << std::setprecision(10) <<  fy << ' ' << " " << std::setprecision(10) << res[i] << " \n" << endl;
    //       assert(res[i] == fy);
    //   }
    //   cout << "pass" << endl;
    // }
  }
};


template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  F{}.template operator()<float, SimdAbi>();
  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}
int main(int, char**) {
  test_all_simd_abi<CheckSimdMathArithmetic>();

  return 0;
}

