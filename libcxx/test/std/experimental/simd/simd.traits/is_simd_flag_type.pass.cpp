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
// [simd.traits]
// template <class T> struct is_simd_flag_type;
// template <class T> inline constexpr bool ex::is_simd_flag_type_v = ex::is_simd_flag_type<T>::value;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

class EmptyEntry {};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  static_assert(ex::is_simd_flag_type<ex::element_aligned_tag>::value);
  static_assert(ex::is_simd_flag_type<ex::vector_aligned_tag>::value);
  static_assert(ex::is_simd_flag_type<ex::overaligned_tag<_Np>>::value);

  static_assert(!ex::is_simd_flag_type<_Tp>::value);
  static_assert(!ex::is_simd_flag_type<ex::simd<_Tp, SimdAbi>>::value);
  static_assert(!ex::is_simd_flag_type<ex::simd_mask<_Tp, SimdAbi>>::value);

  static_assert(ex::is_simd_flag_type_v<ex::element_aligned_tag>);
  static_assert(ex::is_simd_flag_type_v<ex::vector_aligned_tag>);
  static_assert(ex::is_simd_flag_type_v<ex::overaligned_tag<_Np>>);

  static_assert(!ex::is_simd_flag_type_v<_Tp>);
  static_assert(!ex::is_simd_flag_type_v<ex::simd<_Tp, SimdAbi>>);
  static_assert(!ex::is_simd_flag_type_v<ex::simd_mask<_Tp, SimdAbi>>);

  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<EmptyEntry>();
  return 0;
}
