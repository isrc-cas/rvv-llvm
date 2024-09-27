// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_EXPERIMENTAL___SIMD_SIMD_STORAGE_H
#define _LIBCPP_EXPERIMENTAL___SIMD_SIMD_STORAGE_H

#include <experimental/__config>

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

template <class _Tp, class _Abi>
struct __simd_storage;

template <class _Tp, class _Abi>
struct __mask_storage;

template <class _Tp, class _Abi>
struct __simd_traits;

template <class _Tp, class _Abi>
struct __mask_traits;

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP_EXPERIMENTAL___SIMD_SIMD_STORAGE_H
