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
// [simd.nonmembers]
// friend mask_type operator==(const simd& lhs, const simd& rhs) noexcept;
// friend mask_type operator!=(const simd& lhs, const simd& rhs) noexcept;
// friend mask_type operator>=(const simd& lhs, const simd& rhs) noexcept;
// friend mask_type operator<=(const simd& lhs, const simd& rhs) noexcept;
// friend mask_type operator>(const simd& lhs, const simd& rhs) noexcept;
// friend mask_type operator<(const simd& lhs, const simd& rhs) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdComparison {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> lhs(static_cast<_Tp>(5));
    const ex::simd<_Tp, SimdAbi> rhs([](_Tp i) { return i; });

    {
      const ex::simd_mask<_Tp, SimdAbi> equal_simd_mask(lhs == rhs);
      for (size_t i = 0; i < equal_simd_mask.size(); ++i) {
        assert(equal_simd_mask[i] == (lhs[i] == rhs[i]));
      }
    }
    {
      const ex::simd_mask<_Tp, SimdAbi> not_equal_simd_mask(lhs != rhs);
      for (size_t i = 0; i < not_equal_simd_mask.size(); ++i) {
        assert(not_equal_simd_mask[i] == (lhs[i] != rhs[i]));
      }
    }
    {
      const ex::simd_mask<_Tp, SimdAbi> great_equal_simd_mask(lhs >= rhs);
      for (size_t i = 0; i < great_equal_simd_mask.size(); ++i) {
        assert(great_equal_simd_mask[i] == (lhs[i] >= rhs[i]));
      }
    }
    {
      const ex::simd_mask<_Tp, SimdAbi> less_equal_simd_mask(lhs <= rhs);
      for (size_t i = 0; i < less_equal_simd_mask.size(); ++i) {
        assert(less_equal_simd_mask[i] == (lhs[i] <= rhs[i]));
      }
    }
    {
      const ex::simd_mask<_Tp, SimdAbi> great_simd_mask(lhs > rhs);
      for (size_t i = 0; i < great_simd_mask.size(); ++i) {
        assert(great_simd_mask[i] == (lhs[i] > rhs[i]));
      }
    }
    {
      const ex::simd_mask<_Tp, SimdAbi> less_simd_mask(lhs < rhs);
      for (size_t i = 0; i < less_simd_mask.size(); ++i) {
        assert(less_simd_mask[i] == (lhs[i] < rhs[i]));
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
  test_all_simd_abi<CheckSimdComparison>();
  return 0;
}
