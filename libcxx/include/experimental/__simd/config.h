// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_EXPERIMENTAL___SIMD_CONFIG_H
#define _LIBCPP_EXPERIMENTAL___SIMD_CONFIG_H

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

#ifdef __MMX__
#  define _LIBCPP_SIMD_HAVE_MMX 1
#else
#  define _LIBCPP_SIMD_HAVE_MMX 0
#endif

#ifdef __SSE__
#  define _LIBCPP_SIMD_HAVE_SSE 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE 0
#endif

#ifdef __SSE2__
#  define _LIBCPP_SIMD_HAVE_SSE2 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE2 0
#endif

#ifdef __SSE3__
#  define _LIBCPP_SIMD_HAVE_SSE3 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE3 0
#endif

#ifdef __SSSE3__
#  define _LIBCPP_SIMD_HAVE_SSSE3 1
#else
#  define _LIBCPP_SIMD_HAVE_SSSE3 0
#endif

#ifdef __SSE4_1__
#  define _LIBCPP_SIMD_HAVE_SSE4_1 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE4_1 0
#endif

#ifdef __SSE4_2__
#  define _LIBCPP_SIMD_HAVE_SSE4_2 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE4_2 0
#endif

#ifdef __XOP__
#  define _LIBCPP_SIMD_HAVE_XOP 1
#else
#  define _LIBCPP_SIMD_HAVE_XOP 0
#endif

#ifdef __AVX__
#  define _LIBCPP_SIMD_HAVE_AVX 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX 0
#endif

#ifdef __AVX2__
#  define _LIBCPP_SIMD_HAVE_AVX2 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX2 0
#endif

#ifdef __BMI__
#  define _LIBCPP_SIMD_HAVE_BMI1 1
#else
#  define _LIBCPP_SIMD_HAVE_BMI1 0
#endif

#ifdef __BMI2__
#  define _LIBCPP_SIMD_HAVE_BMI2 1
#else
#  define _LIBCPP_SIMD_HAVE_BMI2 0
#endif

#ifdef __LZCNT__
#  define _LIBCPP_SIMD_HAVE_LZCNT 1
#else
#  define _LIBCPP_SIMD_HAVE_LZCNT 0
#endif

#ifdef __SSE4A__
#  define _LIBCPP_SIMD_HAVE_SSE4A 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE4A 0
#endif

#ifdef __FMA__
#  define _LIBCPP_SIMD_HAVE_FMA 1
#else
#  define _LIBCPP_SIMD_HAVE_FMA 0
#endif

#ifdef __FMA4__
#  define _LIBCPP_SIMD_HAVE_FMA4 1
#else
#  define _LIBCPP_SIMD_HAVE_FMA4 0
#endif

#ifdef __F16C__
#  define _LIBCPP_SIMD_HAVE_F16C 1
#else
#  define _LIBCPP_SIMD_HAVE_F16C 0
#endif

#ifdef __POPCNT__
#  define _LIBCPP_SIMD_HAVE_POPCNT 1
#else
#  define _LIBCPP_SIMD_HAVE_POPCNT 0
#endif

#ifdef __AVX512F__
#  define _LIBCPP_SIMD_HAVE_AVX512F 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX512F 0
#endif

#ifdef __AVX512DQ__
#  define _LIBCPP_SIMD_HAVE_AVX512DQ 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX512DQ 0
#endif

#ifdef __AVX512VL__
#  define _LIBCPP_SIMD_HAVE_AVX512VL 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX512VL 0
#endif

#ifdef __AVX512BW__
#  define _LIBCPP_SIMD_HAVE_AVX512BW 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX512BW 0
#endif

#if _LIBCPP_SIMD_HAVE_SSE
#  define _LIBCPP_SIMD_HAVE_SSE_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_SSE_ABI 0
#endif
#if _LIBCPP_SIMD_HAVE_SSE2
#  define _LIBCPP_SIMD_HAVE_FULL_SSE_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_FULL_SSE_ABI 0
#endif

#if _LIBCPP_SIMD_HAVE_AVX
#  define _LIBCPP_SIMD_HAVE_AVX_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX_ABI 0
#endif
#if _LIBCPP_SIMD_HAVE_AVX2
#  define _LIBCPP_SIMD_HAVE_FULL_AVX_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_FULL_AVX_ABI 0
#endif

#if _LIBCPP_SIMD_HAVE_AVX512F
#  define _LIBCPP_SIMD_HAVE_AVX512_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_AVX512_ABI 0
#endif
#if _LIBCPP_SIMD_HAVE_AVX512BW
#  define _LIBCPP_SIMD_HAVE_FULL_AVX512_ABI 1
#else
#  define _LIBCPP_SIMD_HAVE_FULL_AVX512_ABI 0
#endif

#if _LIBCPP_SIMD_HAVE_SSE || _LIBCPP_SIMD_HAVE_MMX
#  define _LIBCPP_SIMD_X86INTRIN 1
#else
#  define _LIBCPP_SIMD_X86INTRIN 0
#endif

#ifdef __ARM_NEON
#  define _LIBCPP_SIMD_HAVE_NEON 1
#else
#  define _LIBCPP_SIMD_HAVE_NEON 0
#endif

#if defined __ARM_NEON && (__ARM_ARCH >= 8 || defined __aarch64__)
#  define _LIBCPP_SIMD_HAVE_NEON_A32 1
#else
#  define _LIBCPP_SIMD_HAVE_NEON_A32 0
#endif

#if defined __ARM_NEON && defined __aarch64__
#  define _LIBCPP_SIMD_HAVE_NEON_A64 1
#else
#  define _LIBCPP_SIMD_HAVE_NEON_A64 0
#endif

#ifdef _ARCH_PWR10
#  define _LIBCPP_SIMD_HAVE_POWER10VEC 1
#else
#  define _LIBCPP_SIMD_HAVE_POWER10VEC 0
#endif

#ifdef __POWER9_VECTOR__
#  define _LIBCPP_SIMD_HAVE_POWER9VEC 1
#else
#  define _LIBCPP_SIMD_HAVE_POWER9VEC 0
#endif

#ifdef __POWER8_VECTOR__
#  define _LIBCPP_SIMD_HAVE_POWER8VEC 1
#else
#  define _LIBCPP_SIMD_HAVE_POWER8VEC _LIBCPP_SIMD_HAVE_POWER9VEC
#endif

#ifdef __VSX__
#  define _LIBCPP_SIMD_HAVE_VSX 1
#else
#  define _LIBCPP_SIMD_HAVE_VSX _LIBCPP_SIMD_HAVE_POWER8VEC
#endif

#ifdef __ALTIVEC__
#  define _LIBCPP_SIMD_HAVE_PPC 1
#else
#  define _LIBCPP_SIMD_HAVE_PPC _LIBCPP_SIMD_HAVE_VSX
#endif

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP_EXPERIMENTAL___SIMD_CONFIG_H
