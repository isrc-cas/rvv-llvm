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
// class where_expression : public const_where_expression<M, T> {
// public:
//   where_expression(const where_expression&) = delete;
//   where_expression& operator=(const where_expression&) = delete;
//   template <class U> void operator=(U&& x);
//   template <class U> void operator+=(U&& x);
//   template <class U> void operator-=(U&& x);
//   template <class U> void operator*=(U&& x);
//   template <class U> void operator/=(U&& x);
//   template <class U> void operator%=(U&& x);
//   template <class U> void operator&=(U&& x);
//   template <class U> void operator|=(U&& x);
//   template <class U> void operator^=(U&& x);
//   template <class U> void operator<<=(U&& x);
//   template <class U> void operator>>=(U&& x);
//   void operator++();
//   void operator++(int);
//   void operator--();
//   void operator--(int);
//   template <class U, class Flags> void copy_from(const U* mem, Flags);
// };

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckWhereExprOperators {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> interval_mask_for_where;
    ex::simd<_Tp, SimdAbi> vec([](_Tp i) { return i; });

    constexpr size_t array_size = vec.size();

    std::array<bool, array_size> mask_val;
    std::array<_Tp, array_size> vec_val{};

    for (size_t i = 0; i < array_size; ++i) {
      if (i % 2) {
        vec_val[i] = static_cast<_Tp>(i);
      }
      interval_mask_for_where[i] = i % 2;
      mask_val[i]                = i % 2;
    }

#define INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(x, y)                                                          \
  for (size_t i = 0; i < array_size; ++i) {                                                                            \
    if (i % 2) {                                                                                                       \
      expected_val[i] = static_cast<_Tp>(x);                                                                           \
    } else {                                                                                                           \
      expected_val[i] = static_cast<_Tp>(y);                                                                           \
    }                                                                                                                  \
  }

    // operator =
    {
      ex::simd<_Tp, SimdAbi> origin(0);
      ex::where(interval_mask_for_where, origin) = vec;
      assert_simd_value_correct(origin, vec_val);

      ex::simd_mask<_Tp, SimdAbi> origin_mask(false);
      ex::where(interval_mask_for_where, origin_mask) = interval_mask_for_where;
      assert_simd_mask_value_correct(origin_mask, mask_val);
    }

    // operator +=
    {
      ex::simd<_Tp, SimdAbi> origin(1);
      ex::where(interval_mask_for_where, origin) += vec;

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(i + 1, 1);

      assert_simd_value_correct(origin, expected_val);

      ex::where(interval_mask_for_where, origin) += static_cast<_Tp>(2);

      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(i + 3, 1);

      assert_simd_value_correct(origin, expected_val);
    }

    // operator -=
    {
      ex::simd<_Tp, SimdAbi> origin(20);
      ex::where(interval_mask_for_where, origin) -= static_cast<_Tp>(3);

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(17, 20);

      assert_simd_value_correct(origin, expected_val);

      ex::where(interval_mask_for_where, origin) -= ex::simd<_Tp, SimdAbi>(1);
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(16, 20);

      assert_simd_value_correct(origin, expected_val);
    }

    // operator *=
    {
      // 1, 2, 1, 2
      ex::simd<_Tp, SimdAbi> origin([](_Tp i) {
        if (static_cast<int>(i) % 2)
          return static_cast<_Tp>(2);
        else
          return static_cast<_Tp>(1);
      });
      // 1, 2 * 2, 1, 2 * 2
      ex::where(interval_mask_for_where, origin) *= static_cast<_Tp>(2);

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(4, 1);

      assert_simd_value_correct(origin, expected_val);
      // 1, 2 * 2 * 2, 1, 2 * 2 * 2
      ex::where(interval_mask_for_where, origin) *= ex::simd<_Tp, SimdAbi>(2);
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(8, 1);

      assert_simd_value_correct(origin, expected_val);
    }

    // operator /=
    {
      // 1 * 4, 2 * 4, 3 * 4, 4 * 4
      ex::simd<_Tp, SimdAbi> origin([](_Tp i) { return static_cast<_Tp>(i * 4); });
      // 4, 2 * 4 / 2, 3 * 4, 4 * 4 / 2
      ex::where(interval_mask_for_where, origin) /= static_cast<_Tp>(2);

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(i * 2, i * 4);

      assert_simd_value_correct(origin, expected_val);
      // 4, 2 * 4 / 2 / 2, 3 * 4, 4 * 4 / 2 / 2
      ex::where(interval_mask_for_where, origin) /= ex::simd<_Tp, SimdAbi>(2);
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(i, i * 4);

      assert_simd_value_correct(origin, expected_val);
    }

    // operator %=
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        // 1, 2, 1, 2
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) { return static_cast<_Tp>(i % 2 + 1); });
        // 1, 2 % 2, 1, 2 % 2
        ex::where(interval_mask_for_where, origin) %= static_cast<_Tp>(2);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(0, 1);

        assert_simd_value_correct(origin, expected_val);

        // 1 + 1, 0 + 1, 1 + 1, 0 + 1
        origin += static_cast<_Tp>(1);

        // 2, 1 % 2, 2, 1 % 2
        ex::where(interval_mask_for_where, origin) %= ex::simd<_Tp, SimdAbi>(2);
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(1, 2);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator &=
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        // 1, 2, 1, 2
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) { return static_cast<_Tp>(i % 2 + 1); });
        // 1, 2 && 2, 1, 2 && 2
        ex::where(interval_mask_for_where, origin) &= static_cast<_Tp>(2);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(2, 1);

        assert_simd_value_correct(origin, expected_val);

        // 1 + 2, 2 + 2, 1 + 2, 2 + 2
        origin += static_cast<_Tp>(2);
        // 1 + 2, 4 && 2, 1 + 2, 4 && 2
        ex::where(interval_mask_for_where, origin) &= ex::simd<_Tp, SimdAbi>(2);
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(0, 3);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator |=
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        // 1, 2, 1, 2
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) { return static_cast<_Tp>(i % 2 + 1); });

        // 1, 2 || 2, 1, 2 || 2
        ex::where(interval_mask_for_where, origin) |= static_cast<_Tp>(2);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(2, 1);

        assert_simd_value_correct(origin, expected_val);
        // 1 + 2, 2 + 2, 1 + 2, 2 + 2
        origin += static_cast<_Tp>(2);

        // 1 + 2, 4 || 2, 1 + 2, 4 || 2
        ex::where(interval_mask_for_where, origin) |= ex::simd<_Tp, SimdAbi>(2);

        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(6, 3);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator ^=
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) { return static_cast<_Tp>(i % 2 + 1); });
        ex::where(interval_mask_for_where, origin) ^= static_cast<_Tp>(2);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(0, 1);

        assert_simd_value_correct(origin, expected_val);

        origin += static_cast<_Tp>(2);
        ex::where(interval_mask_for_where, origin) ^= ex::simd<_Tp, SimdAbi>(2);
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(0, 3);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator <<
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) {
          if (i % 2)
            return static_cast<_Tp>(1);
          else
            return static_cast<_Tp>(4);
        });
        ex::where(interval_mask_for_where, origin) <<= static_cast<_Tp>(1);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(2, 4);

        assert_simd_value_correct(origin, expected_val);

        ex::where(interval_mask_for_where, origin) <<= ex::simd<_Tp, SimdAbi>(1);
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(4, 4);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator >>
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        ex::simd<_Tp, SimdAbi> origin([](_Tp i) {
          if (i % 2)
            return static_cast<_Tp>(4);
          else
            return static_cast<_Tp>(1);
        });
        ex::where(interval_mask_for_where, origin) >>= static_cast<_Tp>(1);

        std::array<_Tp, array_size> expected_val;
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(2, 1);

        assert_simd_value_correct(origin, expected_val);

        ex::where(interval_mask_for_where, origin) >>= ex::simd<_Tp, SimdAbi>(1);
        INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(1, 1);

        assert_simd_value_correct(origin, expected_val);
      }
    }

    // operator ++
    {
      ex::simd<_Tp, SimdAbi> origin([](_Tp i) {
        if (static_cast<int>(i) % 2)
          return static_cast<_Tp>(2);
        else
          return static_cast<_Tp>(1);
      });
      ex::where(interval_mask_for_where, origin)++;

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(3, 1);

      assert_simd_value_correct(origin, expected_val);

      ++ex::where(interval_mask_for_where, origin);
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(4, 1);

      assert_simd_value_correct(origin, expected_val);
    }

    // operator --
    {
      ex::simd<_Tp, SimdAbi> origin([](_Tp i) {
        if (static_cast<int>(i) % 2)
          return static_cast<_Tp>(3);
        else
          return static_cast<_Tp>(2);
      });

      ex::where(interval_mask_for_where, origin)--;

      std::array<_Tp, array_size> expected_val;
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(2, 2);

      assert_simd_value_correct(origin, expected_val);

      --ex::where(interval_mask_for_where, origin);
      INITIALIZE_EXPECTED_VAL_WITH_INTERVAL_CONDITION(1, 2);

      assert_simd_value_correct(origin, expected_val);
    }
  }
};

struct CheckWhereExprCopyFrom {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> interval_mask_for_where;
    for (size_t i = 0; i < interval_mask_for_where.size(); ++i)
      interval_mask_for_where[i] = i % 2;

    {
      ex::simd<_Tp, SimdAbi> origin_simd(static_cast<_Tp>(-1));
      constexpr size_t simd_size = origin_simd.size();
      alignas(alignof(_Tp)) _Tp buffer[simd_size * 2];
      for (size_t i = 0; i < simd_size * 2; ++i)
        buffer[i] = static_cast<_Tp>(i);

      ex::where(interval_mask_for_where, origin_simd).copy_from(&buffer[1], ex::element_aligned_tag());

      assert(origin_simd[0] == static_cast<_Tp>(-1));
      for (size_t i = 1; i < simd_size; ++i) {
        if ((i - 1) % 2 == 1)
          assert(origin_simd[i] == static_cast<_Tp>(-1));
        else
          assert(origin_simd[i] == static_cast<_Tp>(i + 1));
      }
    }

    {
      ex::simd<_Tp, SimdAbi> origin_simd(static_cast<_Tp>(-1));
      constexpr size_t simd_size = origin_simd.size();
      alignas(alignof(_Tp)) _Tp buffer[simd_size * 2];
      for (size_t i = 0; i < simd_size * 2; ++i)
        buffer[i] = static_cast<_Tp>(i);

      ex::where(interval_mask_for_where, origin_simd).copy_from(&buffer[2], ex::vector_aligned_tag());

      assert(origin_simd[0] == static_cast<_Tp>(-1));
      if constexpr (simd_size >= 2)
        assert(origin_simd[1] == static_cast<_Tp>(3));

      for (size_t i = 2; i < simd_size; ++i) {
        if ((i - 2) % 2 == 0)
          assert(origin_simd[i] == static_cast<_Tp>(-1));
        else
          assert(origin_simd[i] == static_cast<_Tp>(i + 2));
      }
    }

    {
      ex::simd<_Tp, SimdAbi> origin_simd(static_cast<_Tp>(-1));
      constexpr size_t simd_size = origin_simd.size();
      alignas(alignof(_Tp)) _Tp buffer[simd_size * 2];
      for (size_t i = 0; i < simd_size * 2; ++i)
        buffer[i] = static_cast<_Tp>(i);

      ex::where(interval_mask_for_where, origin_simd).copy_from(buffer, ex::element_aligned_tag());

      for (size_t i = 0; i < simd_size; ++i) {
        if (i % 2 == 0)
          assert(origin_simd[i] == static_cast<_Tp>(-1));
        else
          assert(origin_simd[i] == static_cast<_Tp>(i));
      }
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(false);
      constexpr size_t mask_size = origin_mask.size();

      alignas(alignof(bool)) bool buffer[mask_size * 2];
      for (size_t i = 0; i < mask_size * 2; ++i)
        buffer[i] = i % 2;

      ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_mask).copy_from(&buffer[1], ex::vector_aligned_tag());

      assert(origin_mask[0] == true);
      for (size_t i = 1; i < mask_size; ++i)
        assert(!origin_mask[i] == i % 2);
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(false);
      constexpr size_t mask_size = origin_mask.size();

      alignas(alignof(bool)) bool buffer[mask_size * 2];
      for (size_t i = 0; i < mask_size * 2; ++i)
        buffer[i] = i % 2;

      ex::where(ex::simd_mask<_Tp, SimdAbi>(true), origin_mask).copy_from(buffer, ex::element_aligned_tag());

      for (size_t i = 0; i < mask_size; ++i)
        assert(origin_mask[i] == i % 2);
    }

    {
      ex::simd_mask<_Tp, SimdAbi> origin_mask(true);
      constexpr size_t mask_size = origin_mask.size();

      alignas(ex::memory_alignment_v<ex::simd_mask<_Tp, SimdAbi>, bool>) bool buffer[mask_size * 2]{};

      ex::where(interval_mask_for_where, origin_mask).copy_to(buffer, ex::vector_aligned_tag());

      for (size_t i = 0; i < mask_size; ++i)
        assert(origin_mask[i] == true);
    }
  }
};

struct CheckWhereBaseType {
  template <class _Tp, class>
  void operator()() {
    {
      _Tp val              = 3;
      ex::where(true, val) = val + 1;
      assert(val == static_cast<_Tp>(4));
      ex::where(false, val) = val + 1;
      assert(val == static_cast<_Tp>(4));
      ex::where(true, val) += 1;
      assert(val == static_cast<_Tp>(5));
      ex::where(true, val) -= 1;
      assert(val == static_cast<_Tp>(4));
      val++;
      ex::where(true, val) *= 2;
      assert(val == static_cast<_Tp>(10));
      ex::where(true, val) /= 2;
      assert(val == static_cast<_Tp>(5));
      ex::where(true, val)--;
      assert(val == static_cast<_Tp>(4));
      --ex::where(true, val);
      assert(val == static_cast<_Tp>(3));
      ++ex::where(true, val);
      assert(val == static_cast<_Tp>(4));
      ex::where(true, val)++;
      assert(val == static_cast<_Tp>(5));

      if constexpr (!std::is_floating_point_v<_Tp>) {
        ex::where(true, val) %= 2;
        assert(val == static_cast<_Tp>(1));
        ex::where(true, val) <<= 2;
        assert(val == static_cast<_Tp>(4));
        ex::where(true, val) |= 2;
        assert(val == static_cast<_Tp>(6));
        ex::where(true, val) &= 2;
        assert(val == static_cast<_Tp>(2));
        ex::where(true, val) >>= 1;
        assert(val == static_cast<_Tp>(1));
      }

      {
        constexpr _Tp rhs = static_cast<_Tp>(1);
        _Tp lhs           = 3;
        ex::where(true, lhs).copy_from(&rhs, ex::vector_aligned_tag());
        assert(lhs == 1);
      }

      {
        constexpr _Tp rhs = static_cast<_Tp>(1);
        _Tp lhs           = 3;
        ex::where(true, lhs).copy_from(&rhs, ex::element_aligned_tag());
        assert(lhs == 1);
      }

      {
        constexpr _Tp rhs = static_cast<_Tp>(1);
        _Tp lhs           = 3;
        ex::where(true, lhs).copy_from(&rhs, ex::overaligned_tag<alignof(_Tp)>());
        assert(lhs == 1);
      }
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
  test_all_simd_abi<CheckWhereExprOperators>();
  test_all_simd_abi<CheckWhereExprCopyFrom>();
  test_all_simd_abi<CheckWhereBaseType>();
  return 0;
}
