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
// template <class T, class... Abis>
// simd<T, simd_abi::deduce_t<T, (simd_size_v<T, Abis> + ...)>> concat(const simd<T, Abis>&...) noexcept;
// template <class T, class... Abis>
// simd_mask<T, simd_abi::deduce_t<T, (simd_size_v<T, Abis> + ...)>> concat(const simd_mask<T, Abis>&...) noexcept;
// template <class T, class Abi, size_t N>
// resize_simd<simd_size_v<T, Abi> * N, simd<T, Abi>> concat(const array<simd<T, Abi>, N>& arr) noexcept;
// template <class T, class Abi, size_t N>
// resize_simd<simd_size_v<T, Abi> * N, simd_mask<T, Abi>> concat(const array<simd_mask<T, Abi>, N>& arr) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckConcatSimd {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    constexpr size_t concat_length =
        ex::simd_size_v<_Tp, ex::simd_abi::native<_Tp>> + ex::simd_size_v<_Tp, ex::simd_abi::fixed_size<_Np>> +
        ex::simd_size_v<_Tp, SimdAbi>;
    if constexpr (concat_length <= max_simd_size) {
      ex::native_simd<_Tp> native_simd_([](_Tp i) { return i; });
      ex::fixed_size_simd<_Tp, _Np> fixed_size_simd_([](_Tp i) { return static_cast<_Tp>(_Np - i); });

      ex::simd<_Tp, SimdAbi> full_type_simd_([](_Tp i) { return i; });

      using concat_result_type = ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, concat_length>>;

      auto concat_result = ex::concat<_Tp, ex::simd_abi::native<_Tp>, ex::simd_abi::fixed_size<_Np>, SimdAbi>(
          native_simd_, fixed_size_simd_, full_type_simd_);
      static_assert(ex::is_simd_v<decltype(concat_result)>);
      static_assert(std::is_same_v<decltype(concat_result), concat_result_type>);

      size_t k = 0;
      for (size_t i = 0; i < native_simd_.size(); ++i, ++k)
        assert(concat_result[k] == native_simd_[i]);

      for (size_t i = 0; i < fixed_size_simd_.size(); ++i, ++k)
        assert(concat_result[k] == fixed_size_simd_[i]);

      for (size_t i = 0; i < full_type_simd_.size(); ++i, ++k)
        assert(concat_result[k] == full_type_simd_[i]);
    }
  }
};

struct CheckConcatSimdMask {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    constexpr size_t concat_length =
        ex::simd_size_v<_Tp, ex::simd_abi::native<_Tp>> + ex::simd_size_v<_Tp, ex::simd_abi::fixed_size<_Np>> +
        ex::simd_size_v<_Tp, SimdAbi>;
    if constexpr (concat_length <= max_simd_size) {
      ex::native_simd_mask<_Tp> native_simd_mask_([](_Tp i) { return i; });
      ex::fixed_size_simd_mask<_Tp, _Np> fixed_size_simd_mask_([](_Tp i) { return static_cast<_Tp>(_Np - i); });

      ex::simd_mask<_Tp, SimdAbi> full_type_simd_mask_([](_Tp i) { return i; });
      using concat_result_type = ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, concat_length>>;
      auto concat_result       = ex::concat<_Tp, ex::simd_abi::native<_Tp>, ex::simd_abi::fixed_size<_Np>, SimdAbi>(
          native_simd_mask_, fixed_size_simd_mask_, full_type_simd_mask_);
      static_assert(ex::is_simd_mask_v<decltype(concat_result)>);
      static_assert(std::is_same_v<decltype(concat_result), concat_result_type>);

      size_t k = 0;
      for (size_t i = 0; i < native_simd_mask_.size(); ++i, ++k)
        assert(concat_result[k] == native_simd_mask_[i]);

      for (size_t i = 0; i < fixed_size_simd_mask_.size(); ++i, ++k)
        assert(concat_result[k] == fixed_size_simd_mask_[i]);

      for (size_t i = 0; i < full_type_simd_mask_.size(); ++i, ++k)
        assert(concat_result[k] == full_type_simd_mask_[i]);
    }
  }
};

struct CheckConcatResizeSimd {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    if constexpr (ex::simd_size_v<_Tp, SimdAbi> * _Np <= max_simd_size) {
      std::array<ex::simd<_Tp, SimdAbi>, _Np> arr;
      for (auto& sub_simd : arr) {
        for (size_t i = 0; i < sub_simd.size(); ++i) {
          sub_simd[i] = static_cast<_Tp>(i);
        }
      }

      using concat_result_type = ex::resize_simd_t<ex::simd_size_v<_Tp, SimdAbi> * _Np, ex::simd<_Tp, SimdAbi>>;
      auto concat_result       = ex::concat<_Tp, SimdAbi, _Np>(arr);

      static_assert(ex::is_simd_v<decltype(concat_result)>);
      static_assert(std::is_same_v<decltype(concat_result), concat_result_type>);

      constexpr size_t prev_length = ex::simd_size_v<_Tp, SimdAbi>;

      for (size_t i = 0; i < concat_result.size(); ++i)
        assert(concat_result[i] == arr[i / prev_length][i % prev_length]);
    }
  }
};

struct CheckConcatResizeSimdMask {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    if constexpr (ex::simd_size_v<_Tp, SimdAbi> * _Np <= max_simd_size) {
      std::array<ex::simd_mask<_Tp, SimdAbi>, _Np> arr;
      for (auto& sub_simd_mask : arr) {
        for (size_t i = 0; i < sub_simd_mask.size(); ++i) {
          sub_simd_mask[i] = static_cast<bool>(i);
        }
      }

      using concat_result_type = ex::resize_simd_t<ex::simd_size_v<_Tp, SimdAbi> * _Np, ex::simd_mask<_Tp, SimdAbi>>;
      auto concat_result       = ex::concat<_Tp, SimdAbi, _Np>(arr);

      static_assert(ex::is_simd_mask_v<decltype(concat_result)>);
      static_assert(std::is_same_v<decltype(concat_result), concat_result_type>);

      constexpr size_t prev_length = ex::simd_size_v<_Tp, SimdAbi>;

      for (size_t i = 0; i < concat_result.size(); ++i)
        assert(concat_result[i] == arr[i / prev_length][i % prev_length]);
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
  test_all_simd_abi<CheckConcatSimd>();
  test_all_simd_abi<CheckConcatSimdMask>();
  test_all_simd_abi<CheckConcatResizeSimd>();
  test_all_simd_abi<CheckConcatResizeSimdMask>();
  return 0;
}
