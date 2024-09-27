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
// friend simd_mask operator&&(const simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask operator||(const simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask operator& (const simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask operator| (const simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask operator^ (const simd_mask& lhs, const simd_mask& rhs) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMaskBinary {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> l_simd(static_cast<_Tp>(5));
    ex::simd<_Tp, SimdAbi> r_simd([](_Tp i) { return i; });

    const ex::simd_mask<_Tp, SimdAbi> lhs(l_simd == r_simd);
    const ex::simd_mask<_Tp, SimdAbi> rhs(l_simd > r_simd);

    {
      ex::simd_mask<_Tp, SimdAbi> logical_and_mask(lhs && rhs);
      for (size_t i = 0; i < logical_and_mask.size(); ++i) {
        assert(logical_and_mask[i] == (lhs[i] && rhs[i]));
      }
    }
    {
      ex::simd_mask<_Tp, SimdAbi> logical_or_mask(lhs || rhs);
      for (size_t i = 0; i < logical_or_mask.size(); ++i) {
        assert(logical_or_mask[i] == (lhs[i] || rhs[i]));
      }
    }
    {
      ex::simd_mask<_Tp, SimdAbi> bitwise_and_mask(lhs & rhs);
      for (size_t i = 0; i < bitwise_and_mask.size(); ++i) {
        assert(bitwise_and_mask[i] == (lhs[i] && rhs[i]));
      }
    }
    {
      ex::simd_mask<_Tp, SimdAbi> bitwise_or_mask(lhs | rhs);
      for (size_t i = 0; i < bitwise_or_mask.size(); ++i) {
        assert(bitwise_or_mask[i] == (lhs[i] || rhs[i]));
      }
    }
    {
      ex::simd_mask<_Tp, SimdAbi> bitwise_xor_mask(lhs ^ rhs);
      for (size_t i = 0; i < bitwise_xor_mask.size(); ++i) {
        assert(bitwise_xor_mask[i] == (lhs[i] ^ rhs[i]));
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
  test_all_simd_abi<CheckSimdMaskBinary>();
  return 0;
}
