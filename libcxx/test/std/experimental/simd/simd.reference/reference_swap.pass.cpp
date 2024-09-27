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
// [simd.reference]
// friend void swap(reference&& a, reference&& b) noexcept;
// friend void swap(value_type& a, reference&& b) noexcept;
// friend void swap(reference&& a, value_type& b) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckReferenceSwap {
  template <class _Tp, class SimdAbi>
  void operator()() {
    if constexpr (ex::simd_size_v<_Tp, SimdAbi> >= 2) {
      ex::simd<_Tp, SimdAbi> reference_simd([](_Tp i) { return static_cast<_Tp>(i + 2); });

      {
        assert(reference_simd[0] == static_cast<_Tp>(2));
        assert(reference_simd[1] == static_cast<_Tp>(3));
        swap(reference_simd[0], reference_simd[1]);
        assert(reference_simd[1] == static_cast<_Tp>(2));
        assert(reference_simd[0] == static_cast<_Tp>(3));
      }

      {
        _Tp value_to_swap = static_cast<_Tp>(4);
        assert(value_to_swap == static_cast<_Tp>(4));

        assert(reference_simd[0] == static_cast<_Tp>(3));
        swap(value_to_swap, reference_simd[0]);
        assert(value_to_swap == static_cast<_Tp>(3));
        assert(reference_simd[0] == static_cast<_Tp>(4));
      }
      {
        _Tp value_to_swap = static_cast<_Tp>(6);
        assert(value_to_swap == static_cast<_Tp>(6));

        assert(reference_simd[0] == static_cast<_Tp>(4));
        swap(reference_simd[0], value_to_swap);
        assert(value_to_swap == static_cast<_Tp>(4));
        assert(reference_simd[0] == static_cast<_Tp>(6));
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
  test_all_simd_abi<CheckReferenceSwap>();
  return 0;
}
