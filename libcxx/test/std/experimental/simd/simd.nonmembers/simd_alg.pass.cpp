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
// template<class T, class Abi> simd<T, Abi> min(const simd<T, Abi>& a, const simd<T, Abi>& b) noexcept;
// template<class T, class Abi> simd<T, Abi> max(const simd<T, Abi>& a, const simd<T, Abi>& b) noexcept;
// template<class T, class Abi> pair<simd<T, Abi>, simd<T, Abi>> minmax(const simd<T, Abi>& a, const simd<T, Abi>& b) noexcept;
// template<class T, class Abi> simd<T, Abi> clamp(const simd<T, Abi>& v, const simd<T, Abi>& lo, const simd<T, Abi>& hi);

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMin {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> lhs([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> rhs(static_cast<_Tp>(5));

    auto result_min_simd = ex::min(lhs, rhs);
    static_assert(std::is_same_v<decltype(result_min_simd), ex::simd<_Tp, SimdAbi>>);

    std::array<_Tp, lhs.size()> expected_values;

    for (size_t i = 0; i < lhs.size(); ++i)
      expected_values[i] = std::min(lhs[i], rhs[i]);

    assert_simd_value_correct(result_min_simd, expected_values);
  }
};

struct CheckSimdMax {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> lhs([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> rhs(static_cast<_Tp>(5));

    auto result_max_simd = ex::max(lhs, rhs);
    static_assert(std::is_same_v<decltype(result_max_simd), ex::simd<_Tp, SimdAbi>>);

    std::array<_Tp, lhs.size()> expected_values;

    for (size_t i = 0; i < lhs.size(); ++i)
      expected_values[i] = std::max(lhs[i], rhs[i]);

    assert_simd_value_correct(result_max_simd, expected_values);
  }
};

struct CheckSimdMinMax {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> lhs([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> rhs(static_cast<_Tp>(5));

    auto result_min_max_simd = ex::minmax(lhs, rhs);
    static_assert(
        std::is_same_v<decltype(result_min_max_simd), std::pair<ex::simd<_Tp, SimdAbi>, ex::simd<_Tp, SimdAbi>>>);

    std::array<_Tp, lhs.size()> expected_min_values;

    for (size_t i = 0; i < lhs.size(); ++i)
      expected_min_values[i] = std::min(lhs[i], rhs[i]);

    assert_simd_value_correct(result_min_max_simd.first, expected_min_values);

    std::array<_Tp, lhs.size()> expected_max_values;

    for (size_t i = 0; i < lhs.size(); ++i)
      expected_max_values[i] = std::max(lhs[i], rhs[i]);

    assert_simd_value_correct(result_min_max_simd.second, expected_max_values);
  }
};

struct CheckSimdClamp {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> lo([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> hi([](_Tp i) { return static_cast<_Tp>(i + 1); });
    const ex::simd<_Tp, SimdAbi> v(static_cast<_Tp>(5));

    auto result_simd = ex::clamp(v, lo, hi);
    static_assert(std::is_same_v<decltype(result_simd), ex::simd<_Tp, SimdAbi>>);

    std::array<_Tp, lo.size()> expected_values;

    for (size_t i = 0; i < lo.size(); ++i)
      expected_values[i] = std::clamp(v[i], lo[i], hi[i]);

    assert_simd_value_correct(result_simd, expected_values);
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
  test_all_simd_abi<CheckSimdMin>();
  test_all_simd_abi<CheckSimdMax>();
  test_all_simd_abi<CheckSimdMinMax>();
  test_all_simd_abi<CheckSimdClamp>();
  return 0;
}
