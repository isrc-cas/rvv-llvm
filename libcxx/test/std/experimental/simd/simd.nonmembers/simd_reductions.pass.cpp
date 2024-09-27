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
// template <class T, class Abi, class BinaryOperation = plus<>>
//   T reduce(const simd<T, Abi>&,
//            BinaryOperation = {});

// template <class M, class V, class BinaryOperation>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 typename V::value_type neutral_element,
//                                 BinaryOperation binary_op);

// template <class M, class V>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 plus<> binary_op = {}) noexcept;
// template <class M, class V>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 multiplies<> binary_op) noexcept;
// template <class M, class V>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 bit_and<> binary_op) noexcept;
// template <class M, class V>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 bit_or<> binary_op) noexcept;
// template <class M, class V>
//   typename V::value_type reduce(const const_where_expression<M, V>& x,
//                                 bit_xor<> binary_op) noexcept;

// template <class T, class Abi>
//   T hmin(const simd<T, Abi>&) noexcept;
// template <class M, class V>
//   typename V::value_type hmin(const const_where_expression<M, V>&) noexcept;
// template <class T, class Abi>
//   T hmax(const simd<T, Abi>&) noexcept;
// template <class M, class V>
//   typename V::value_type hmax(const const_where_expression<M, V>&) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdReductionPart1 {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> simd_zero_start([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> simd_one_start([](_Tp i) { return static_cast<_Tp>(i + 1); });
    {
      static const auto reduce_test_plus = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(simd_, std::plus<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        _Tp expected{simd_[0]};
        for (size_t i = 1; i < simd_.size(); ++i)
          expected += simd_[i];
        assert(reduce_result == expected);
      };
      reduce_test_plus(simd_zero_start);
      reduce_test_plus(simd_one_start);
    }
    {
      static const auto reduce_test_multiplies = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(simd_, std::multiplies<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        _Tp expected{simd_[0]};
        for (size_t i = 1; i < simd_.size(); ++i)
          expected *= simd_[i];
        assert(reduce_result == expected);
      };
      reduce_test_multiplies(simd_zero_start);
      reduce_test_multiplies(simd_one_start);
    }
    {
      static const auto reduce_test_bit_and = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp reduce_result = ex::reduce(simd_, std::bit_and<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        _Tp expected{simd_[0]};
        for (size_t i = 1; i < simd_.size(); ++i)
          expected &= simd_[i];
        assert(reduce_result == expected);
      };
      reduce_test_bit_and(simd_zero_start);
      reduce_test_bit_and(simd_one_start);
    }
    {
      static const auto reduce_test_bit_or = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp reduce_result = ex::reduce(simd_, std::bit_or<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        _Tp expected{simd_[0]};
        for (size_t i = 1; i < simd_.size(); ++i)
          expected |= simd_[i];
        assert(reduce_result == expected);
      };
      reduce_test_bit_or(simd_zero_start);
      reduce_test_bit_or(simd_one_start);
    }
    {
      static const auto reduce_test_bit_xor = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp reduce_result = ex::reduce(simd_, std::bit_xor<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        _Tp expected{simd_[0]};
        for (size_t i = 1; i < simd_.size(); ++i)
          expected ^= simd_[i];
        assert(reduce_result == expected);
      };
      reduce_test_bit_xor(simd_zero_start);
      reduce_test_bit_xor(simd_one_start);
    }
  }
};

struct CheckSimdReductionPart2 {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> simd_zero_start([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> simd_one_start([](_Tp i) { return static_cast<_Tp>(i + 1); });
    {
      static const auto reduce_test_plus = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp identity_element{};
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), identity_element, std::plus<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(identity_element == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::plus<>());
          _Tp expected{};
          for (size_t i = 0; i < simd_.size(); ++i)
            expected += simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_plus(simd_zero_start);
      reduce_test_plus(simd_one_start);
    }

    {
      static const auto reduce_test_multiplies = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp identity_element{};
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), identity_element, std::multiplies<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(identity_element == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::multiplies<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected *= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_multiplies(simd_zero_start);
      reduce_test_multiplies(simd_one_start);
    }

    {
      static const auto reduce_test_bit_and = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp identity_element{};
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), identity_element, std::bit_and<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(identity_element == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_and<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected &= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_and(simd_zero_start);
      reduce_test_bit_and(simd_one_start);
    }

    {
      static const auto reduce_test_bit_or = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp identity_element{};
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), identity_element, std::bit_or<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(identity_element == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_or<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected |= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_or(simd_zero_start);
      reduce_test_bit_or(simd_one_start);
    }

    {
      static const auto reduce_test_bit_xor = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        _Tp identity_element{};
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), identity_element, std::bit_xor<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(identity_element == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_xor<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected ^= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_xor(simd_zero_start);
      reduce_test_bit_xor(simd_one_start);
    }
  }
};

struct CheckSimdReductionPart3 {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> simd_zero_start([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> simd_one_start([](_Tp i) { return static_cast<_Tp>(i + 1); });

    {
      static const auto reduce_test_plus = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), std::plus<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(0 == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::plus<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected += simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_plus(simd_zero_start);
      reduce_test_plus(simd_one_start);
    }
    {
      static const auto reduce_test_multiplies = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), std::multiplies<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(1 == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::multiplies<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected *= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_multiplies(simd_zero_start);
      reduce_test_multiplies(simd_one_start);
    }
    if constexpr (std::is_signed_v<_Tp>) {
      static const auto reduce_test_bit_and = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), std::bit_and<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(~(_Tp()) == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_and<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected &= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_and(simd_zero_start);
      reduce_test_bit_and(simd_one_start);
    }
    if constexpr (std::is_signed_v<_Tp>) {
      static const auto reduce_test_bit_or = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), std::bit_or<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(0 == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_or<>());
          _Tp expected{simd_[0]};
          for (size_t i = 1; i < simd_.size(); ++i)
            expected |= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_or(simd_zero_start);
      reduce_test_bit_or(simd_one_start);
    }
    if constexpr (std::is_signed_v<_Tp>) {
      static const auto reduce_test_bit_xor = [](const ex::simd<_Tp, SimdAbi>& simd_) {
        auto reduce_result = ex::reduce(ex::where(simd_ > 0, simd_), std::bit_xor<>());
        static_assert(std::is_same_v<decltype(reduce_result), _Tp>);
        if (ex::none_of(simd_ > 0)) {
          assert(0 == reduce_result);
        } else {
          _Tp result = ex::reduce(simd_, std::bit_xor<>());
          _Tp expected{0};
          for (size_t i = 0; i < simd_.size(); ++i)
            expected ^= simd_[i];
          assert(result == expected);
        }
      };
      reduce_test_bit_xor(simd_zero_start);
      reduce_test_bit_xor(simd_one_start);
    }
  }
};

struct CheckSimdReductionPart4 {
  template <class _Tp, class SimdAbi>
  void operator()() {
    const ex::simd<_Tp, SimdAbi> simd_zero_start([](_Tp i) { return i; });
    const ex::simd<_Tp, SimdAbi> simd_one_start([](_Tp i) { return static_cast<_Tp>(i + 1); });
    {
      static const auto reduce_hmin_test = [](const ex::simd<_Tp, SimdAbi>& simd_, bool is_start_zero) {
        auto hmin_result = ex::hmin(simd_);
        static_assert(std::is_same_v<_Tp, decltype(hmin_result)>);
        if (is_start_zero)
          assert(hmin_result == static_cast<_Tp>(0));
        else
          assert(hmin_result == static_cast<_Tp>(1));

        const ex::simd_mask<_Tp, SimdAbi> mask_(simd_ > 0);
        auto where_expr = ex::where(mask_, simd_);
        auto result     = ex::hmin(where_expr);
        static_assert(std::is_same_v<_Tp, decltype(result)>);

        if (ex::none_of(mask_)) {
          assert(std::numeric_limits<_Tp>::max() == result);
        } else {
          _Tp expected(std::numeric_limits<_Tp>::max());
          for (size_t i = 0; i < simd_.size(); ++i) {
            if (mask_[i] == true) {
              if (expected == std::numeric_limits<_Tp>::max())
                expected = simd_[i];
              else
                expected = std::min(expected, simd_[i]);
            }
          }
          assert(result == expected);
        }
      };
      reduce_hmin_test(simd_zero_start, true);
      reduce_hmin_test(simd_one_start, false);
    }
    {
      static const auto reduce_hmax_test = [](const ex::simd<_Tp, SimdAbi>& simd_, bool is_start_zero) {
        auto hmax_result = ex::hmax(simd_);
        static_assert(std::is_same_v<_Tp, decltype(hmax_result)>);
        if (is_start_zero)
          assert(hmax_result == static_cast<_Tp>(simd_.size() - 1));
        else
          assert(hmax_result == static_cast<_Tp>(simd_.size()));

        const ex::simd_mask<_Tp, SimdAbi> mask_(simd_ > 0);
        auto where_expr = ex::where(mask_, simd_);
        _Tp result      = ex::hmax(where_expr);

        if (ex::none_of(mask_)) {
          assert(std::numeric_limits<_Tp>::lowest() == result);
        } else {
          _Tp expected{0};
          for (size_t i = 0; i < simd_.size(); ++i) {
            if (mask_[i] == true) {
              if (expected == 0)
                expected = simd_[i];
              else
                expected = std::max(expected, simd_[i]);
            }
          }
          assert(result == expected);
        }
      };
      reduce_hmax_test(simd_zero_start, true);
      reduce_hmax_test(simd_one_start, false);
    }
  }
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  if constexpr (!std::is_floating_point_v<_Tp>) {
    F{}.template operator()<_Tp, SimdAbi>();
    test_simd_abi<F, _Np, _Tp, SimdAbis...>();
  }
}

int main(int, char**) {
  test_all_simd_abi<CheckSimdReductionPart1>();
  test_all_simd_abi<CheckSimdReductionPart2>();
  test_all_simd_abi<CheckSimdReductionPart3>();
  test_all_simd_abi<CheckSimdReductionPart4>();
  return 0;
}
