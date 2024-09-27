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
// simd_mask operator!() const noexcept;

#include "../test_utils.h"
#include <experimental/simd>

struct CheckSimdMaskUnary {
  template <class _Tp, class SimdAbi>
  void operator()() {
    constexpr size_t array_size = ex::simd_size_v<_Tp, SimdAbi>;

    bool origin_value[array_size];
    for (size_t i = 0; i < array_size; i++)
      origin_value[i] = static_cast<bool>(i % 2);

    ex::simd_mask<_Tp, SimdAbi> origin_mask(origin_value, ex::element_aligned_tag());

    ex::simd_mask<_Tp, SimdAbi> after_convert_mask = !origin_mask;

    std::array<bool, array_size> expected_value;
    for (size_t i = 0; i < array_size; i++)
      expected_value[i] = !origin_value[i];

    assert_simd_mask_value_correct<array_size, bool>(after_convert_mask, expected_value);
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
  test_all_simd_abi<CheckSimdMaskUnary>();
  return 0;
}
