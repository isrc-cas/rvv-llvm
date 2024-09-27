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
// template <size_t... Sizes, class T, class Abi>
// tuple<simd<T, simd_abi::deduce_t<T, Sizes>>...> split(const simd<T, Abi>&) noexcept;

// template <size_t... Sizes, class T, class Abi>
// tuple<simd_mask<T, simd_mask_abi::deduce_t<T, Sizes>>...> split(const simd_mask<T, Abi>&) noexcept;

// template <class V, class Abi>
// array<V, simd_size_v<typename V::value_type, Abi> / V::size()> split(const simd<typename V::value_type, Abi>&) noexcept;

// template <class V, class Abi>
// array<V, simd_size_v<typename V::simd_type::value_type, Abi> / V::size()>
// split(const simd_mask<typename V::simd_type::value_type, Abi>&) noexcept;

#include "../test_utils.h"
#include <experimental/simd>

namespace ex = std::experimental::parallelism_v2;

// TODO: implement call in utility.h? or replace with std::apply later
template <typename Tuple, typename Func, size_t... N>
[[maybe_unused]] void call_tuple(Tuple& t, Func&& func, std::index_sequence<N...>) {
  static_cast<void>(std::initializer_list<int>{(func(std::get<N>(t)), 0)...});
}

template <typename... Args, typename Func>
[[maybe_unused]] void for_tuple(std::tuple<Args...>& t, Func&& func) {
  call_tuple(t, std::forward<Func>(func), std::make_index_sequence<sizeof...(Args)>{});
}

struct CheckSplitToArray {
  template <class _Tp, class SimdAbi>
  void operator()() {
    using simd_type_                       = ex::simd<_Tp, SimdAbi>;
    using simd_mask_type_                  = ex::simd_mask<_Tp, SimdAbi>;
    using target_simd_abi                  = ex::simd_abi::fixed_size<max_simd_size>;
    constexpr size_t origin_simd_size      = simd_type_::size();
    constexpr size_t origin_simd_mask_size = simd_mask_type_::size();

    {
      if constexpr (ex::simd_size_v<_Tp, target_simd_abi> % origin_simd_size == 0) {
        constexpr size_t array_count = ex::simd_size_v<_Tp, target_simd_abi> / origin_simd_size;
        using split_result_type      = std::array<simd_type_, array_count>;

        const ex::simd<_Tp, target_simd_abi> origin_simd([](_Tp i) { return i; });
        auto split_result = ex::split<simd_type_>(origin_simd);
        static_assert(std::is_same_v<decltype(split_result), split_result_type>);

        std::array<std::array<_Tp, origin_simd_size>, array_count> expected_value;
        for (size_t i = 0, idx = 0; i < expected_value.size(); ++i) {
          for (size_t j = 0; j < expected_value[i].size(); ++j) {
            expected_value[i][j] = origin_simd[idx++];
          }
        }

        for (size_t i = 0; i < split_result.size(); ++i) {
          assert(ex::is_simd_v<std::remove_reference_t<decltype(split_result[i])>>);
          assert_simd_value_correct<origin_simd_size>(split_result[i], expected_value[i]);
        }
      }
    }

    {
      if constexpr (ex::simd_size_v<_Tp, target_simd_abi> % origin_simd_mask_size == 0) {
        constexpr size_t array_count = ex::simd_size_v<_Tp, target_simd_abi> / origin_simd_mask_size;
        using split_result_type      = std::array<simd_mask_type_, array_count>;

        ex::simd_mask<_Tp, target_simd_abi> origin_simd_mask;

        for (size_t i = 0; i < origin_simd_mask_size;) {
          origin_simd_mask[i++] = 0;
          if (i + 1 < origin_simd_mask_size) {
            origin_simd_mask[i + 1] = 1;
            i++;
          }
        }
        auto split_result = ex::split<simd_mask_type_>(origin_simd_mask);
        static_assert(std::is_same_v<decltype(split_result), split_result_type>);

        std::array<std::array<_Tp, origin_simd_mask_size>, array_count> expected_value;
        for (size_t i = 0, idx = 0; i < expected_value.size(); ++i) {
          for (size_t j = 0; j < expected_value[i].size(); ++j) {
            expected_value[i][j] = origin_simd_mask[idx++];
          }
        }

        for (size_t i = 0; i < split_result.size(); ++i) {
          assert(ex::is_simd_mask_v<std::remove_reference_t<decltype(split_result[i])>>);
          assert_simd_mask_value_correct<origin_simd_mask_size>(split_result[i], expected_value[i]);
        }
      }
    }
  }
};

struct CheckSplitToTuple {
  template <class _Tp, class SimdAbi, std::size_t _Np>
  void operator()() {
    static auto initialize_mask = [](auto& origin_mask) {
      origin_mask[0] = 0;
      for (std::size_t i = 1; i < origin_mask.size(); ++i) {
        origin_mask[i] = 1;
      }
    };
    if constexpr (_Np == 1) {
      {
        ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 1); });

        auto temp = ex::split<1>(origin_simd);
        static_assert(std::is_same_v<decltype(temp), std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, _Np>>>>);
        size_t idx = 0;

        std::apply(
            [origin_simd, &idx](auto&& item) {
              for (size_t i = 0; i < item.size(); ++i) {
                assert(item[i] == origin_simd[idx]);
                ++idx;
              }
            },
            temp);
      }
      {
        ex::simd_mask<_Tp, SimdAbi> origin_mask;
        initialize_mask(origin_mask);

        auto temp = ex::split<1>(origin_mask);
        static_assert(std::is_same_v<decltype(temp), std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, _Np>>>>);
        size_t idx = 0;

        std::apply(
            [origin_mask, &idx](auto&& item) {
              for (size_t i = 0; i < item.size(); ++i) {
                assert(item[i] == origin_mask[idx]);
                ++idx;
              }
            },
            temp);
      }

    } else if constexpr (_Np == 4) {
      ex::simd<_Tp, SimdAbi> origin_simd([](_Tp i) { return static_cast<_Tp>(i + 1); });
      ex::simd_mask<_Tp, SimdAbi> origin_mask;
      initialize_mask(origin_mask);

      static const auto split22_to_simd = [](const auto& origin_simd) {
        auto temp = ex::split<2, 2>(origin_simd);

        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 2>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 2>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd[idx++]);
        });
      };
      static const auto split22_to_simd_mask = [](const auto& origin_simd_mask) {
        auto temp = ex::split<2, 2>(origin_simd_mask);

        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 2>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 2>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd_mask, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd_mask[idx++]);
        });
      };
      static const auto split13_to_simd = [](const auto& origin_simd) {
        auto temp = ex::split<1, 3>(origin_simd);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 3>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd[idx++]);
        });
      };
      static const auto split13_to_simd_mask = [](const auto& origin_simd_mask) {
        auto temp = ex::split<1, 3>(origin_simd_mask);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 3>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd_mask, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd_mask[idx++]);
        });
      };
      static const auto split31_to_simd = [](const auto& origin_simd) {
        auto temp = ex::split<3, 1>(origin_simd);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 3>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd[idx++]);
        });
      };
      static const auto split31_to_simd_mask = [](const auto& origin_simd_mask) {
        auto temp = ex::split<3, 1>(origin_simd_mask);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 3>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>>>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd_mask, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd_mask[idx++]);
        });
      };
      static const auto split112_to_simd = [](const auto& origin_simd) {
        auto temp = ex::split<1, 1, 2>(origin_simd);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 2>> >>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd[idx++]);
        });
      };
      static const auto split112_to_simd_mask = [](const auto& origin_simd_mask) {
        auto temp = ex::split<1, 1, 2>(origin_simd_mask);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 2>> >>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd_mask, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i)
            assert(item[i] == origin_simd_mask[idx++]);
        });
      };
      static const auto split1111_to_simd = [](const auto& origin_simd) {
        auto temp = ex::split<1, 1, 1, 1>(origin_simd);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd<_Tp, ex::simd_abi::deduce_t<_Tp, 1>> >>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i) {
            assert(item[i] == origin_simd[idx++]);
          }
        });
      };
      static const auto split1111_to_simd_mask = [](const auto& origin_simd_mask) {
        auto temp = ex::split<1, 1, 1, 1>(origin_simd_mask);
        static_assert(std::is_same_v<decltype(temp),
                                     typename std::tuple<ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>>,
                                                         ex::simd_mask<_Tp, ex::simd_abi::deduce_t<_Tp, 1>> >>);
        size_t idx = 0;
        for_tuple(temp, [origin_simd_mask, &idx](auto&& item) {
          for (size_t i = 0; i < item.size(); ++i) {
            assert(item[i] == origin_simd_mask[idx++]);
          }
        });
      };
      //split22(origin_mask, false);
      split22_to_simd(origin_simd);
      split22_to_simd_mask(origin_mask);
      split13_to_simd(origin_simd);
      split13_to_simd_mask(origin_mask);
      split31_to_simd(origin_simd);
      split31_to_simd_mask(origin_mask);
      split112_to_simd(origin_simd);
      split112_to_simd_mask(origin_mask);
      split1111_to_simd(origin_simd);
      split1111_to_simd_mask(origin_mask);
    } else { // pass if simd_size not equal 1 and 4
    }
  }
};

template <class F, std::size_t _Np, class _Tp>
void test_simd_abi() {}
template <class F, std::size_t _Np, class _Tp, class SimdAbi, class... SimdAbis>
void test_simd_abi() {
  if constexpr (std::is_same_v<F, CheckSplitToTuple>) {
    if constexpr (ex::simd_size_v<_Tp, SimdAbi> <= 4) {
      F{}.template operator()<_Tp, SimdAbi, ex::simd_size_v<_Tp, SimdAbi>>();
      test_simd_abi<F, _Np, _Tp, SimdAbis...>();
    }
  } else {
    F{}.template operator()<_Tp, SimdAbi>();
    test_simd_abi<F, _Np, _Tp, SimdAbis...>();
  }
}

int main(int, char**) {
  test_all_simd_abi<CheckSplitToArray>();
  test_all_simd_abi<CheckSplitToTuple>();
  return 0;
}
