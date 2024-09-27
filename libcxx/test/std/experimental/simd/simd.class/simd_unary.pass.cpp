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
// simd& operator++() noexcept;
// simd operator++(int) noexcept;
// simd& operator--() noexcept;
// simd operator--(int) noexcept;
// mask_type operator!() const noexcept;
// simd operator~() const noexcept;
// simd operator+() const noexcept;
// simd operator-() const noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdUnaryOperator {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    constexpr size_t array_size = ex::simd_size_v<_Tp, SimdAbi>;
    {
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

      std::array<_Tp, array_size> origin_value;
      for (size_t i = 0; i < array_size; i++)
        origin_value[i] = static_cast<_Tp>(i);

      auto after_increment_simd = ++origin_simd;
      static_assert(std::is_same_v<decltype(after_increment_simd), ex::simd<_Tp, SimdAbi>>);

      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; i++)
        expected_value[i] = static_cast<_Tp>(origin_value[i] + 1);

      assert_simd_value_correct<array_size>(after_increment_simd, expected_value);
      assert_simd_value_correct<array_size>(origin_simd, expected_value);
    }
    {
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

      std::array<_Tp, array_size> origin_value;
      for (size_t i = 0; i < array_size; i++)
        origin_value[i] = static_cast<_Tp>(i);

      auto after_increment_simd = origin_simd++;
      static_assert(std::is_same_v<decltype(after_increment_simd), ex::simd<_Tp, SimdAbi>>);

      assert_simd_value_correct<array_size>(after_increment_simd, origin_value);

      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; i++)
        expected_value[i] = static_cast<_Tp>(origin_value[i] + 1);

      assert_simd_value_correct<array_size>(origin_simd, expected_value);
    }
    {
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 1); });

      std::array<_Tp, array_size> origin_value;
      for (size_t i = 0; i < array_size; i++)
        origin_value[i] = static_cast<_Tp>(i + 1);

      auto after_decrement_simd = --origin_simd;
      static_assert(std::is_same_v<decltype(after_decrement_simd), ex::simd<_Tp, SimdAbi>>);

      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; i++)
        expected_value[i] = static_cast<_Tp>(origin_value[i] - 1);

      assert_simd_value_correct<array_size>(after_decrement_simd, expected_value);
      assert_simd_value_correct<array_size>(origin_simd, expected_value);
    }
    {
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 1); });

      std::array<_Tp, array_size> origin_value;
      for (size_t i = 0; i < array_size; i++)
        origin_value[i] = static_cast<_Tp>(i + 1);

      auto after_decrement_simd = origin_simd--;
      static_assert(std::is_same_v<decltype(after_decrement_simd), ex::simd<_Tp, SimdAbi>>);

      assert_simd_value_correct<array_size>(after_decrement_simd, origin_value);

      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; i++)
        expected_value[i] = static_cast<_Tp>(origin_value[i] - 1);

      assert_simd_value_correct<array_size>(origin_simd, expected_value);
    }
    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

      auto origin_mask = !origin_simd;
      static_assert(std::is_same_v<decltype(origin_mask), ex::simd_mask<_Tp, SimdAbi>>);

      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = !static_cast<_Tp>(i);

      assert_simd_mask_value_correct<array_size, bool>(origin_mask, expected_value);
    }
    {
      if constexpr (std::is_integral_v<_Tp> && !std::is_unsigned_v<_Tp>) {
        const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

        auto after_inverse_simd = ~origin_simd;
        static_assert(std::is_same_v<decltype(after_inverse_simd), ex::simd<_Tp, SimdAbi>>);

        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = ~static_cast<_Tp>(i);

        assert_simd_value_correct<array_size>(after_inverse_simd, expected_value);
      }
    }
    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = static_cast<_Tp>(i);

      auto expected_simd = +origin_simd;
      static_assert(std::is_same_v<decltype(expected_simd), ex::simd<_Tp, SimdAbi>>);

      assert_simd_value_correct<array_size>(expected_simd, expected_value);
    }
    {
      if constexpr (!std::is_unsigned_v<_Tp>) {
        const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });

        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = -static_cast<_Tp>(i);

        auto expected_simd = -origin_simd;
        static_assert(std::is_same_v<decltype(expected_simd), ex::simd<_Tp, SimdAbi>>);

        assert_simd_value_correct<array_size>(expected_simd, expected_value);
      }
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
  test_all_simd_abi<CheckSimdUnaryOperator>();
  return 0;
}
