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
// [simd.class]
// template<class U> simd(U&& value) noexcept;
// template<class U> simd(const simd<U, simd_abi::fixed_size<size()>>&) noexcept;
// template<class G> explicit simd(G&& gen) noexcept;
// template<class U, class Flags> simd(const U* mem, Flags f);

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

template <class T, class SimdAbi, std::size_t array_size>
struct BroadCastHelper {
  const std::array<T, array_size>& expected_value;

  BroadCastHelper(const std::array<T, array_size>& value) : expected_value(value) {}

  template <class U>
  void operator()() const {
    if constexpr (is_non_narrowing_convertible_v<U, T>) {
      ex::simd<T, SimdAbi> simd_broadcast_from_vectorizable_type(static_cast<U>(3));
      assert_simd_value_correct<array_size>(simd_broadcast_from_vectorizable_type, expected_value);
    }
  }
};

struct CheckSimdBroadcastCtorFromVectorizedType {
  template <class T,class SimdAbi, std::size_t>
  void operator()() {
    constexpr std::size_t array_size = ex::simd_size_v<T, SimdAbi>;
    std::array<T, array_size> expected_value;
    std::fill(expected_value.begin(), expected_value.end(), 3);

    types::for_each(arithmetic_no_bool_types(), BroadCastHelper<T, SimdAbi, array_size>(expected_value));
  }
};

template <class T>
class implicit_type {
  T val;

public:
  implicit_type(T v) : val(v) {}
  operator T() const { return val; }
};

struct CheckSimdBroadcastCtor {
  template <class T,class SimdAbi, std::size_t>
  void operator()() {
    constexpr std::size_t array_size = ex::simd_size_v<T, SimdAbi>;
    std::array<T, array_size> expected_value;
    std::fill(expected_value.begin(), expected_value.end(), 3);

    implicit_type<T> implicit_convert_to_3(3);
    ex::simd<T, SimdAbi> simd_broadcast_from_implicit_type(std::move(implicit_convert_to_3));
    assert_simd_value_correct<array_size>(simd_broadcast_from_implicit_type, expected_value);

    ex::simd<T, SimdAbi> simd_broadcast_from_int(3);
    assert_simd_value_correct<array_size>(simd_broadcast_from_int, expected_value);

    if constexpr (std::is_unsigned_v<T>) {
      ex::simd<T, SimdAbi> simd_broadcast_from_uint(3u);
      assert_simd_value_correct<array_size>(simd_broadcast_from_uint, expected_value);
    }
  }
};

template <class T, class SimdAbi, std::size_t array_size>
struct ConversionHelper {
  const std::array<T, array_size>& expected_value;

  ConversionHelper(const std::array<T, array_size>& value) : expected_value(value) {}

  template <class U>
  void operator()() const {
    if constexpr (!std::is_same_v<U, T> && std::is_same_v<SimdAbi, ex::simd_abi::fixed_size<array_size>> &&
                  is_non_narrowing_convertible_v<U, T>) {
      ex::simd<U, SimdAbi> origin_simd([](U i) { return i; });
      ex::simd<T, SimdAbi> simd_from_implicit_conversion(origin_simd);
      assert_simd_value_correct<array_size>(simd_from_implicit_conversion, expected_value);
    }
  }
};

struct CheckConversionSimdCtor {
  template <class T,class SimdAbi, std::size_t>
  void operator()() {
    constexpr std::size_t array_size = ex::simd_size_v<T, SimdAbi>;
    std::array<T, array_size> expected_value;
    for (size_t i = 0; i < array_size; ++i)
      expected_value[i] = static_cast<T>(i);

    types::for_each(arithmetic_no_bool_types(), ConversionHelper<T, SimdAbi, array_size>(expected_value));
  }
};

struct CheckGenerateSimdCtor {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
    constexpr size_t array_size = origin_simd.size();
    std::array<_Tp, array_size> expected_value;
    for (size_t i = 0; i < array_size; ++i)
      expected_value[i] = static_cast<_Tp>(i);
    assert_simd_value_correct(origin_simd, expected_value);
  }
};

struct CheckLoadSimdCtor {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    {
      constexpr auto alignas_size = ex::memory_alignment_v<ex::simd<_Tp, SimdAbi>, _Tp>;

      constexpr auto array_length = ex::simd_size_v<_Tp, SimdAbi>;

      alignas(alignas_size) _Tp buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<_Tp>(i + 1);

      ex::simd<_Tp, SimdAbi> origin_simd(buffer, ex::vector_aligned_tag());
      assert_simd_value_correct(origin_simd, buffer);
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(_Np));

      constexpr auto array_length = ex::simd_size_v<_Tp, SimdAbi>;

      alignas(alignas_size) _Tp buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<_Tp>(i);

      ex::simd<_Tp, SimdAbi> origin_simd(buffer, ex::overaligned_tag<alignas_size>());
      assert_simd_value_correct(origin_simd, buffer);
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(alignof(_Tp)));

      constexpr auto array_length = ex::simd_size_v<_Tp, SimdAbi>;

      alignas(alignas_size) _Tp buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<_Tp>(i);

      ex::simd<_Tp, SimdAbi> origin_simd(buffer, ex::element_aligned_tag());
      assert_simd_value_correct(origin_simd, buffer);
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
  test_all_simd_abi<CheckSimdBroadcastCtorFromVectorizedType>();
  test_all_simd_abi<CheckSimdBroadcastCtor>();
  test_all_simd_abi<CheckConversionSimdCtor>();
  test_all_simd_abi<CheckGenerateSimdCtor>();
  test_all_simd_abi<CheckLoadSimdCtor>();
  return 0;
}
