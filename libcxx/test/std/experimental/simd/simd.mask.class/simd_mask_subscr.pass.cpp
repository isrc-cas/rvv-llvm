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
// [simd.mask.class]
// reference operator[](size_t i);
// value_type operator[](size_t i) const;

#include "../test_utils.h"
#include <cassert>
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMaskReferenceSubscr {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    ex::simd<_Tp, SimdAbi> lhs{}, rhs{};
    ex::simd_mask<_Tp, SimdAbi> origin_simd_mask(lhs == rhs);
    for (size_t i = 0; i < origin_simd_mask.size(); ++i) {
      static_assert(is_simd_reference<decltype(origin_simd_mask[i])>::value);
      assert(origin_simd_mask[i] == true);
    }
  }
};
struct CheckSimdMaskValueTypeSubscr {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    ex::simd<_Tp, SimdAbi> lhs{}, rhs{};
    const ex::simd_mask<_Tp, SimdAbi> origin_simd_mask(lhs == rhs);
    for (size_t i = 0; i < origin_simd_mask.size(); ++i) {
      static_assert(std::is_pod_v<decltype(origin_simd_mask[i])>);
      static_assert(std::is_same_v<bool, decltype(origin_simd_mask[i])>);
      assert(origin_simd_mask[i] == true);
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
  test_all_simd_abi<CheckSimdMaskReferenceSubscr>();
  test_all_simd_abi<CheckSimdMaskValueTypeSubscr>();
  return 0;
}
