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
// [simd.mask.nonmembers]
// template <class T, class Abi>
// where_expression<simd_mask<T, Abi>, simd<T, Abi>> where(const typename simd<T, Abi>::mask_type& k,
//                                                         simd<T, Abi>& v) noexcept;
// template <class T, class Abi>
// const_where_expression<simd_mask<T, Abi>, simd<T, Abi>> where(const typename simd<T, Abi>::mask_type& k,
//                                                               const simd<T, Abi>& v) noexcept;
// template <class T, class Abi>
// where_expression<simd_mask<T, Abi>, simd_mask<T, Abi>> where(const type_identity_t<simd_mask<T, Abi>>& k,
//                                                              simd_mask<T, Abi>& v) noexcept;
// template <class T, class Abi>
// const_where_expression<simd_mask<T, Abi>, simd_mask<T, Abi>> where(const type_identity_t<simd_mask<T, Abi>>& k,
//                                                                    const simd_mask<T, Abi>& v) noexcept;
// template <class T>
// where_expression<bool T> where(see below k, T& v) noexcept;
// template <class T>
// const_where_expression<bool, T> where(see below k, const T& v) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdMaskWhere {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const typename ex::simd<_Tp, SimdAbi>::mask_type mask_{};
    ex::simd<_Tp, SimdAbi> simd_([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> const_simd_([](_Tp i) { return i; });
    const ex::simd_mask<_Tp, SimdAbi> const_mask_(simd_ == const_simd_);
    constexpr size_t array_size = simd_.size();
    {
      auto pure_simd = +ex::where(mask_, simd_);
      static_assert(std::is_same_v<decltype(pure_simd), ex::simd<_Tp, SimdAbi> >);

      std::array<_Tp, array_size> expected_values;
      for (size_t i = 0; i < array_size; ++i)
        expected_values[i] = static_cast<_Tp>(i);

      assert_simd_value_correct(pure_simd, expected_values);
    }
    {
      auto const_simd = +ex::where(mask_, const_simd_);
      static_assert(std::is_same_v<decltype(const_simd), ex::simd<_Tp, SimdAbi>>);

      std::array<_Tp, array_size> expected_values;
      for (size_t i = 0; i < array_size; ++i)
        expected_values[i] = static_cast<_Tp>(i);

      assert_simd_value_correct(const_simd, expected_values);
    }
    {
      // Using the '__type_identity_t' internal keyword in test is unsightly, but necessary for compatibility with C++17.
      auto pure_mask = +ex::where(std::__type_identity_t<ex::simd_mask<_Tp, SimdAbi>>(mask_), mask_);

      static_assert(std::is_same_v<ex::simd_mask<_Tp, SimdAbi>, decltype(pure_mask)>);
      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = mask_[i];

      assert_simd_mask_value_correct(pure_mask, expected_value);
    }
    {
      // Using the '__type_identity_t' internal keyword in test is unsightly, but necessary for compatibility with C++17.
      auto const_mask = +ex::where(std::__type_identity_t<ex::simd_mask<_Tp, SimdAbi>>(mask_), const_mask_);

      static_assert(std::is_same_v<ex::simd_mask<_Tp, SimdAbi>, decltype(const_mask)>);
      std::array<bool, array_size> expected_value;
      for (size_t i = 0; i < array_size; ++i)
        expected_value[i] = const_mask_[i];

      assert_simd_mask_value_correct(const_mask, expected_value);
    }
    {
      _Tp val{1};
      auto data = +ex::where<_Tp>(true, val);

      static_assert(std::is_same_v<_Tp, decltype(data)>);
      assert(data == val);
    }
    {
      const _Tp val{1};
      auto data = +ex::where<_Tp>(true, val);

      static_assert(std::is_same_v<_Tp, decltype(data)>);
      assert(data == val);
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
  test_all_simd_abi<CheckSimdMaskWhere>();
  return 0;
}
