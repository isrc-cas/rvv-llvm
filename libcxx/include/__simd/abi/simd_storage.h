#ifndef _LIBCPP___SIMD_ABI_SIMD_STORAGE_H
#define _LIBCPP___SIMD_ABI_SIMD_STORAGE_H

#include <experimental/__config>
#include <__simd/arch/config.h>
#include <__simd/abi/utility.h>
#if _LIBCPP_SIMD_HAVE_NEON
#include <arm_neon.h>
#endif
#if _LIBCPP_SIMD_HAVE_PPC
#include <altivec.h>
#endif

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD_ABI

struct __scalar {
  static constexpr bool __is_abi_tag = true;
  static constexpr size_t __simd_size = 1;
};

template <int _Np>
struct __vec_ext {
  static constexpr bool __is_abi_tag = _Np > 0;
  static constexpr size_t __simd_size = _Np;
};

template <int _Np>
struct __neon {
  static constexpr bool __is_abi_tag = true;
  static constexpr size_t __simd_size = _Np;
};

template <int _Np>
struct __ppc {
  static constexpr bool __is_abi_tag = true;
  static constexpr size_t __simd_size = _Np;
};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD_ABI

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

template <class _Tp, class _Abi>
struct __simd_storage;

template <class _Tp, class _Abi>
struct __mask_storage;

template <class _Tp>
struct __simd_storage <_Tp, simd_abi::__scalar> {
  _Tp __data;

  _Tp __get(size_t __idx) const noexcept { return (&__data)[__idx]; }

  void __set(size_t __idx, _Tp __v) noexcept { (&__data)[__idx] = __v; }
};

template <class _Tp>
struct __mask_storage <_Tp, simd_abi::__scalar> : __simd_storage<bool, simd_abi::__scalar> {};

template <class _Tp, int _Np>
struct __simd_storage <_Tp, simd_abi::__vec_ext<_Np>> {
#if defined(_LIBCPP_COMPILER_CLANG_BASED)
  _Tp __data __attribute__((vector_size(sizeof(_Tp) * _Np)));
#else
  _Tp __data __attribute__((vector_size(__next_pow_of_2(sizeof(_Tp) * _Np))));
#endif

  _Tp __get(size_t __idx) const noexcept { return __data[__idx]; }

  void __set(size_t __idx, _Tp __v) noexcept { __data[__idx] = __v; }
};

template <class _Tp, int _Np>
struct __mask_storage <_Tp, simd_abi::__vec_ext<_Np>> : __simd_storage<decltype(__choose_mask_type<_Tp>()), simd_abi::__vec_ext<_Np>> {};

#if _LIBCPP_SIMD_HAVE_NEON
template <class _Tp, int _Np>
struct __neon_type;

template <class _Tp, int _Np>
using __neon_type_t = typename __neon_type<_Tp, _Np>::type;

#define _LIBCPP_SIMD_ARM_NEON_TYPES(_Tp, _NTp, _Np)         \
template <>                                                 \
struct __neon_type<_Tp, _Np>                                \
{ using type = _NTp##x##_Np##_t; };
_LIBCPP_SIMD_ARM_NEON_TYPES(char,               uint8,    8)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned char,      uint8,    8)
_LIBCPP_SIMD_ARM_NEON_TYPES(signed char,        int8,     8)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned short,     uint16,   4)
_LIBCPP_SIMD_ARM_NEON_TYPES(short,              int16,    4)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned,           uint32,   2)
_LIBCPP_SIMD_ARM_NEON_TYPES(int,                int32,    2)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned long long, uint64,   1)
_LIBCPP_SIMD_ARM_NEON_TYPES(long long,          int64,    1)
_LIBCPP_SIMD_ARM_NEON_TYPES(float,              float32,  2)
_LIBCPP_SIMD_ARM_NEON_TYPES(double,             float64,  1)
_LIBCPP_SIMD_ARM_NEON_TYPES(long double,        float64,  1)
_LIBCPP_SIMD_ARM_NEON_TYPES(char,               uint8,    16)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned char,      uint8,    16)
_LIBCPP_SIMD_ARM_NEON_TYPES(signed char,        int8,     16)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned short,     uint16,   8)
_LIBCPP_SIMD_ARM_NEON_TYPES(short,              int16,    8)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned,           uint32,   4)
_LIBCPP_SIMD_ARM_NEON_TYPES(int,                int32,    4)
_LIBCPP_SIMD_ARM_NEON_TYPES(unsigned long long, uint64,   2)
_LIBCPP_SIMD_ARM_NEON_TYPES(long long,          int64,    2)
_LIBCPP_SIMD_ARM_NEON_TYPES(float,              float32,  4)
_LIBCPP_SIMD_ARM_NEON_TYPES(double,             float64,  2)
_LIBCPP_SIMD_ARM_NEON_TYPES(long double,        float64,  2)

#define _LIBCPP_SIMD_ARM_NEON_STORAGE(_Tp, _Np)             \
template <>                                                 \
struct __simd_storage <_Tp, simd_abi::__neon<_Np>> {        \
  __neon_type_t<_Tp, _Np> __data;                           \
  _Tp __get(size_t __idx) const noexcept                    \
  { return __data[__idx]; }                                 \
  void __set(size_t __idx, _Tp __v) noexcept                \
  { __data[__idx] = __v; }                                  \
};
_LIBCPP_SIMD_ARM_NEON_STORAGE(char,               8)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned char,      8)
_LIBCPP_SIMD_ARM_NEON_STORAGE(signed char,        8)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned short,     4)
_LIBCPP_SIMD_ARM_NEON_STORAGE(short,              4)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned,           2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(int,                2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned long long, 1)
_LIBCPP_SIMD_ARM_NEON_STORAGE(long long,          1)
_LIBCPP_SIMD_ARM_NEON_STORAGE(float,              2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(double,             1)
_LIBCPP_SIMD_ARM_NEON_STORAGE(long double,        1)
_LIBCPP_SIMD_ARM_NEON_STORAGE(char,               16)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned char,      16)
_LIBCPP_SIMD_ARM_NEON_STORAGE(signed char,        16)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned short,     8)
_LIBCPP_SIMD_ARM_NEON_STORAGE(short,              8)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned,           4)
_LIBCPP_SIMD_ARM_NEON_STORAGE(int,                4)
_LIBCPP_SIMD_ARM_NEON_STORAGE(unsigned long long, 2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(long long,          2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(float,              4)
_LIBCPP_SIMD_ARM_NEON_STORAGE(double,             2)
_LIBCPP_SIMD_ARM_NEON_STORAGE(long double,        2)

template <class _Tp, int _Np>
struct __mask_storage <_Tp, simd_abi::__neon<_Np>> : __simd_storage<decltype(__choose_mask_type<_Tp>()), simd_abi::__neon<_Np>> {};
#endif

#if _LIBCPP_SIMD_HAVE_PPC
template <class _Tp>
struct __ppc_type;

template <class _Tp>
using __ppc_type_t = typename __ppc_type<_Tp>::type;

#define _LIBCPP_SIMD_PPC_TYPES(_Tp)         \
template <>                                 \
struct __ppc_type<_Tp>                      \
{ using type = vector _Tp; };
// PPC types
_LIBCPP_SIMD_PPC_TYPES(unsigned char)
_LIBCPP_SIMD_PPC_TYPES(signed char)
_LIBCPP_SIMD_PPC_TYPES(unsigned short)
_LIBCPP_SIMD_PPC_TYPES(short)
_LIBCPP_SIMD_PPC_TYPES(unsigned)
_LIBCPP_SIMD_PPC_TYPES(int)
_LIBCPP_SIMD_PPC_TYPES(unsigned long long)
_LIBCPP_SIMD_PPC_TYPES(long long)
_LIBCPP_SIMD_PPC_TYPES(float)
_LIBCPP_SIMD_PPC_TYPES(double)

template <class _Tp, int _Np>
struct __simd_storage <_Tp, simd_abi::__ppc<_Np>> {
  __ppc_type_t<_Tp> __data;
  _Tp __get(size_t __idx) const noexcept
  { return vec_extract(__data, __idx); }
  void __set(size_t __idx, _Tp __v) noexcept
  { vec_insert(__v, __data, __idx); }
}

template <class _Tp, int _Np>
struct __mask_storage <_Tp, simd_abi::__ppc<_Np>> : __simd_storage<decltype(__choose_mask_type<_Tp>()), simd_abi::__ppc<_Np>> {};
#endif

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP___SIMD_ABI_SIMD_STORAGE_H
