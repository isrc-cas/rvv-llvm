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
// [simd.mask.nonmembers]
// friend simd_mask operator==(const simd_mask&, const simd_mask&) noexcept;
// friend simd_mask operator!=(const simd_mask&, const simd_mask&) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMaskComparision {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> l_simd(static_cast<_Tp>(5));
    ex::simd<_Tp, SimdAbi> r_simd([](_Tp i) { return i; });

    const ex::simd_mask<_Tp, SimdAbi> lhs(l_simd == r_simd);
    const ex::simd_mask<_Tp, SimdAbi> rhs(l_simd > r_simd);

    {
      ex::simd_mask<_Tp, SimdAbi> equal_mask(lhs == rhs);
      for (size_t i = 0; i < equal_mask.size(); ++i) {
        assert(equal_mask[i] == (lhs[i] == rhs[i]));
      }
    }
    {
      ex::simd_mask<_Tp, SimdAbi> not_equal_mask(lhs != rhs);
      for (size_t i = 0; i < not_equal_mask.size(); ++i) {
        assert(not_equal_mask[i] == (lhs[i] != rhs[i]));
      }
    }
  }
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  F{}.template operator()<_Tp, SimdAbi>();
  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<CheckSimdMaskComparision>();
  return 0;
}
