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
// template <class T, class U, class Abi> see below ex::static_simd_cast<(const
// ex::simd<U, Abi>&);

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

#define GNERATOR_FULL_TYPE(TYPE)                                                                                       \
  if constexpr (!std::is_same_v<_Tp, TYPE>) {                                                                          \
    ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });                                                       \
    auto after_cast = ex::static_simd_cast<TYPE>(origin_simd);                                                         \
    static_assert(std::is_same_v< ex::simd<TYPE, ex::simd_abi::fixed_size<ex::simd<_Tp, SimdAbi>::size()>>,            \
                                  decltype(after_cast)>);                                                              \
    std::array<_Tp, origin_simd.size()> expected_value;                                                                \
    for (size_t i = 0; i < expected_value.size(); ++i)                                                                 \
      expected_value[i] = origin_simd[i];                                                                              \
    assert_simd_value_correct(after_cast, expected_value);                                                             \
  }

// 2.2. test for the `std::make_signed_t<U>` same as `std::make_signed_t<T>`
#define GNERATOR_FULL_TYPE_SAME_SIGNED(TYPE)                                                                           \
                                                                                                                       \
  if constexpr (!std::is_same_v<SimdAbi, ex::simd_abi::scalar> && std::is_integral_v<_Tp> &&                           \
                std::is_integral_v<TYPE>) {                                                                            \
    if constexpr (std::is_same_v<std::make_signed_t<_Tp>, std::make_signed_t<TYPE>>) {                                 \
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });                                                     \
      auto after_cast = ex::static_simd_cast<TYPE>(origin_simd);                                                       \
      static_assert(std::is_same_v< ex::simd<TYPE, ex::simd_abi::fixed_size<ex::simd<_Tp, SimdAbi>::size()>>,          \
                                    decltype(after_cast)>);                                                            \
      std::array<_Tp, origin_simd.size()> expected_value;                                                              \
      for (size_t i = 0; i < expected_value.size(); ++i)                                                               \
        expected_value[i] = origin_simd[i];                                                                            \
      assert_simd_value_correct(after_cast, expected_value);                                                           \
    }                                                                                                                  \
  }

#define SHORT_TYPE(F)                                                                                                  \
  F(long double)                                                                                                       \
  F(double)                                                                                                            \
  F(float)                                                                                                             \
  F(long long)                                                                                                         \
  F(unsigned long long)                                                                                                \
  F(long)                                                                                                              \
  F(unsigned long)                                                                                                     \
  F(int) F(unsigned int) F(short) F(unsigned short) F(wchar_t) F(signed char) F(unsigned char) F(char32_t) F(char16_t)

// `std::make_signed_t` require `no float`, otherwise fail on `preprocessor stage` see also: https://cppinsights.io/s/aed94d59
#define SHORT_TYPE_NO_FLOAT(F)                                                                                         \
  F(long long)                                                                                                         \
  F(unsigned long long)                                                                                                \
  F(long)                                                                                                              \
  F(unsigned long)                                                                                                     \
  F(int) F(unsigned int) F(short) F(unsigned short) F(wchar_t) F(signed char) F(unsigned char) F(char32_t) F(char16_t)

struct CheckStaticSimdCast {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    constexpr size_t array_size = ex::simd_size_v<_Tp, SimdAbi>;
    SHORT_TYPE(GNERATOR_FULL_TYPE)
    SHORT_TYPE_NO_FLOAT(GNERATOR_FULL_TYPE_SAME_SIGNED)
    // 2.1. test for the `std::is_same_v<U, T>`
    {
      auto after_cast = ex::static_simd_cast<_Tp>(ex::simd<_Tp, SimdAbi>([](_Tp i) { return i; }));
      static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
      std::array<_Tp, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = static_cast<_Tp>(i);
      assert_simd_value_correct(after_cast, expected_value);
    }

    constexpr std::size_t origin_full_abi_simd_size = ex::simd<_Tp, SimdAbi>::size();
    static auto InitializeSimd                      = [](auto& origin_simd) {
      const size_t simd_size = origin_simd.size();
      for (size_t i = 0; i < simd_size; ++i)
        origin_simd[i] = static_cast<_Tp>(i + 1);
    };
    // 1. test for the `ex::is_simd_v<T>`
    if constexpr (origin_full_abi_simd_size == ex::native_simd<_Tp>::size()) {
      {
        auto origin = ex::simd<_Tp, SimdAbi>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::native_simd<_Tp>>(origin);

        static_assert(std::is_same_v<ex::native_simd<_Tp>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }

      {
        auto origin = ex::native_simd<_Tp>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, SimdAbi>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }
    }

    if constexpr (origin_full_abi_simd_size == ex::fixed_size_simd<_Tp, _Np>::size()) {
      {
        auto origin = ex::simd<_Tp, SimdAbi>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::fixed_size_simd<_Tp, _Np>>(origin);

        static_assert(std::is_same_v<ex::fixed_size_simd<_Tp, _Np>, decltype(after_cast)>);
        std::array<_Tp, origin.size()> expected_value;
        for (size_t i = 0; i < origin.size(); ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }

      {
        auto origin = ex::fixed_size_simd<_Tp, _Np>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, SimdAbi>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }
    }

    if constexpr (origin_full_abi_simd_size == ex::simd<_Tp, ex::simd_abi::scalar>::size()) {
      {
        auto origin = ex::simd<_Tp, SimdAbi>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, ex::simd_abi::scalar>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, ex::simd_abi::scalar>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }

      {
        auto origin = ex::simd<_Tp, ex::simd_abi::scalar>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, SimdAbi>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }
    }

    if constexpr (origin_full_abi_simd_size == ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, _Np + 1>>::size()) {
      {
        auto origin = ex::simd<_Tp, SimdAbi>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, _Np + 1>>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, _Np + 1>>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }

      {
        auto origin = ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, _Np + 1>>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, SimdAbi>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }
    }

    if constexpr (origin_full_abi_simd_size == ex::simd<_Tp, ex::simd_abi::compatible<_Tp>>::size()) {
      {
        auto origin = ex::simd<_Tp, SimdAbi>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, ex::simd_abi::compatible<_Tp>>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, ex::simd_abi::compatible<_Tp>>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
      }

      {
        auto origin = ex::simd<_Tp, ex::simd_abi::compatible<_Tp>>();
        InitializeSimd(origin);
        auto after_cast = ex::static_simd_cast<ex::simd<_Tp, SimdAbi>>(origin);

        static_assert(std::is_same_v<ex::simd<_Tp, SimdAbi>, decltype(after_cast)>);
        std::array<_Tp, array_size> expected_value;
        for (size_t i = 0; i < array_size; ++i)
          expected_value[i] = static_cast<_Tp>(i + 1);
        assert_simd_value_correct(after_cast, expected_value);
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
  test_all_simd_abi<CheckStaticSimdCast>();
  return 0;
}
