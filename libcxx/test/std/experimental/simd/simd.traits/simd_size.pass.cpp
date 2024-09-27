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
// [simd.traits]
//template <class T, class Abi = simd_abi::compatible<T>> struct simd_size;
//template <class T, class Abi = simd_abi::compatible<T>>
//inline constexpr size_t ex::simd_size_v = ex::simd_size<T, Abi>::value;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckSimdSizeScalar {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == 1);
  };
};
struct CheckSimdSizeVScalar {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == 1);
  };
};
struct CheckSimdSizeDeduce {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == _Np);
  };
};
struct CheckSimdSizeVDeduce {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == _Np);
  };
};
struct CheckSimdSizeFixedSize {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == _Np);
  };
};
struct CheckSimdSizeVFixedSize {
  template <class _Tp, class SimdAbi, size_t _Np>
  void operator()() {
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == _Np);
  };
};
struct CheckSimdSizeCompatible {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == 16 / sizeof(_Tp));
  };
};
struct CheckSimdSizeVCompatible {
  template <class _Tp, class SimdAbi>
  void operator()() {
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == 16 / sizeof(_Tp));
  };
};

struct CheckSimdSizeNative {
  template <class _Tp, class SimdAbi>
  void operator()() {
#if defined(__AVX__)
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == _LIBCPP_NATIVE_SIMD_WIDTH_IN_BYTES / sizeof(_Tp));
#else
    static_assert(ex::simd_size<_Tp, SimdAbi>::value == 16 / sizeof(_Tp));
#endif
  };
};
struct CheckSimdSizeVNative {
  template <class _Tp, class SimdAbi>
  void operator()() {
#if defined(__AVX__)
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == _LIBCPP_NATIVE_SIMD_WIDTH_IN_BYTES / sizeof(_Tp));
#else
    static_assert(ex::simd_size_v<_Tp, SimdAbi> == 16 / sizeof(_Tp));
#endif
  };
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  if constexpr (std::is_same<F, CheckSimdSizeScalar>::value || std::is_same<F, CheckSimdSizeVScalar>::value) {
    F{}.template operator()<_Tp, ex::simd_abi::scalar>();
  } else if constexpr (std::is_same<F, CheckSimdSizeDeduce>::value || std::is_same<F, CheckSimdSizeVDeduce>::value) {
    F{}.template operator()<_Tp, ex::simd_abi::deduce_t<_Tp, _Np>, _Np>();
  } else if constexpr (std::is_same<F, CheckSimdSizeFixedSize>::value ||
                       std::is_same<F, CheckSimdSizeVFixedSize>::value) {
    F{}.template operator()<_Tp, ex::simd_abi::fixed_size<_Np>, _Np>();
  } else if constexpr (std::is_same<F, CheckSimdSizeCompatible>::value ||
                       std::is_same<F, CheckSimdSizeVCompatible>::value) {
    F{}.template operator()<_Tp, ex::simd_abi::compatible<_Tp>>();
  } else {
    F{}.template operator()<_Tp, ex::simd_abi::native<_Tp>>();
  }
  test_simd_abi<F, _Np, _Tp, SimdAbis...>();
}

int main(int, char**) {
  test_all_simd_abi<CheckSimdSizeScalar>();
  test_all_simd_abi<CheckSimdSizeVScalar>();

  test_all_simd_abi<CheckSimdSizeDeduce>();
  test_all_simd_abi<CheckSimdSizeVDeduce>();

  test_all_simd_abi<CheckSimdSizeFixedSize>();
  test_all_simd_abi<CheckSimdSizeVFixedSize>();

  test_all_simd_abi<CheckSimdSizeCompatible>();
  test_all_simd_abi<CheckSimdSizeVCompatible>();

  test_all_simd_abi<CheckSimdSizeNative>();
  test_all_simd_abi<CheckSimdSizeVNative>();

  return 0;
}
