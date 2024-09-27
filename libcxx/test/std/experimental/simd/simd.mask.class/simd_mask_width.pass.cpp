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
// [simd.mask.class]
// static constexpr size_t size() noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;
struct CheckSimdMaskWidth {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd_mask<_Tp, SimdAbi> origin_simd_mask{};
    static_assert(origin_simd_mask.size() == ex::simd_size_v<_Tp, SimdAbi>);
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
  test_all_simd_abi<CheckSimdMaskWidth>();
  return 0;
}
