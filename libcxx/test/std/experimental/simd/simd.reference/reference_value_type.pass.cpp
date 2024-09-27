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
// [simd.reference]
// operator value_type() const noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;
struct CheckReferenceValueType {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 2); });

    // cannot use `auto` for variable `r` here, the type will be evalued as `ex::simd_reference`
    _Tp r = origin_simd[0];

    assert(r == 2);
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
  test_all_simd_abi<CheckReferenceValueType>();
  return 0;
}
