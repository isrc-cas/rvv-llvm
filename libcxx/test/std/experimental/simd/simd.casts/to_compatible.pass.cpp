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
// template<class T, int N> simd<T> to_compatible(const fixed_size_simd<T, N>&) noexcept;
// template<class T, int N> simd_mask<T> to_compatible(const fixed_size_simd_mask<T, N>&) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckToCompatibleSimd {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    static_assert(
        std::is_same_v<decltype(ex::to_compatible<_Tp, _Np>(ex::fixed_size_simd<_Tp, _Np>())), ex::simd<_Tp>>, "");
    static auto InitializeSimd = [](auto& origin_simd) {
      const size_t simd_size = origin_simd.size();
      for (size_t i = 0; i < simd_size; ++i)
        origin_simd[i] = i;
    };

    {
      auto origin = ex::fixed_size_simd<_Tp, _Np>();
      InitializeSimd(origin);
      auto after_cast = ex::to_compatible<_Tp, _Np>(origin);
      std::array<_Tp, origin.size()> expected_values;
      for (size_t i = 0; i < origin.size(); ++i)
        expected_values[i] = static_cast<_Tp>(i);
      assert_simd_value_correct(after_cast, expected_values);
    }
  }
};
struct CheckToCompatibleSimdMask {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    static_assert(
        std::is_same_v<decltype(ex::to_compatible<_Tp, _Np>(ex::fixed_size_simd<_Tp, _Np>())), ex::simd<_Tp>>, "");
    static auto InitializeSimdMask = [](auto& origin_simd_mask) {
      const size_t mask_size = origin_simd_mask.size();
      for (size_t i = 0; i < mask_size; ++i)
        origin_simd_mask[i] = static_cast<bool>(i);
    };

    {
      auto origin = ex::fixed_size_simd_mask<_Tp, _Np>();
      InitializeSimdMask(origin);
      auto after_cast = ex::to_compatible<_Tp, _Np>(origin);
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
  F{}.template operator()<_Tp, SimdAbi, ex::simd_size_v<_Tp, ex::simd_abi::compatible<_Tp>>>();
  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<CheckToCompatibleSimd>();
  test_all_simd_abi<CheckToCompatibleSimdMask>();
  return 0;
}
