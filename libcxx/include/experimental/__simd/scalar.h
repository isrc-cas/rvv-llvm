// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_EXPERIMENTAL___SIMD_SCALAR_H
#define _LIBCPP_EXPERIMENTAL___SIMD_SCALAR_H

#include <algorithm>
#include <experimental/__simd/declaration.h>
#include <experimental/__simd/utility.h>

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD_ABI

struct __scalar {
  static constexpr bool __is_abi_tag  = true;
  static constexpr size_t __simd_size = 1;
};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD_ABI

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

template <class _Tp>
struct __simd_storage<_Tp, simd_abi::__scalar> {
  _Tp __data;

  _Tp __get(size_t __idx) const noexcept { return (&__data)[__idx]; }

  void __set(size_t __idx, _Tp __v) noexcept { (&__data)[__idx] = __v; }
};

template <class _Tp>
struct __mask_storage<_Tp, simd_abi::__scalar> : __simd_storage<bool, simd_abi::__scalar> {};

template <class _Tp>
struct __simd_traits<_Tp, simd_abi::__scalar> {
  using _Simd = __simd_storage<_Tp, simd_abi::__scalar>;
  using _Mask = __mask_storage<_Tp, simd_abi::__scalar>;

  static _Simd __broadcast(_Tp __v) noexcept { return {__v}; }

  template <class _Generator>
  static _LIBCPP_HIDE_FROM_ABI _Simd __generate(_Generator&& __g) noexcept {
    return {__g(std::integral_constant<size_t, 0>())};
  }

  template <class _Up>
  static _LIBCPP_HIDE_FROM_ABI void __load(_Simd& __s, const _Up* __mem) noexcept {
    __s.__data = static_cast<_Tp>(__mem[0]);
  }

  template <class _Up>
  static _LIBCPP_HIDE_FROM_ABI void __store(_Simd __s, _Up* __mem) noexcept {
    *__mem = static_cast<_Up>(__s.__data);
  }

  static void __increment(_Simd& __s) noexcept { ++__s.__data; }

  static void __decrement(_Simd& __s) noexcept { --__s.__data; }

  static _Mask __negate(_Simd __s) noexcept { return {!__s.__data}; }

  static _Simd __bitwise_not(_Simd __s) noexcept { return {static_cast<_Tp>(~__s.__data)}; }

  static _Simd __unary_minus(_Simd __s) noexcept { return {static_cast<_Tp>(-__s.__data)}; }

  static _Simd __plus(_Simd __lhs, _Simd __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data + __rhs.__data)}; }

  static _Simd __minus(_Simd __lhs, _Simd __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data - __rhs.__data)}; }

  static _Simd __multiplies(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data * __rhs.__data)};
  }

  static _Simd __divides(_Simd __lhs, _Simd __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data / __rhs.__data)}; }

  static _Simd __modulus(_Simd __lhs, _Simd __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data % __rhs.__data)}; }

  static _Simd __bitwise_and(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data & __rhs.__data)};
  }

  static _Simd __bitwise_or(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data | __rhs.__data)};
  }

  static _Simd __bitwise_xor(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data ^ __rhs.__data)};
  }

  static _Simd __shift_left(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data << __rhs.__data)};
  }

  static _Simd __shift_right(_Simd __lhs, _Simd __rhs) noexcept {
    return {static_cast<_Tp>(__lhs.__data >> __rhs.__data)};
  }

  static _Simd __shift_left(_Simd __lhs, int __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data << __rhs)}; }

  static _Simd __shift_right(_Simd __lhs, int __rhs) noexcept { return {static_cast<_Tp>(__lhs.__data >> __rhs)}; }

  static _Mask __equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data == __rhs.__data}; }

  static _Mask __not_equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data != __rhs.__data}; }

  static _Mask __less_equal(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data <= __rhs.__data}; }

  static _Mask __less(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data < __rhs.__data}; }

  static _Tp __hmin(_Simd __s) { return __s.__data; }

  static _Tp __hmax(_Simd __s) { return __s.__data; }

  // fix macro conflict after https://reviews.llvm.org/D151654
  static _Simd __min(_Simd __a, _Simd __b) noexcept {
#ifdef min
#  undef min
#endif

    return {std::min(__a.__data, __b.__data)};
  }

  // fix macro conflict after https://reviews.llvm.org/D151654
  static _Simd __max(_Simd __a, _Simd __b) noexcept {
#ifdef max
#  undef max
#endif

    return {std::max(__a.__data, __b.__data)};
  }

  static std::pair<_Simd, _Simd> __minmax(_Simd __a, _Simd __b) noexcept { return {__min(__a, __b), __max(__a, __b)}; }

  static _Simd __clamp(_Simd __v, _Simd __lo, _Simd __hi) noexcept { return __min(__max(__v, __lo), __hi); }

  static _Simd __masked_assign(_Simd& __s, _Mask __m, _Simd __v) noexcept {
    __s.__data = __m.__data ? __v.__data : __s.__data;
    return __s;
  }

  template <class _BinaryOp>
  static _LIBCPP_HIDE_FROM_ABI _Tp __reduce(const _Simd& __s, _BinaryOp) {
    return __s.__data;
  }
};

template <class _Tp>
struct __mask_traits<_Tp, simd_abi::__scalar> {
  using _Mask = __mask_storage<_Tp, simd_abi::__scalar>;

  static _Mask __broadcast(bool __v) noexcept { return {__v}; }

  static void __load(_Mask& __s, const bool* __mem) noexcept { __s.__data = __mem[0]; }

  static void __store(_Mask __s, bool* __mem) noexcept { __mem[0] = __s.__data; }

  static _Mask __negate(_Mask __s) noexcept { return {{!__s.__data}}; }

  static _Mask __logical_and(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data && __rhs.__data}}; }

  static _Mask __logical_or(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data || __rhs.__data}}; }

  static _Mask __bitwise_and(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data && __rhs.__data}}; }

  static _Mask __bitwise_or(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data || __rhs.__data}}; }

  static _Mask __bitwise_xor(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data != __rhs.__data}}; }

  static bool __all_of(_Mask __s) noexcept { return __s.__data; }

  static bool __any_of(_Mask __s) noexcept { return __s.__data; }

  static bool __none_of(_Mask __s) noexcept { return !__s.__data; }

  static bool __some_of(_Mask) noexcept { return false; }

  static int __popcount(_Mask __s) noexcept { return __s.__data; }

  static int __find_first_set(_Mask) { return 0; }

  static int __find_last_set(_Mask) { return 0; }

  static _Mask __masked_assign(_Mask& __s, _Mask __m, _Mask __v) noexcept {
    __s.__data = __m.__data ? __v.__data : __s.__data;
    return __s;
  }
};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP_EXPERIMENTAL___SIMD_SCALAR_H
