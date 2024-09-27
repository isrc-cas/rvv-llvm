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
// [simd.casts]
// template <class T, class Abi>
// fixed_size_simd<T, simd_size_v<T, Abi>> to_fixed_size(const simd<T, Abi>&) noexcept;
// template<class T, class Abi>
// fixed_size_simd_mask<T, simd_size_v<T, Abi>> to_fixed_size(const simd_mask<T, Abi>&) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckToFixedSizeSimd {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static auto InitializeSimd = [](auto& origin_simd) {
      const size_t simd_size = origin_simd.size();
      for (size_t i = 0; i < simd_size; ++i)
        origin_simd[i] = static_cast<_Tp>(i);
    };
    {
      ex::simd<_Tp, SimdAbi> origin;
      InitializeSimd(origin);
      auto after_cast = ex::to_fixed_size<_Tp, SimdAbi>(origin);
      static_assert(std::is_same_v< ex::fixed_size_simd<_Tp, ex::simd_size_v<_Tp, SimdAbi>>, decltype(after_cast)>, "");
      std::array<_Tp, origin.size()> expected_values;
      for (size_t i = 0; i < origin.size(); ++i)
        expected_values[i] = static_cast<_Tp>(i);
      assert_simd_value_correct(after_cast, expected_values);
    }
  }
};
struct CheckToFixedSizeSimdMask {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static auto InitializeSimdMask = [](auto& origin_simd_mask) {
      const size_t mask_size = origin_simd_mask.size();
      for (size_t i = 0; i < mask_size; ++i)
        origin_simd_mask[i] = static_cast<bool>(i);
    };
    {
      ex::simd_mask<_Tp, SimdAbi> origin{};
      InitializeSimdMask(origin);
      auto after_cast = ex::to_fixed_size<_Tp, SimdAbi>(origin);
      static_assert(
          std::is_same_v< ex::fixed_size_simd_mask<_Tp, ex::simd_size_v<_Tp, SimdAbi>>, decltype(after_cast)>, "");
      std::array<_Tp, origin.size()> expected_values;
      for (size_t i = 0; i < origin.size(); ++i)
        expected_values[i] = static_cast<bool>(i);
      assert_simd_mask_value_correct(after_cast, expected_values);
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
  test_all_simd_abi<CheckToFixedSizeSimd>();
  test_all_simd_abi<CheckToFixedSizeSimdMask>();
  return 0;
}
