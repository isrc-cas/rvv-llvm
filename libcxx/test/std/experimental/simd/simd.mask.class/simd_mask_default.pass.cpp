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
// simd_mask()                                = default;
// simd_mask(const simd_mask&)                = default;
// simd_mask& operator=(const simd_mask&)     = default;
// simd_mask(simd_mask&&) noexcept            = default;
// simd_mask& operator=(simd_mask&&) noexcept = default;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

// See https://www.open-std.org/jtc1/sc22/WG21/docs/papers/2019/n4808.pdf
// Default intialization performs no initialization of the elements; value-initialization initializes each element with T().
// [ Note: Thus, default initialization leaves the elements in an indeterminate state. — end note ]
struct CheckSimdMaskDefaultCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> pure_mask;
    // trash value in default ctor
    static_assert(pure_mask.size() > 0);
  }
};
struct CheckSimdMaskCopyCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> pure_mask;
    for (size_t i = 0; i < pure_mask.size(); ++i) {
      if (i % 2 == 0)
        pure_mask[i] = true;
      else
        pure_mask[i] = false;
    }

    ex::simd_mask<_Tp, SimdAbi> from_copy_ctor(pure_mask);
    std::array<bool, pure_mask.size()> expected_value;

    for (size_t i = 0; i < pure_mask.size(); ++i)
      expected_value[i] = pure_mask[i];

    assert_simd_mask_value_correct(from_copy_ctor, expected_value);
  }
};
struct CheckSimdMaskMoveCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> pure_mask;
    for (size_t i = 0; i < pure_mask.size(); ++i) {
      if (i % 2 == 0)
        pure_mask[i] = true;
      else
        pure_mask[i] = false;
    }

    ex::simd_mask<_Tp, SimdAbi> from_move_ctor(std::move(pure_mask));
    std::array<bool, pure_mask.size()> expected_value;

    for (size_t i = 0; i < pure_mask.size(); ++i)
      expected_value[i] = pure_mask[i];

    assert_simd_mask_value_correct(from_move_ctor, expected_value);
  }
};
struct CheckSimdMaskCopyAssignment {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> pure_mask;
    for (size_t i = 0; i < pure_mask.size(); ++i) {
      if (i % 2 == 0)
        pure_mask[i] = true;
      else
        pure_mask[i] = false;
    }

    ex::simd_mask<_Tp, SimdAbi> from_copy_assignment;
    from_copy_assignment = pure_mask;
    std::array<bool, pure_mask.size()> expected_value;

    for (size_t i = 0; i < pure_mask.size(); ++i)
      expected_value[i] = pure_mask[i];

    assert_simd_mask_value_correct(from_copy_assignment, expected_value);
  }
};
struct CheckSimdMaskMoveAssignment {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd_mask<_Tp, SimdAbi> pure_mask;
    for (size_t i = 0; i < pure_mask.size(); ++i) {
      if (i % 2 == 0)
        pure_mask[i] = true;
      else
        pure_mask[i] = false;
    }

    ex::simd_mask<_Tp, SimdAbi> from_move_assignment;
    from_move_assignment = std::move(pure_mask);
    std::array<bool, pure_mask.size()> expected_value;

    for (size_t i = 0; i < pure_mask.size(); ++i)
      expected_value[i] = pure_mask[i];

    assert_simd_mask_value_correct(from_move_assignment, expected_value);
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
  test_all_simd_abi<CheckSimdMaskCopyCtor>();
  test_all_simd_abi<CheckSimdMaskMoveCtor>();
  test_all_simd_abi<CheckSimdMaskCopyAssignment>();
  test_all_simd_abi<CheckSimdMaskMoveAssignment>();
  return 0;
}
