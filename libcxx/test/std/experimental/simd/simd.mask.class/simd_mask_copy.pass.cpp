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
// template<class Flags> void copy_from(const value_type* mem, Flags);
// template<class Flags> void copy_to(value_type* mem, Flags);

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdCopyFrom {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    {
      constexpr auto alignas_size = ex::memory_alignment_v<ex::simd_mask<_Tp, SimdAbi>>;
      ex::simd_mask<_Tp, SimdAbi> origin_simd_mask{};

      constexpr auto array_length = origin_simd_mask.size();

      alignas(alignas_size) bool buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<bool>(i + 1);

      origin_simd_mask.copy_from(buffer, ex::vector_aligned_tag());

      assert_simd_mask_value_correct(origin_simd_mask, buffer);
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(_Np));
      ex::simd_mask<_Tp, SimdAbi> origin_simd_mask{};
      constexpr auto array_length = origin_simd_mask.size();

      alignas(alignas_size) bool buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<bool>(i + 1);

      origin_simd_mask.copy_from(buffer, ex::overaligned_tag<alignas_size>());

      assert_simd_mask_value_correct(origin_simd_mask, buffer);
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(alignof(_Tp)));
      ex::simd_mask<_Tp, SimdAbi> origin_simd_mask{};
      constexpr auto array_length = origin_simd_mask.size();

      alignas(alignas_size) bool buffer[array_length];
      for (size_t i = 0; i < array_length; i++)
        buffer[i] = static_cast<bool>(i + 1);

      origin_simd_mask.copy_from(buffer, ex::element_aligned_tag());

      assert_simd_mask_value_correct(origin_simd_mask, buffer);
    }
  }
};

struct CheckSimdCopyTo {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    {
      constexpr auto alignas_size = ex::memory_alignment_v<ex::simd_mask<_Tp, SimdAbi>>;

      const ex::simd_mask<_Tp, SimdAbi> origin_simd_mask([](bool i) { return static_cast<bool>(i + 1); });
      constexpr auto array_length = origin_simd_mask.size();
      alignas(alignas_size) bool expected_buffer[array_length]{};

      origin_simd_mask.copy_to(expected_buffer, ex::vector_aligned_tag());

      for (size_t i = 0; i < origin_simd_mask.size(); i++)
        assert(expected_buffer[i] == static_cast<bool>(i + 1));
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(_Np));
      const ex::simd_mask<_Tp, SimdAbi> origin_simd_mask([](bool i) { return static_cast<bool>(i + 1); });

      constexpr auto array_length = origin_simd_mask.size();

      alignas(alignas_size) bool expected_buffer[array_length]{};

      origin_simd_mask.copy_to(expected_buffer, ex::overaligned_tag<alignas_size>());

      for (size_t i = 0; i < array_length; i++)
        assert(expected_buffer[i] == static_cast<bool>(i + 1));
    }

    {
      constexpr auto alignas_size = std::max(next_pow2(sizeof(_Tp)), next_pow2(alignof(_Tp)));
      const ex::simd_mask<_Tp, SimdAbi> origin_simd_mask([](bool i) { return static_cast<bool>(i + 1); });

      constexpr auto array_length = origin_simd_mask.size();

      alignas(alignas_size) bool expected_buffer[array_length]{};

      origin_simd_mask.copy_to(expected_buffer, ex::element_aligned_tag());

      for (size_t i = 0; i < array_length; i++)
        assert(expected_buffer[i] == static_cast<bool>(i + 1));
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
  test_all_simd_abi<CheckSimdCopyFrom>();
  test_all_simd_abi<CheckSimdCopyTo>();
  return 0;
}
