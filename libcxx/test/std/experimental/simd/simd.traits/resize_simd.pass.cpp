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
// [simd.traits]
// template<int N, class V> struct resize_simd { using type = see below ; };
// template<int N, class V> using resize_simd_t = typename resize_simd<N, V>::type;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

class CheckResizeSimdTrue {
public:
  template <class _Tp, std::size_t _Np, class SimdAbi, class Func>
  void operator()(Func func) {
    using U1 = ex::resize_simd<func(ex::simd_size_v<_Tp, SimdAbi>, _Np), ex::simd_mask<_Tp, SimdAbi>>;
    using V1 = ex::simd_mask<_Tp, SimdAbi>;
    static_assert(std::is_same_v<
                  typename U1::type,
                  ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, func(ex::simd_size_v<_Tp, SimdAbi>, _Np), SimdAbi>>>);

    using U2 = ex::resize_simd<func(ex::simd_size_v<_Tp, SimdAbi>, _Np), ex::simd<_Tp, SimdAbi>>;
    using V2 = ex::simd<_Tp, SimdAbi>;
    static_assert(
        std::is_same_v<typename U2::type,
                       ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, func(ex::simd_size_v<_Tp, SimdAbi>, _Np), SimdAbi>>>);

    using U3 = ex::resize_simd_t<func(ex::simd_size_v<_Tp, SimdAbi>, _Np), ex::simd_mask<_Tp, SimdAbi>>;
    using V3 = ex::simd_mask<_Tp, SimdAbi>;
    static_assert(std::is_same_v<
                  U3,
                  ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, func(ex::simd_size_v<_Tp, SimdAbi>, _Np), SimdAbi>>>);

    using U4 = ex::resize_simd_t<func(ex::simd_size_v<_Tp, SimdAbi>, _Np), ex::simd<_Tp, SimdAbi>>;
    using V4 = ex::simd<_Tp, SimdAbi>;
    static_assert(
        std::is_same_v<U4,
                       ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, func(ex::simd_size_v<_Tp, SimdAbi>, _Np), SimdAbi>>>);
  }
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  F{}.template operator()<_Tp, _Np, SimdAbi, std::plus<std::size_t>>(std::plus<std::size_t>());
  if constexpr (ex::simd_size_v<_Tp, SimdAbi> >= _Np) {
    F{}.template operator()<_Tp, _Np, SimdAbi, std::divides<std::size_t>>(std::divides<std::size_t>());
  }
  F{}.template operator()<_Tp, _Np, SimdAbi, std::multiplies<std::size_t>>(std::multiplies<std::size_t>());
  if constexpr (ex::simd_size_v < _Tp, SimdAbi >> _Np) {
    F{}.template operator()<_Tp, _Np, SimdAbi, std::minus<std::size_t>>(std::minus<std::size_t>());
  }

  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}
int main(int, char**) {
  test_all_simd_abi<CheckResizeSimdTrue>();
  return 0;
}
