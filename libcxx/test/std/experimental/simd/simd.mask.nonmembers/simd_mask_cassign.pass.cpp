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
// friend simd_mask& operator&=(simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask& operator|=(simd_mask& lhs, const simd_mask& rhs) noexcept;
// friend simd_mask& operator^=(simd_mask& lhs, const simd_mask& rhs) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMaskCassign {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> l_simd(static_cast<_Tp>(5));
    ex::simd<_Tp, SimdAbi> r_simd([](_Tp i) { return i; });

    ex::simd_mask<_Tp, SimdAbi> lhs(l_simd == r_simd);
    const ex::simd_mask<_Tp, SimdAbi> rhs(l_simd > r_simd);
    constexpr size_t array_size = ex::simd_size_v<_Tp, SimdAbi>;
    {
      auto origin_lhs = lhs;
      auto and_equal_mask(lhs &= rhs);

      static_assert(std::is_same_v<ex::simd_mask<_Tp, SimdAbi>, decltype(and_equal_mask)>);
      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = (origin_lhs[i] && rhs[i]);

      assert_simd_mask_value_correct(lhs, expected_value);
      assert_simd_mask_value_correct(and_equal_mask, expected_value);

      lhs = origin_lhs;
    }
    {
      auto origin_lhs = lhs;
      auto or_equal_mask(lhs |= rhs);

      static_assert(std::is_same_v<ex::simd_mask<_Tp, SimdAbi>, decltype(or_equal_mask)>);
      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = (origin_lhs[i] || rhs[i]);

      assert_simd_mask_value_correct(lhs, expected_value);
      assert_simd_mask_value_correct(or_equal_mask, expected_value);

      lhs = origin_lhs;
    }
    {
      auto origin_lhs = lhs;
      auto xor_equal_mask(lhs ^= rhs);

      static_assert(std::is_same_v<ex::simd_mask<_Tp, SimdAbi>, decltype(xor_equal_mask)>);
      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = (origin_lhs[i] ^ rhs[i]);

      assert_simd_mask_value_correct(lhs, expected_value);
      assert_simd_mask_value_correct(xor_equal_mask, expected_value);

      lhs = origin_lhs;
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
  test_all_simd_abi<CheckSimdMaskCassign>();
  return 0;
}
