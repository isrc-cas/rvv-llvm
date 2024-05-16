//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <cassert>
#include <experimental/simd>
#include "type_algorithms.h"

namespace ex = std::experimental::parallelism_v2;

template <class T>
struct is_simd_reference : std::false_type {};

template <class _Tp, class _Storage, class _ValueType>
struct is_simd_reference<ex::__simd_reference<_Tp, _Storage, _ValueType>> : std::true_type {};

template <class SimdAbi, class _Tp>
void assert_simd_value_correct(const ex::simd<_Tp, SimdAbi>& origin_simd, _Tp* expected_value) {
  for (size_t i = 0; i < origin_simd.size(); ++i)
    assert(origin_simd[i] == expected_value[i]);
}

template <class SimdAbi, class _Tp>
void assert_simd_mask_value_correct(const ex::simd_mask<_Tp, SimdAbi>& origin_mask, bool* expected_value) {
  for (size_t i = 0; i < origin_mask.size(); ++i)
    assert(origin_mask[i] == expected_value[i]);
}

template <std::size_t ArraySize, class SimdAbi, class _Tp>
void assert_simd_value_correct(const ex::simd<_Tp, SimdAbi>& origin_simd,
                               const std::array<_Tp, ArraySize>& expected_value) {
  for (size_t i = 0; i < origin_simd.size(); ++i)
    assert(origin_simd[i] == expected_value[i]);
}

template <std::size_t ArraySize, class SimdAbi, class _Tp>
void assert_simd_mask_value_correct(const ex::simd_mask<_Tp, SimdAbi>& origin_mask,
                                    const std::array<_Tp, ArraySize>& expected_value) {
  for (size_t i = 0; i < origin_mask.size(); ++i)
    assert(origin_mask[i] == expected_value[i]);
}

template <std::size_t ArraySize, class ArrayType, class SimdAbi, class _Tp>
void assert_simd_value_correct(const ex::simd<_Tp, SimdAbi>& origin_simd,
                               const std::array<ArrayType, ArraySize>& expected_value) {
  for (size_t i = 0; i < origin_simd.size(); ++i)
    assert(origin_simd[i] == static_cast<_Tp>(expected_value[i]));
}

template <std::size_t ArraySize, class ArrayType, class SimdAbi, class _Tp>
void assert_simd_mask_value_correct(const ex::simd_mask<_Tp, SimdAbi>& origin_mask,
                                    const std::array<ArrayType, ArraySize>& expected_value) {
  for (size_t i = 0; i < origin_mask.size(); ++i)
    assert(origin_mask[i] == static_cast<_Tp>(expected_value[i]));
}

template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi();

template <class F, class _Tp>
void test_native_compatible_scalar() {
  test_simd_abi<F, 1, _Tp, ex::simd_abi::scalar, ex::simd_abi::native<_Tp>, ex::simd_abi::compatible<_Tp>>();
}

template <class F, class _Tp, std::size_t... _Np>
void test_fixed_size_deduce_t(std::integer_sequence<std::size_t, _Np...>) {
  (test_simd_abi<F, _Np + 1, _Tp, ex::simd_abi::fixed_size<_Np + 1>, ex::simd_abi::deduce_t<_Tp, _Np + 1>>(), ...);
}

constexpr std::size_t max_simd_size = 32;

template <class F, class _Tp>
void test_all_simd_abi() {
  test_native_compatible_scalar<F, _Tp>();

  constexpr static auto integer_seq_from_make_integer = std::make_integer_sequence<size_t, max_simd_size - 1>();

  test_fixed_size_deduce_t<F, _Tp>(integer_seq_from_make_integer);
}

using integral_no_bool_types =
    types::concatenate_t<types::character_types, types::signed_integer_types, types::unsigned_integer_types>;
using arithmetic_no_bool_types = types::concatenate_t<integral_no_bool_types, types::floating_point_types>;

template <class F>
struct TestAllSimdAbiFunctor {
  template <class _Tp>
  void operator()() {
    test_all_simd_abi<F, _Tp>();
  }
};

template <class F>
void test_all_simd_abi() {
  types::for_each(arithmetic_no_bool_types(), TestAllSimdAbiFunctor<F>());
}

// credit to: https://stackoverflow.com/a/466242
constexpr size_t next_pow2(size_t v) noexcept {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

template <class From, class To, class = void>
inline constexpr bool is_non_narrowing_convertible_v = false;

template <class From, class To>
inline constexpr bool is_non_narrowing_convertible_v<From, To, std::void_t<decltype(To{std::declval<From>()})>> = true;

#endif // TEST_UTIL_H