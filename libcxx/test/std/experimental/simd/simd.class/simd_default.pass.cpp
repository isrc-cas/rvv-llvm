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
// simd()                           = default;
// simd(const simd&)                = default;
// simd& operator=(const simd&)     = default;
// simd(simd&&) noexcept            = default;
// simd& operator=(simd&&) noexcept = default;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

// See https://www.open-std.org/jtc1/sc22/WG21/docs/papers/2019/n4808.pdf
// Default intialization performs no initialization of the elements; value-initialization initializes each element with T().
// [ Note: Thus, default initialization leaves the elements in an indeterminate state. end note ]
struct CheckSimdDefaultCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> pure_simd;

    static_assert(pure_simd.size() > 0);
  }
};
struct CheckSimdCopyCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> pure_simd([](_Tp i) { return i; });
    ex::simd<_Tp, SimdAbi> from_copy_ctor(pure_simd);
    std::array<_Tp, pure_simd.size()> expected_value;

    for (size_t i = 0; i < pure_simd.size(); ++i)
      expected_value[i] = pure_simd[i];

    assert_simd_value_correct(from_copy_ctor, expected_value);
  }
};
struct CheckSimdMoveCtor {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> pure_simd([](_Tp i) { return i; });
    ex::simd<_Tp, SimdAbi> from_move_ctor(std::move(pure_simd));
    std::array<_Tp, pure_simd.size()> expected_value;

    for (size_t i = 0; i < pure_simd.size(); ++i)
      expected_value[i] = pure_simd[i];

    assert_simd_value_correct(from_move_ctor, expected_value);
  }
};
struct CheckSimdCopyAssignment {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> pure_simd([](_Tp i) { return i; });
    ex::simd<_Tp, SimdAbi> from_copy_assignment;
    from_copy_assignment = pure_simd;
    std::array<_Tp, pure_simd.size()> expected_value;

    for (size_t i = 0; i < pure_simd.size(); ++i)
      expected_value[i] = pure_simd[i];

    assert_simd_value_correct(from_copy_assignment, expected_value);
  }
};
struct CheckSimdMoveAssignment {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> pure_simd([](_Tp i) { return i; });
    ex::simd<_Tp, SimdAbi> from_move_assignment;
    from_move_assignment = std::move(pure_simd);
    std::array<_Tp, pure_simd.size()> expected_value;

    for (size_t i = 0; i < pure_simd.size(); ++i)
      expected_value[i] = pure_simd[i];

    assert_simd_value_correct(from_move_assignment, expected_value);
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
  test_all_simd_abi<CheckSimdCopyCtor>();
  test_all_simd_abi<CheckSimdMoveCtor>();
  test_all_simd_abi<CheckSimdCopyAssignment>();
  test_all_simd_abi<CheckSimdMoveAssignment>();
  return 0;
}
