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
// [simd.reference]
// template<class U> reference=(U&& x) && noexcept;
// template<class U> reference+=(U&& x) && noexcept;
// template<class U> reference-=(U&& x) && noexcept;
// template<class U> reference*=(U&& x) && noexcept;
// template<class U> reference/=(U&& x) && noexcept;
// template<class U> reference%=(U&& x) && noexcept;
// template<class U> reference|=(U&& x) && noexcept;
// template<class U> reference&=(U&& x) && noexcept;
// template<class U> reference^=(U&& x) && noexcept;
// template<class U> reference<<=(U&& x) && noexcept;
// template<class U> reference>>=(U&& x) && noexcept;
// reference++() && noexcept;
// value_type++(int) && noexcept;
// reference--() && noexcept;
// value_type--(int) && noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

struct CheckReferenceOperators {
  template <class _Tp, class SimdAbi>
  void operator()() {
    ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 2); });

    {
      assert(origin_simd[0] == static_cast<_Tp>(2));

      using U = _Tp;
      U x     = static_cast<U>(3);

      auto res = origin_simd[0] = (x);
      assert(origin_simd[0] == static_cast<_Tp>(3));
      static_assert(is_simd_reference<decltype(res)>::value);
      assert(res == static_cast<_Tp>(3));
    }

    {
      assert(origin_simd[0] == static_cast<_Tp>(3));

      using U = _Tp;
      U x     = static_cast<U>(4);

      auto res = origin_simd[0] += (x);
      assert(origin_simd[0] == static_cast<_Tp>(7));
      static_assert(is_simd_reference<decltype(res)>::value);
      assert(res == static_cast<_Tp>(7));
    }
    {
      assert(origin_simd[0] == static_cast<_Tp>(7));

      using U = _Tp;
      U x     = static_cast<U>(6);

      auto res = origin_simd[0] -= (x);
      assert(origin_simd[0] == static_cast<_Tp>(1));
      static_assert(is_simd_reference<decltype(res)>::value);
      assert(res == static_cast<_Tp>(1));
    }
    {
      assert(origin_simd[0] == static_cast<_Tp>(1));

      using U = _Tp;
      U x     = static_cast<U>(12);

      auto res = origin_simd[0] *= (x);
      assert(origin_simd[0] == static_cast<_Tp>(12));
      static_assert(is_simd_reference<decltype(res)>::value);
      assert(res == static_cast<_Tp>(12));
    }

    {
      assert(origin_simd[0] == static_cast<_Tp>(12));

      using U = _Tp;
      U x     = static_cast<U>(2);

      auto res = origin_simd[0] /= (x);
      assert(origin_simd[0] == static_cast<_Tp>(6));
      static_assert(is_simd_reference<decltype(res)>::value);
      assert(res == static_cast<_Tp>(6));
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(6));

        using U = _Tp;
        U x     = static_cast<U>(4);

        auto res = origin_simd[0] %= (x);
        assert(origin_simd[0] == static_cast<_Tp>(2));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(2));
      }
    }

    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(2));

        using U = _Tp;
        U x     = static_cast<U>(5);

        auto res = origin_simd[0] |= (x);
        assert(origin_simd[0] == static_cast<_Tp>(7));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(7));
      }
    }

    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(7));

        using U = _Tp;
        U x     = static_cast<U>(3);

        auto res = origin_simd[0] &= (x);
        assert(origin_simd[0] == static_cast<_Tp>(3));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(3));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(3));

        using U = _Tp;
        U x     = static_cast<U>(8);

        auto res = origin_simd[0] ^= (x);
        assert(origin_simd[0] == static_cast<_Tp>(11));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(11));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(11));

        using U = _Tp;
        U x     = static_cast<U>(2);

        auto res = origin_simd[0] <<= (x);
        assert(origin_simd[0] == static_cast<_Tp>(44));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(44));
      }
    }

    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(44));

        using U = _Tp;
        U x     = static_cast<U>(2);

        auto res = origin_simd[0] >>= (x);
        assert(origin_simd[0] == static_cast<_Tp>(11));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(11));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(11));

        auto res = ++origin_simd[0];
        assert(origin_simd[0] == static_cast<_Tp>(12));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(12));

      } else {
        assert(origin_simd[0] == static_cast<_Tp>(6));

        auto res = ++origin_simd[0];
        assert(origin_simd[0] == static_cast<_Tp>(7));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(7));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(12));

        auto res = --origin_simd[0];
        assert(origin_simd[0] == static_cast<_Tp>(11));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(11));

      } else {
        assert(origin_simd[0] == static_cast<_Tp>(7));

        auto res = --origin_simd[0];
        assert(origin_simd[0] == static_cast<_Tp>(6));
        static_assert(is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(6));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(11));

        auto res = origin_simd[0]++;

        assert(origin_simd[0] == static_cast<_Tp>(12));
        static_assert(!is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(11));

      } else {
        assert(origin_simd[0] == static_cast<_Tp>(6));

        auto res = origin_simd[0]++;

        assert(origin_simd[0] == static_cast<_Tp>(7));
        static_assert(!is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(6));
      }
    }
    {
      if constexpr (!std::is_floating_point_v<_Tp>) {
        assert(origin_simd[0] == static_cast<_Tp>(12));

        auto res = origin_simd[0]--;

        assert(origin_simd[0] == static_cast<_Tp>(11));
        static_assert(!is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(12));

      } else {
        assert(origin_simd[0] == static_cast<_Tp>(7));

        auto res = origin_simd[0]--;

        assert(origin_simd[0] == static_cast<_Tp>(6));
        static_assert(!is_simd_reference<decltype(res)>::value);
        assert(res == static_cast<_Tp>(7));
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
  test_all_simd_abi<CheckReferenceOperators>();
  return 0;
}
