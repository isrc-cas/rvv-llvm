#ifndef _LIBCPP___SIMD_ABI_SCALAR_H
#define _LIBCPP___SIMD_ABI_SCALAR_H

#include <__simd/abi/simd_storage.h>

_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL_SIMD

template <class _Tp>
struct __simd_traits<_Tp, simd_abi::__scalar> {
  using _Simd = __simd_storage<_Tp, simd_abi::__scalar>;
  using _Mask = __mask_storage<_Tp, simd_abi::__scalar>;

  static _Simd __broadcast(_Tp __v) noexcept { return {__v}; }

  template <class _Generator>
  static _Simd __generate(_Generator&& __g) noexcept {
    return {__g(std::integral_constant<size_t, 0>())};
  }

  template <class _Up, class _Flags>
  static _Simd __load(_Up* __mem, _Flags) noexcept {
    return {static_cast<_Tp>(*__mem)};
  }

  template <class _Up, class _Flags>
  static void __store(_Simd __s, const _Up* __mem, _Flags) noexcept {
    *__mem = static_cast<_Up>(__s.__data);
  }

  static void __increment(_Simd& __s) noexcept { ++__s.__data; }

  static void __decrement(_Simd& __s) noexcept { --__s.__data; }

  static _Simd __negate(_Simd __s) noexcept { return {!__s.__data}; }

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

  static _Mask __equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data == __rhs.__data}; }

  static _Mask __not_equal_to(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data != __rhs.__data}; }

  static _Mask __less_equal(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data <= __rhs.__data}; }

  static _Mask __less(_Simd __lhs, _Simd __rhs) noexcept { return {__lhs.__data < __rhs.__data}; }

  static _Tp __hmin(_Simd __s) { return __s.__data; }

  static _Tp __hmax(_Simd __s) { return __s.__data; }

  static _Simd __min(_Simd __a, _Simd __b) noexcept { return {std::min(__a.__data, __b.__data)}; }

  static _Simd __max(_Simd __a, _Simd __b) noexcept { return {std::max(__a.__data, __b.__data)}; }

  static std::pair<_Simd, _Simd> __minmax(_Simd __a, _Simd __b) noexcept {
    auto [__min, __max] = std::minmax(__a.__data, __b.__data);
    return {__min, __max};
  }

  static _Simd __clamp(_Simd __v, _Simd __lo, _Simd __hi) noexcept {
    return {std::clamp(__v.__data, __lo.__data, __hi.__data)};
  }

  static _Simd __masked_unary_minus(_Simd __s, _Mask __m) noexcept { return {__m.__data ? -__s.__data : __s.__data}; }

  static _Simd __masked_assign(_Simd& __s, _Mask __m, _Tp __v) noexcept { __s.__data = __m.__data ? __v : __v; }

#define _LIBCXX_MASKED_OP_(__op, __name)                                  \
  static void __masked##__name(_Simd&__s, _Mask __m, _Tp __v) noexcept {  \
    __s.__data = __m.__data ? __s.__data __op __v : __s.__data;           \
  }
  _LIBCXX_MASKED_OP_(+, _plus)
  _LIBCXX_MASKED_OP_(-, _minus)
  _LIBCXX_MASKED_OP_(*, _multiplues)
  _LIBCXX_MASKED_OP_(/, _divides)
  _LIBCXX_MASKED_OP_(%, _modulus)
  _LIBCXX_MASKED_OP_(&, _bit_and)
  _LIBCXX_MASKED_OP_(|, _bit_or)
  _LIBCXX_MASKED_OP_(^, _bit_xor)
  _LIBCXX_MASKED_OP_(<<, _shift_left)
  _LIBCXX_MASKED_OP_(>>, _shift_right)
#undef _LIBCXX_MASKED_OP_
  static void __masked_incre(_Simd& __s, _Mask __m) noexcept {
    __s.__data = __m.__data ? __s.__data++ : __s.__data;
  }

  static void __masked_decre(_Simd& __s, _Mask __m) noexcept {
    __s.__data = __m.__data ? __s.__data-- : __s.__data;
  }
  template<class _Up>
  static void __masked_load(_Simd& __s, _Mask __m, _Up* __mem) noexcept {
    __s.__data = __m.__data ? static_cast<_Tp>(__mem) : __s.__data;
  }
 template<class _Up>
 static void __masked_store(const _Simd& __s,  _Mask __m, _Up* __mem) noexcept {
    __mem = __m.__data ? static_cast<_Up>(__s.__data) : __mem;
  }

};

template <class _Tp>
struct __mask_traits<_Tp, simd_abi::__scalar> {
  using _Mask = __mask_storage<_Tp, simd_abi::__scalar>;

  static _Mask __broadcast(_Tp __v) noexcept { return {__v}; }

  template <class _Up, class _Flags>
  static _Mask __load(_Up* __mem, _Flags) noexcept {
    return {static_cast<_Tp>(*__mem)};
  }

  template <class _Up, class _Flags>
  static void __store(_Mask __s, const _Up* __mem, _Flags) noexcept {
    *__mem = static_cast<_Up>(__s.__data);
  }

  static _Mask __logical_and(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data && __rhs.__data}}; }

  static _Mask __logical_or(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data || __rhs.__data}}; }

  static _Mask __bitwise_and(_Mask __lhs, _Mask __rhs) noexcept {
    return {{static_cast<bool>(__lhs.__data & __rhs.__data)}};
  }

  static _Mask __bitwise_or(_Mask __lhs, _Mask __rhs) noexcept {
    return {{static_cast<bool>(__lhs.__data | __rhs.__data)}};
  }

  static _Mask __bitwise_xor(_Mask __lhs, _Mask __rhs) noexcept {
    return {{static_cast<bool>(__lhs.__data ^ __rhs.__data)}};
  }

  static _Mask __equal_to(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data == __rhs.__data}}; }

  static _Mask __not_equal_to(_Mask __lhs, _Mask __rhs) noexcept { return {{__lhs.__data != __rhs.__data}}; }

  static bool __all_of(_Mask __s) noexcept { return __s.__data; }

  static bool __any_of(_Mask __s) noexcept { return __s.__data; }

  static bool __none_of(_Mask __s) noexcept { return !__s.__data; }

  static bool __some_of(_Mask) noexcept { return false; }

  static int __popcount(_Mask __s) noexcept { return __s.__data; }

  static int __find_first_set(_Mask) { return 0; }

  static int __find_last_set(_Mask) { return 0; }

  static _Mask __masked_unary_minus(_Mask __s, _Mask __m) noexcept {
    return __m.__data ? -__s.__data : __s.__data;
  }

  static _Mask __masked_assign(_Mask& __s, _Mask __m, _Tp __v) noexcept {
    return __m.__data ? __v : __s.__data;
  }
#define _LIBCXX_MASKED_OP_M(__op, __name)                                 \
  static void __masked##__name(_Mask&__s, _Mask __m, _Tp __v) noexcept {  \
    __s.__data = __m.__data ? __s.__data __op __v : __s.__data;           \
  }
  _LIBCXX_MASKED_OP_M(+, _plus)
  _LIBCXX_MASKED_OP_M(-, _minus)
  _LIBCXX_MASKED_OP_M(*, _multiplues)
  _LIBCXX_MASKED_OP_M(/, _divides)
  _LIBCXX_MASKED_OP_M(%, _modulus)
  _LIBCXX_MASKED_OP_M(&, _bit_and)
  _LIBCXX_MASKED_OP_M(|, _bit_or)
  _LIBCXX_MASKED_OP_M(^, _bit_xor)
  _LIBCXX_MASKED_OP_M(<<, _shift_left)
  _LIBCXX_MASKED_OP_M(>>, _shift_right)
#undef _LIBCXX_MASKED_OP_M
  static void __masked_incre(_Mask& __s, _Mask __m) noexcept {
    __s.__data = __m.__data ? __s.__data++ : __s.__data;
  }
  static void __masked_decre(_Mask& __s, _Mask __m) noexcept {
    __s.__data = __m.__data ? __s.__data-- : __s.__data;
  }
  template<class _Up>
  static void __masked_load(_Mask& __s, _Mask __m, _Up* __mem) noexcept {
    __s.__data = __m.__data ?static_cast<_Tp>(__mem) : __s.__data;
  }
 template<class _Up>
 static void __masked_store(const _Mask& __s, _Mask __m, _Up* __mem) noexcept {
    __mem = __m.__data ? static_cast<_Up>(__s.__data) : __mem;
  }

};

_LIBCPP_END_NAMESPACE_EXPERIMENTAL_SIMD

#endif // _LIBCPP___SIMD_ABI_SCALAR_H
