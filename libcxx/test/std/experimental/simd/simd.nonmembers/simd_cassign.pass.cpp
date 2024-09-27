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
// friend simd& operator+=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator-=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator*=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator/=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator%=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator&=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator|=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator^=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator<<=(simd& lhs, const simd& rhs) noexcept;
// friend simd& operator>>=(simd& lhs, const simd& rhs) noexcept;
#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdCassign {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> lhs([](_Tp i) { return static_cast<_Tp>(i + 2); });
    const ex::simd<_Tp, SimdAbi> rhs([]([[maybe_unused]] _Tp i) { return static_cast<_Tp>(2); });
    constexpr std::size_t array_size = lhs.size();
    {
      const auto origin_lhs_backup = lhs;
      auto result_plus_simd        = lhs += rhs;
      static_assert(std::is_same_v<decltype(result_plus_simd), ex::simd<_Tp, SimdAbi>>);
      std::array<_Tp, array_size> expected_value;

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = lhs[i];
      assert_simd_value_correct(result_plus_simd, expected_value);

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = origin_lhs_backup[i] + rhs[i];
      assert_simd_value_correct(result_plus_simd, expected_value);

      lhs = origin_lhs_backup;
    }

    {
      const auto origin_lhs_backup = lhs;
      auto result_minus_simd       = lhs -= rhs;
      static_assert(std::is_same_v<decltype(result_minus_simd), ex::simd<_Tp, SimdAbi>>);
      std::array<_Tp, array_size> expected_value;

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = lhs[i];
      assert_simd_value_correct(result_minus_simd, expected_value);

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = origin_lhs_backup[i] - rhs[i];
      assert_simd_value_correct(result_minus_simd, expected_value);

      lhs = origin_lhs_backup;
    }
    {
      const auto origin_lhs_backup = lhs;
      auto result_multiply_simd    = lhs *= rhs;
      static_assert(std::is_same_v<decltype(result_multiply_simd), ex::simd<_Tp, SimdAbi>>);
      std::array<_Tp, array_size> expected_value;

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = lhs[i];
      assert_simd_value_correct(result_multiply_simd, expected_value);

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = origin_lhs_backup[i] * rhs[i];
      assert_simd_value_correct(result_multiply_simd, expected_value);

      lhs = origin_lhs_backup;
    }
    {
      const auto origin_lhs_backup = lhs;
      auto result_division_simd    = lhs /= rhs;
      static_assert(std::is_same_v<decltype(result_division_simd), ex::simd<_Tp, SimdAbi>>);
      std::array<_Tp, array_size> expected_value;

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = lhs[i];
      assert_simd_value_correct(result_division_simd, expected_value);

      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = origin_lhs_backup[i] / rhs[i];
      assert_simd_value_correct(result_division_simd, expected_value);

      lhs = origin_lhs_backup;
    }
    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_mod_simd         = lhs %= rhs;
        static_assert(std::is_same_v<decltype(result_mod_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_mod_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] % rhs[i];
        assert_simd_value_correct(result_mod_simd, expected_value);

        lhs = origin_lhs_backup;
      }
    }

    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_and_simd         = lhs &= rhs;
        static_assert(std::is_same_v<decltype(result_and_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_and_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] & rhs[i];
        assert_simd_value_correct(result_and_simd, expected_value);

        lhs = origin_lhs_backup;
      }
    }
    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_or_simd          = lhs |= rhs;
        static_assert(std::is_same_v<decltype(result_or_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_or_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] | rhs[i];
        assert_simd_value_correct(result_or_simd, expected_value);

        lhs = origin_lhs_backup;
      }
    }
    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_exor_simd        = lhs ^= rhs;
        static_assert(std::is_same_v<decltype(result_exor_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_exor_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] ^ rhs[i];
        assert_simd_value_correct(result_exor_simd, expected_value);

        lhs = origin_lhs_backup;
      }
    }
    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_shift_left_simd  = lhs <<= rhs;
        static_assert(std::is_same_v<decltype(result_shift_left_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_shift_left_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] << rhs[i];
        assert_simd_value_correct(result_shift_left_simd, expected_value);

        lhs = origin_lhs_backup;
      }
    }
    {
      if constexpr (std::is_integral_v<_Tp>) {
        const auto origin_lhs_backup = lhs;
        auto result_shift_right_simd = lhs >>= rhs;
        static_assert(std::is_same_v<decltype(result_shift_right_simd), ex::simd<_Tp, SimdAbi>>);
        std::array<_Tp, array_size> expected_value;

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = lhs[i];
        assert_simd_value_correct(result_shift_right_simd, expected_value);

        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = origin_lhs_backup[i] >> rhs[i];
        assert_simd_value_correct(result_shift_right_simd, expected_value);

        lhs = origin_lhs_backup;
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
  test_all_simd_abi<CheckSimdCassign>();
  return 0;
}
