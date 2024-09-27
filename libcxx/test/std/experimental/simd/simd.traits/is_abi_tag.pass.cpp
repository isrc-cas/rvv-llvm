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
// template <class T> struct is_abi_tag;
// template <class T> inline constexpr bool ex::is_abi_tag_v = ex::is_abi_tag<T>::value;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

class EmptyEntry {};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  static_assert(ex::is_abi_tag<SimdAbi>::value);

  static_assert(!ex::is_abi_tag<_Tp>::value);
  static_assert(!ex::is_abi_tag<ex::simd< _Tp, SimdAbi>>::value);
  static_assert(!ex::is_abi_tag<ex::simd_mask< _Tp, SimdAbi>>::value);

  static_assert(ex::is_abi_tag_v<SimdAbi>);

  static_assert(!ex::is_abi_tag_v<_Tp>);
  static_assert(!ex::is_abi_tag_v<ex::simd< _Tp, SimdAbi>>);
  static_assert(!ex::is_abi_tag_v<ex::simd_mask< _Tp, SimdAbi>>);

  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<EmptyEntry>();
  return 0;
}
