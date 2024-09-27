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
// // [simd.whereexpr]
// template <class M, class T>
// class const_where_expression {
//   const M& mask; // exposition only
//   T& data; // exposition only
// public:
//   const_where_expression(const const_where_expression&) = delete;
//   const_where_expression& operator=(const const_where_expression&) = delete;
//   T operator-() const && noexcept;
//   T operator+() const && noexcept;
//   T operator~() const && noexcept;
//   template<class U, class Flags> void copy_to(U* mem, Flags f) const &&;
// };

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckConstWhereExprOperatorMinus {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    // -where(mask, mask)
    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask;

      constexpr size_t simd_size = origin_mask.size();
      std::array<bool, simd_size> expected_value;

      for (size_t i = 0; i < origin_mask.size(); ++i) {
        origin_mask[i]    = static_cast<bool>(i % 2);
        expected_value[i] = i % 2;
      }

      auto where_result = -ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_mask);

      assert_simd_mask_value_correct<simd_size>(where_result, expected_value);
    }

    // -where(mask, simd)
    if constexpr (ex::simd_size_v<_Tp, SimdAbi> >= 2) {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      auto where_result = -ex::where(origin_simd < static_cast<_Tp>(2), origin_simd);

      constexpr size_t simd_size = origin_simd.size();
      std::array<_Tp, simd_size> expected_value;
      for (size_t i = 0; i < simd_size; ++i)
        expected_value[i] = static_cast<_Tp>(i);

      expected_value[1] = static_cast<_Tp>(-1);

      assert_simd_value_correct<simd_size>(where_result, expected_value);
    }
  }
};

struct CheckConstWhereExprOperatorPositive {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    // +where(mask, mask)
    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask;

      constexpr size_t simd_size = origin_mask.size();
      std::array<bool, simd_size> expected_value;

      for (size_t i = 0; i < origin_mask.size(); ++i) {
        origin_mask[i]    = static_cast<bool>(i % 2);
        expected_value[i] = i % 2;
      }

      auto where_result = +ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_mask);

      assert_simd_mask_value_correct<simd_size>(where_result, expected_value);
    }

    // +where(mask, simd)
    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      auto where_result = +ex::where(origin_simd < static_cast<_Tp>(2), origin_simd);

      constexpr size_t simd_size = origin_simd.size();
      std::array<_Tp, simd_size> expected_value;
      for (size_t i = 0; i < simd_size; ++i)
        expected_value[i] = static_cast<_Tp>(i);

      assert_simd_value_correct<simd_size>(where_result, expected_value);
    }
  }
};

struct CheckConstWhereExprOperatorNegation {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    // ~where(mask, mask)
    if constexpr (!std::is_floating_point_v<_Tp>) {
      ex::simd_mask<_Tp, SimdAbi> origin_mask;

      constexpr int simd_size = origin_mask.size();
      std::array<bool, simd_size> expected_value;

      for (int i = 0; i < simd_size; ++i) {
        origin_mask[i]    = static_cast<bool>(i % 2);
        expected_value[i] = !(static_cast<bool>(i % 2));
      }

      auto where_result = ~ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_mask);

      assert_simd_mask_value_correct<simd_size>(where_result, expected_value);
    }

    // ~where(mask, simd)
    if constexpr (!std::is_floating_point_v<_Tp>) {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      auto where_result = ~ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_simd);

      constexpr int simd_size = origin_simd.size();
      std::array<_Tp, simd_size> expected_value;

      for (int i = 0; i < simd_size; ++i)
        expected_value[i] = static_cast<_Tp>(-i - 1);

      assert_simd_value_correct<simd_size>(where_result, expected_value);
    }
  }
};

struct CheckConstWhereExprCopyTo {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> interval_mask_for_where;
    for (size_t i = 0; i < interval_mask_for_where.size(); ++i)
      interval_mask_for_where[i] = i % 2;

    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      constexpr size_t simd_size = origin_simd.size();
      alignas(alignof(_Tp)) _Tp arr[simd_size * 2]{};
      for (size_t i = 0; i < simd_size * 2; ++i)
        arr[i] = static_cast<_Tp>(-1);

      ex::where(interval_mask_for_where, origin_simd).copy_to(&arr[1], ex::element_aligned_tag());

      assert(arr[0] == static_cast<_Tp>(-1));
      for (size_t i = 1; i < simd_size + 1 && i < simd_size * 2; ++i) {
        if (i % 2 == 1)
          assert(arr[i] == static_cast<_Tp>(-1));
        else
          assert(arr[i] == static_cast<_Tp>(i - 1));
      }
      for (size_t i = simd_size + 1; i < simd_size * 2; ++i)
        assert(arr[i] == static_cast<_Tp>(-1));
    }

    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      constexpr size_t simd_size = origin_simd.size();
      alignas(ex::memory_alignment_v<ex::simd<_Tp, SimdAbi>, _Tp>) _Tp arr[simd_size * 2]{};
      for (size_t i = 0; i < simd_size * 2; ++i)
        arr[i] = static_cast<_Tp>(-1);

      ex::where(interval_mask_for_where, origin_simd).copy_to(&arr[2], ex::vector_aligned_tag());

      assert(arr[0] == static_cast<_Tp>(-1));
      assert(arr[1] == static_cast<_Tp>(-1));
      for (size_t i = 2; i < simd_size + 2 && i < simd_size * 2; ++i) {
        if (i % 2 == 0)
          assert(arr[i] == static_cast<_Tp>(-1));
        else
          assert(arr[i] == static_cast<_Tp>(i - 2));
      }
      for (size_t i = simd_size + 2; i < simd_size * 2; ++i)
        assert(arr[i] == static_cast<_Tp>(-1));
    }

    {
      const ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return i; });
      constexpr size_t simd_size = origin_simd.size();
      alignas(alignof(_Tp)) _Tp arr[simd_size * 2]{};
      for (size_t i = 0; i < simd_size * 2; ++i)
        arr[i] = static_cast<_Tp>(-1);

      ex::where(interval_mask_for_where, origin_simd).copy_to(&arr[simd_size], ex::overaligned_tag<alignof(_Tp)>());

      for (size_t i = 0; i < simd_size; ++i)
        assert(arr[i] == static_cast<_Tp>(-1));
      for (size_t i = simd_size; i < simd_size * 2; ++i) {
        if ((i - simd_size) % 2 == 0)
          assert(arr[i] == static_cast<_Tp>(-1));
        else
          assert(arr[i] == static_cast<_Tp>(i - simd_size));
      }
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(true);
      constexpr size_t mask_size = origin_mask.size();

      alignas(alignof(bool)) bool buffer[mask_size * 2]{};

      ex::where(interval_mask_for_where, origin_mask).copy_to(&buffer[1], ex::vector_aligned_tag());

      assert(buffer[0] == false);
      for (size_t i = 1; i < mask_size + 1; ++i)
        assert(!buffer[i] == i % 2);
      for (size_t i = 1 + mask_size; i < mask_size * 2; ++i)
        assert(buffer[i] == false);
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(true);
      constexpr size_t mask_size = origin_mask.size();

      alignas(ex::memory_alignment_v<ex::simd_mask<_Tp, SimdAbi>, bool>) bool buffer[mask_size * 2]{};

      ex::where(interval_mask_for_where, origin_mask).copy_to(&buffer[2], ex::vector_aligned_tag());

      assert(buffer[0] == false);
      assert(buffer[1] == false);
      for (size_t i = 2; i < mask_size + 2 && i < mask_size * 2; ++i)
        assert(buffer[i] == i % 2);
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(true);
      constexpr size_t mask_size = origin_mask.size();

      alignas(alignof(bool)) bool buffer[mask_size * 2]{};

      ex::where(interval_mask_for_where, origin_mask).copy_to(&buffer[mask_size], ex::overaligned_tag<alignof(bool)>());

      for (size_t i = 0; i < mask_size; ++i)
        assert(buffer[i] == false);
      for (size_t i = mask_size; i < mask_size * 2; ++i) {
        if ((i - mask_size) % 2 == 0)
          assert(buffer[i] == false);
        else
          assert(buffer[i] == true);
      }
    }
  }
};

struct CheckConstWhereBaseType {
  template <class _Tp, class, std::size_t _Np>
  void operator()() {
    {
      _Tp val     = 3;
      auto result = -ex::where(true, val);
      static_assert(std::is_same_v<_Tp, decltype(result)>);
      assert(val == (3));
      assert(result == static_cast<_Tp>(-3));
    }

    {
      _Tp val     = 3;
      auto result = +ex::where(true, val);
      static_assert(std::is_same_v<_Tp, decltype(result)>);
      assert(val == (3));
      assert(result == static_cast<_Tp>(+3));
    }

    if constexpr (!std::is_floating_point_v<_Tp>) {
      _Tp val     = 3;
      auto result = ~ex::where(true, val);
      static_assert(std::is_same_v<_Tp, decltype(result)>);
      assert(val == (3));
      assert(result == static_cast<_Tp>(~3));
    }

    {
      _Tp val = 1;

      ex::where(true, static_cast<_Tp>(3)).copy_to(&val, ex::vector_aligned_tag());
      assert(val == static_cast<_Tp>(3));

      ex::where(true, static_cast<_Tp>(4)).copy_to(&val, ex::overaligned_tag<_Np>());
      assert(val == static_cast<_Tp>(4));

      ex::where(true, static_cast<_Tp>(6)).copy_to(&val, ex::element_aligned_tag());
      assert(val == static_cast<_Tp>(6));
    }

    {
      _Tp buffer[_Np + 3]{};

      ex::where(true, static_cast<_Tp>(3)).copy_to(&buffer[2], ex::vector_aligned_tag());
      for (size_t i = 0; i < _Np + 3; ++i) {
        if (i == 2)
          assert(buffer[2] == static_cast<_Tp>(3));
        else
          assert(buffer[i] == static_cast<_Tp>(0));
      }

      ex::where(true, static_cast<_Tp>(6)).copy_to(&buffer[1], ex::element_aligned_tag());
      for (size_t i = 0; i < _Np + 3; ++i) {
        if (i == 2)
          assert(buffer[2] == static_cast<_Tp>(3));
        else if (i == 1)
          assert(buffer[1] == static_cast<_Tp>(6));
        else
          assert(buffer[i] == static_cast<_Tp>(0));
      }

      ex::where(true, static_cast<_Tp>(7)).copy_to(&buffer[0], ex::overaligned_tag<_Np>());
      for (size_t i = 0; i < _Np + 3; ++i) {
        if (i == 0)
          assert(buffer[0] == static_cast<_Tp>(7));
        else if (i == 2)
          assert(buffer[2] == static_cast<_Tp>(3));
        else if (i == 1)
          assert(buffer[1] == static_cast<_Tp>(6));
        else
          assert(buffer[i] == static_cast<_Tp>(0));
      }
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
  test_all_simd_abi<CheckConstWhereExprOperatorMinus>();
  test_all_simd_abi<CheckConstWhereExprOperatorPositive>();
  test_all_simd_abi<CheckConstWhereExprOperatorNegation>();
  test_all_simd_abi<CheckConstWhereExprCopyTo>();
  test_all_simd_abi<CheckConstWhereBaseType>();
  return 0;
}
