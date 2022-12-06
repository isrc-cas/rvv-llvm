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

template <class F, class _Tp, std::size_t... _Np>
void test_all_simd_abi(std::integer_sequence<std::size_t, _Np...>) {
  using namespace ex;

  (test_simd_abi<F,
                 _Np + 1,
                 _Tp,
                 simd_abi::native<_Tp>,
                 simd_abi::fixed_size<_Np + 1>,
                 simd_abi::scalar,
                 simd_abi::compatible<_Tp>,
                 simd_abi::deduce_t<_Tp, _Np + 1>>(),
   ...);
}

template <class F>
void test_all_simd_abi() {
  [[maybe_unused]] constexpr auto integer_seq_from_directly_build =
      std::integer_sequence<std::size_t, 1, 2, 3, 4, 8, 9, 12, 16, 24, 29>();
  [[maybe_unused]] constexpr auto integer_seq_from_make_integer = std::make_integer_sequence<size_t, 31>();
  test_all_simd_abi<F, long double>(integer_seq_from_make_integer);
  test_all_simd_abi<F, double>(integer_seq_from_make_integer);
  test_all_simd_abi<F, float>(integer_seq_from_make_integer);
  test_all_simd_abi<F, long long>(integer_seq_from_make_integer);
  test_all_simd_abi<F, unsigned long long>(integer_seq_from_make_integer);
  test_all_simd_abi<F, long>(integer_seq_from_make_integer);
  test_all_simd_abi<F, unsigned long>(integer_seq_from_make_integer);
  test_all_simd_abi<F, int>(integer_seq_from_make_integer);
  test_all_simd_abi<F, unsigned int>(integer_seq_from_make_integer);
  test_all_simd_abi<F, short>(integer_seq_from_make_integer);
  test_all_simd_abi<F, unsigned short>(integer_seq_from_make_integer);
  test_all_simd_abi<F, wchar_t>(integer_seq_from_make_integer);
  test_all_simd_abi<F, signed char>(integer_seq_from_make_integer);
  test_all_simd_abi<F, unsigned char>(integer_seq_from_make_integer);
  test_all_simd_abi<F, char32_t>(integer_seq_from_make_integer);
  test_all_simd_abi<F, char16_t>(integer_seq_from_make_integer);
}
constexpr std::size_t max_simd_size = 32;

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
#endif // TEST_UTIL_H