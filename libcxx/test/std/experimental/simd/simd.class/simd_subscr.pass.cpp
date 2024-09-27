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
// [simd.class]
// reference operator[](size_t i);
// value_type operator[](size_t i) const;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdReferenceSubscr {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
    for (size_t i = 0; i < origin_simd.size(); ++i) {
      static_assert(is_simd_reference<decltype(origin_simd[i])>::value);
      assert(origin_simd[i] == static_cast<_Tp>(i));
    }
  }
};
struct CheckSimdValueTypeSubscr {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
    for (size_t i = 0; i < origin_simd.size(); ++i) {
      static_assert(std::is_pod_v<decltype(origin_simd[i])>);
      static_assert(std::is_same_v<_Tp, decltype(origin_simd[i])>);
      assert(origin_simd[i] == static_cast<_Tp>(i));
    }
  }
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  F{}.template operator()<_Tp, SimdAbi, _Np>();
  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<CheckSimdReferenceSubscr>();
  test_all_simd_abi<CheckSimdValueTypeSubscr>();
  return 0;
}
