// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_EXPERIMENTAL___SIMD_vec_ext_H
#define _LIBCPP_EXPERIMENTAL___SIMD_vec_ext_H

#include <experimental/__simd/declaration.h>
#include <experimental/__simd/utility.h>
#include <functional>

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD_ABI

template <int _Np>
struct __vec_ext {
  static constexpr bool __is_abi_tag  = _Np > 0 && _Np <= 32;
  static constexpr size_t __simd_size = _Np;
};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD_ABI

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

template <class _Tp, int _Np>
struct __simd_storage<_Tp, simd_abi::__vec_ext<_Np>> {
#if defined(_LIBCPP_COMPILER_CLANG_BASED)
  _Tp __data __attribute__((vector_size(sizeof(_Tp) * _Np)));
#else
  _Tp __data __attribute__((vector_size(__next_pow_of_2(sizeof(_Tp) * _Np))));
#endif

  _Tp __get(size_t __idx) const noexcept { return __data[__idx]; }

  void __set(size_t __idx, _Tp __v) noexcept { __data[__idx] = __v; }
};

template <class _Tp, int _Np>
struct __mask_storage<_Tp, simd_abi::__vec_ext<_Np>>
    : __simd_storage<decltype(__choose_mask_type<_Tp>()), simd_abi::__vec_ext<_Np>> {};

template <class _Tp, int _Np>
struct __simd_traits<_Tp, simd_abi::__vec_ext<_Np>> {
  using _Simd = __simd_storage<_Tp, simd_abi::__vec_ext<_Np>>;
  using _Mask = __mask_storage<_Tp, simd_abi::__vec_ext<_Np>>;

  static _Simd __broadcast(_Tp __v) noexcept {
    // TODO: Optimizeable
    return __generate([=](size_t) { return __v; });
  }

  template <class _Generator, size_t... _Is>
  static _LIBCPP_HIDE_FROM_ABI _Simd __generate_init(_Generator&& __g, std::index_sequence<_Is...>) {
    // TODO: Optimizeable
    // _Simd specified here is to work around GCC
    return _Simd{{__g(std::integral_constant<size_t, _Is>())...}};
  }

  template <class _Generator>
  static _LIBCPP_HIDE_FROM_ABI _Simd __generate(_Generator&& __g) noexcept {
    // TODO: Optimizeable
    return __generate_init(std::forward<_Generator>(__g), std::make_index_sequence<_Np>());
  }

  template <class _Up>
  static _LIBCPP_HIDE_FROM_ABI void __load(_Simd& __s, const _Up* __mem) noexcept {
    // TODO: Optimize with intrinsics
    for (size_t __i = 0; __i < _Np; __i++)
      __s.__data[__i] = static_cast<_Tp>(__mem[__i]);
  }

  template <class _Up>
  static _LIBCPP_HIDE_FROM_ABI void __store(_Simd __s, _Up* __mem) noexcept {
    // TODO: Optimize with intrinsics
    for (size_t __i = 0; __i < _Np; __i++)
      __mem[__i] = static_cast<_Up>(__s.__data[__i]);
  }

  static void __increment(_Simd& __s) noexcept { __s.__data = __s.__data + 1; }

  static void __decrement(_Simd& __s) noexcept { __s.__data = __s.__data - 1; }

  static _Mask __negate(_Simd __s) noexcept { return {!__s.__data}; }

  static _Simd __bitwise_not(_Simd __s) noexcept { return {~__s.__data}; }

  static _Simd __unary_minus(_Simd __s) noexcept { return {-__s.__data}; }

  static _Simd __plus(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data + __rhs.__data}; }

  static _Simd __minus(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data - __rhs.__data}; }

  static _Simd __multiplies(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data * __rhs.__data}; }

  static _Simd __divides(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data / __rhs.__data}; }

  static _Simd __modulus(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data % __rhs.__data}; }

  static _Simd __bitwise_and(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data & __rhs.__data}; }

  static _Simd __bitwise_or(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data | __rhs.__data}; }

  static _Simd __bitwise_xor(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data ^ __rhs.__data}; }

  static _Simd __shift_left(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data << __rhs.__data}; }

  static _Simd __shift_right(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data >> __rhs.__data}; }

  static _Simd __shift_left(_Simd __lhs, int __rhs) noexcept { return {__lhs.__data << __rhs}; }

  static _Simd __shift_right(_Simd __lhs, int __rhs) noexcept { return {__lhs.__data >> __rhs}; }

  static _Mask __equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {{__lhs.__data == __rhs.__data}}; }

  static _Mask __not_equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {{__lhs.__data != __rhs.__data}}; }

  static _Mask __less_equal(_Simd __lhs, _Simd __rhs) noexcept { return {{__lhs.__data <= __rhs.__data}}; }

  static _Mask __less(_Simd __lhs, _Simd __rhs) noexcept { return {{__lhs.__data < __rhs.__data}}; }

  static _Tp __hmin(_Simd __s) noexcept { return __builtin_reduce_min(__s.__data); }

  static _Tp __hmax(_Simd __s) noexcept { return __builtin_reduce_max(__s.__data); }

  static _Simd __min(_Simd __a, _Simd __b) noexcept { return {__a.__data < __b.__data ? __a.__data : __b.__data}; }

  static _Simd __max(_Simd __a, _Simd __b) noexcept { return {__a.__data > __b.__data ? __a.__data : __b.__data}; }

  static std::pair<_Simd, _Simd> __minmax(_Simd __a, _Simd __b) noexcept { return {__min(__a, __b), __max(__a, __b)}; }

  static _Simd __clamp(_Simd __v, _Simd __lo, _Simd __hi) noexcept { return __min(__max(__v, __lo), __hi); }

  static _Simd __masked_assign(_Simd& __s, _Mask __m, _Simd __v) noexcept {
    __s.__data = __m.__data ? __v.__data : __s.__data;
    return __s;
  }

  static _Tp __reduce(const _Simd& __s, plus<>) { return __builtin_reduce_add(__s.__data); }

  static _Tp __reduce(const _Simd& __s, multiplies<>) { return __builtin_reduce_mul(__s.__data); }

  static _Tp __reduce(const _Simd& __s, bit_and<>) { return __builtin_reduce_and(__s.__data); }

  static _Tp __reduce(const _Simd& __s, bit_or<>) { return __builtin_reduce_or(__s.__data); }

  static _Tp __reduce(const _Simd& __s, bit_xor<>) { return __builtin_reduce_xor(__s.__data); }
};

template <class _Tp, int _Np>
struct __mask_traits<_Tp, simd_abi::__vec_ext<_Np>> {
  using _Mask = __mask_storage<_Tp, simd_abi::__vec_ext<_Np>>;

  static _Mask __broadcast(bool __v) noexcept {
    // TODO: Optimizeable
    return __generate([=](size_t) { return std::experimental::parallelism_v2::__set_all_bits<_Tp>(__v); });
  }

  template <class _Generator, size_t... _Is>
  static _LIBCPP_HIDE_FROM_ABI _Mask __generate_init(_Generator&& __g, std::index_sequence<_Is...>) {
    // TODO: Optimizeable
    // _Simd specified here is to work around GCC
    return _Mask{{__g(std::integral_constant<size_t, _Is>())...}};
  }

  template <class _Generator>
  static _LIBCPP_HIDE_FROM_ABI _Mask __generate(_Generator&& __g) noexcept {
    // TODO: Optimizeable
    return __generate_init(std::forward<_Generator>(__g), std::make_index_sequence<_Np>());
  }

  static void __load(_Mask& __s, const bool* __mem) noexcept {
    // TODO: Optimize with intrinsics
    for (size_t __i = 0; __i < _Np; __i++)
      __s.__data[__i] = std::experimental::parallelism_v2::__set_all_bits<_Tp>(__mem[__i]);
  }

  static void __store(_Mask __s, bool* __mem) noexcept {
    // TODO: Optimize with intrinsics
    for (size_t __i = 0; __i < _Np; __i++)
      __mem[__i] = static_cast<bool>(__s.__data[__i]);
  }

  static _Mask __negate(_Mask __s) noexcept { return {{~__s.__data}}; }

  static _Mask __logical_and(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data & __rhs.__data}}; }

  static _Mask __logical_or(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data | __rhs.__data}}; }

  static _Mask __bitwise_and(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data & __rhs.__data}}; }

  static _Mask __bitwise_or(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data | __rhs.__data}}; }

  static _Mask __bitwise_xor(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data ^ __rhs.__data}}; }

  static bool __all_of(_Mask __s) noexcept {
    // TODO: Optimizeable
    for (size_t __i = 0; __i < _Np; ++__i)
      if (!__s.__data[__i])
        return false;
    return true;
  }

  static bool __any_of(_Mask __s) noexcept {
    // TODO: Optimizeable
    for (size_t __i = 0; __i < _Np; ++__i)
      if (__s.__data[__i])
        return true;
    return false;
  }

  static bool __none_of(_Mask __s) noexcept {
    // TODO: Optimizeable
    for (size_t __i = 0; __i < _Np; ++__i)
      if (__s.__data[__i])
        return false;
    return true;
  }

  static bool __some_of(_Mask __s) noexcept {
    // TODO: Optimizeable
    for (size_t __i = 1; __i < _Np; ++__i)
      if (__s.__data[__i] != __s.__data[__i - 1])
        return true;
    return false;
  }

  static int __popcount(_Mask __s) noexcept {
    // TODO: Optimizeable
    int __count = 0;
    for (size_t __i = 0; __i < _Np; ++__i)
      __count += __s.__data[__i] != 0;
    return __count;
  }

  static int __find_first_set(_Mask __s) {
    // TODO: Optimizeable
    size_t __i = 0;
    for (; __i < _Np; ++__i)
      if (__s.__data[__i])
        break;
    return __i;
  }

  static int __find_last_set(_Mask __s) {
    // TODO: Optimizeable
    size_t __i = 1;
    for (; __i < _Np; ++__i)
      if (__s.__data[_Np - __i])
        break;
    return _Np - __i;
  }

  static _Mask __masked_assign(_Mask& __s, _Mask __m, _Mask __v) noexcept {
    __s.__data = __m.__data ? __v.__data : __s.__data;
    return __s;
  }
};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP_EXPERIMENTAL___SIMD_vec_ext_H
