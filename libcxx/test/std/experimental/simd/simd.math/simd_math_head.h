#pragma once

#include <experimental/simd>
#include "test_tool.h"
#include <cmath>
#include <iomanip>
#include <inttypes.h>

#define abi_size 31;

using namespace std;

namespace ex = std::experimental::parallelism_v2;

template <typename T> using simd = ex::simd<T, ex::simd_abi::fixed_size<32>>;
template <typename T> using simd_mask = ex::simd_mask<T, ex::simd_abi::fixed_size<32>>;
// template <typename T> using simd = ex::native_simd<T>;


template<typename T1, typename T2,
    simd<T2> (*llvm_func)(simd<T1>), T2 (*vml_func)(T1)>
void llvm_test_single(int len, const T1* p_src, T2* p_dst){
  int i_increase = simd<T1>::size();
  simd<T1> p_srcv;
  simd<T2> p_dstv;
  const T1* p_srci = p_src;
  T2* p_dsti = p_dst;
  int i = 0;
  for (; i < len-i_increase; i+=i_increase)
  {
    p_srcv.copy_from(&p_srci[i], ex::element_aligned);
    p_dstv = llvm_func(p_srcv);
    p_dstv.copy_to(&p_dsti[i], ex::element_aligned);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_srci[i]);
  }
}

template<typename T1, typename T2,
    simd_mask<T1> (*llvm_func)(simd<T1>), bool (*vml_func)(T1)>
void llvm_test_single_bool(int len, const T1* p_src, T2* p_dst){
  int i_increase = simd<T1>::size();
  simd<T1> p_srcv;
  simd_mask<T1> p_dstv;
  const T1* p_srci = p_src;
  T2* p_dsti = p_dst;
  int i = 0;
  for (; i < len-i_increase; i+=i_increase)
  {
    p_srcv.copy_from(&p_srci[i], ex::element_aligned);
    p_dstv = llvm_func(p_srcv);
    p_dstv.copy_to(&p_dsti[i], ex::element_aligned);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_srci[i]);
  }
}


template<typename T1, typename T2,
  ex::samesize<T2, simd<T1>> (*llvm_func)(simd<T1>), T2 (*vml_func)(T1)>
void llvm_test_single_for_ilogb(int len, const T1* p_src, T2* p_dst){

  int i_increase = simd<T1>::size();
  simd<T1> p_srcv;
  ex::samesize<T2, simd<T1>> p_dstv;
  const T1* p_srci = p_src;
  T2* p_dsti = p_dst;
  int i = 0;
  for (; i < len-i_increase; i+=i_increase)
  {
    p_srcv.copy_from(&p_srci[i], ex::element_aligned);
    p_dstv = llvm_func(p_srcv);
    p_dstv.copy_to(&p_dsti[i], ex::element_aligned);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_srci[i]);
  }
}

template<typename T1, typename T2, 
    T2 (*vml_func)(T1)>
void vml_test_single(int len, const T1* p_src, T2* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src[i]);
  }
}


template<typename T1, typename T2, typename T3,
    simd<T3> (*llvm_func)(simd<T1>, simd<T2>),  T3 (*vml_func)(T1, T2)>
void llvm_test_in2out1(int len, const T1* p_src1, T2* p_src2, T3* p_dst){
  // double copy_from_time = 0.0f;
  // double copy_to_time = 0.0f;
  // double vml_f_time = 0.0f;
  int i = 0;
  int i_increase = simd<T1>::size();
  const T1* p_src1i = p_src1;
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  simd<T3> p_dstv;
  for (; i < len-i_increase; i+=i_increase)
  {
    // struct timeval tvs_from, tve_from;
    // gettimeofday(&tvs_from, NULL);
    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2[i], ex::element_aligned);
    // gettimeofday(&tve_from, NULL);
    // copy_from_time += calc_time(tvs_from, tve_from);

    // struct timeval tvs, tve;
    // gettimeofday(&tvs, NULL);
    p_dstv = llvm_func(p_srcv1, p_srcv2);
    // gettimeofday(&tve, NULL);
    // vml_f_time += calc_time(tvs, tve);


    // struct timeval tvs_to, tve_to;
    // gettimeofday(&tvs_to, NULL);
    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
    // gettimeofday(&tve_to, NULL);
    // copy_to_time += calc_time(tvs_to, tve_to);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1i[i], p_src2[i]);
  }
  // printf("\tcopy_from: %f\n\tfc: %f\n\tcopy_to: %f\n", copy_from_time, vml_f_time, copy_to_time);
}

template<typename T1, typename T2, typename T3,
    T3 (*vml_func)(T1, T2)>
void vml_test_in2out1(int len, const T1* p_src1, T2* p_src2, T3* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src1[i], p_src2[i]);
  }
}


template<typename T1, typename T2, typename T3,
    simd_mask<T1> (*llvm_func)(simd<T1>, simd<T2>),  T3 (*vml_func)(T1, T2)>
void llvm_test_in2out1_for_bool(int len, const T1* p_src1, T2* p_src2, T3* p_dst){
  // double copy_from_time = 0.0f;
  // double copy_to_time = 0.0f;
  // double vml_f_time = 0.0f;
  int i = 0;
  int i_increase = simd<T1>::size();
  const T1* p_src1i = p_src1;
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  simd_mask<T1> p_dstv;
  for (; i < len-i_increase; i+=i_increase)
  {
    // struct timeval tvs_from, tve_from;
    // gettimeofday(&tvs_from, NULL);
    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2[i], ex::element_aligned);
    // gettimeofday(&tve_from, NULL);
    // copy_from_time += calc_time(tvs_from, tve_from);

    // struct timeval tvs, tve;
    // gettimeofday(&tvs, NULL);
    p_dstv = llvm_func(p_srcv1, p_srcv2);
    // gettimeofday(&tve, NULL);
    // vml_f_time += calc_time(tvs, tve);


    // struct timeval tvs_to, tve_to;
    // gettimeofday(&tvs_to, NULL);
    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
    // gettimeofday(&tve_to, NULL);
    // copy_to_time += calc_time(tvs_to, tve_to);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1i[i], p_src2[i]);
  }
  // printf("\tcopy_from: %f\n\tfc: %f\n\tcopy_to: %f\n", copy_from_time, vml_f_time, copy_to_time);
}


template<typename T1, typename T2, typename T3,
    simd<T3> (*llvm_func)(simd<T1>, simd<T2>*), T3 (*vml_func)(T1, T2*)>
void llvm_test_in2out1_2(int len, const T1* p_src1, T2* p_src2, T3* p_dst){
  // double copy_from_time = 0.0f;
  // double copy_to_time = 0.0f;
  // double vml_f_time = 0.0f;
  int i = 0;
  int i_increase = simd<T1>::size();
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  simd<T3> p_dstv;

  const T1* p_src1i = p_src1;
  T2* p_src2i = p_src2;

  for (; i < len-i_increase; i+=i_increase)
  {
    // struct timeval tvs_from, tve_from;
    // gettimeofday(&tvs_from, NULL);
    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2i[i], ex::element_aligned);
    // gettimeofday(&tve_from, NULL);
    // copy_from_time += calc_time(tvs_from, tve_from);

    // struct timeval tvs, tve;
    // gettimeofday(&tvs, NULL);
    p_dstv = llvm_func(p_srcv1, &p_srcv2);
    // gettimeofday(&tve, NULL);
    // vml_f_time += calc_time(tvs, tve);


    // struct timeval tvs_to, tve_to;
    // gettimeofday(&tvs_to, NULL);
    p_srcv2.copy_to(&p_src2[i], ex::element_aligned);
    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
    // gettimeofday(&tve_to, NULL);
    // copy_to_time += calc_time(tvs_to, tve_to);
  }
  // printf("\tcopy_from: %f\n\tfc: %f\n\tcopy_to: %f\n", copy_from_time, vml_f_time, copy_to_time);
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1i[i], &p_src2i[i]);
  }
}

template<typename T1, typename T2, typename T3,
    T3 (*vml_func)(T1, T2*)>
void vml_test_in2out1_2(int len, const T1* p_src1, T2* p_src2, T3* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src1[i], &p_src2[i]);
  }
}



/***************************** whz **********************************/
template<typename T1, typename T2, typename T3, typename T4,
    T4 (*vml_func)(T1, T2, T3)>
void vml_test_in3out1(int len, const T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src1[i], p_src2[i], p_src3[i]);
  }
}

template<typename T1, typename T2, typename T3, typename T4,
    simd<T4> (*llvm_func)(simd<T1>, simd<T2>, simd<T3>),  T4 (*vml_func)(T1, T2, T3)>
void llvm_test_in3out1(int len, const T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst){
  int i = 0;
  int i_increase = simd<T1>::size();
  const T1* p_src1i = p_src1;
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  simd<T3> p_srcv3;
  simd<T4> p_dstv;
  for (; i < len-i_increase; i+=i_increase)
  {
    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2[i], ex::element_aligned);
    p_srcv3.copy_from(&p_src3[i], ex::element_aligned);

    p_dstv = llvm_func(p_srcv1, p_srcv2, p_srcv3);

    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1i[i], p_src2[i], p_src3[i]);
  }
}
template<typename T1, typename T2, typename T3, typename T4,
    simd<T4> (*llvm_func)(simd<T1>, simd<T2>, ex::samesize<T3, simd<T1>>*),  T4 (*vml_func)(T1, T2, T3*)>
void llvm_test_in3out1_2(int len, T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst){
  // double copy_from_time = 0.0f;
  // double copy_to_time = 0.0f;
  // double vml_f_time = 0.0f;
  int i = 0;
  int i_increase = simd<T1>::size();
  const T1* p_src1i = p_src1;
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  ex::samesize<T3, simd<T1>> p_srcv3;
  simd<T4> p_dstv;
  for (; i < len-i_increase; i+=i_increase)
  {
    // struct timeval tvs_from, tve_from;
    // gettimeofday(&tvs_from, NULL);
    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2[i], ex::element_aligned);
    p_srcv3.copy_from(&p_src3[i], ex::element_aligned);
    // gettimeofday(&tve_from, NULL);
    // copy_from_time += calc_time(tvs_from, tve_from);

    // printf("%d, %d\n", i, i_increase);
    

    // struct timeval tvs, tve;
    // gettimeofday(&tvs, NULL);
    p_dstv = llvm_func(p_srcv1, p_srcv2, &p_srcv3);
    // gettimeofday(&tve, NULL);
    // vml_f_time += calc_time(tvs, tve);

    // if(i == 17696)
    // {
    //   for(int j = 0; j < i_increase; j++)
    //   {
    //     printf("x: %lf, y: %lf, res: %lf\n", (double)p_srcv1[i+j], (double)p_srcv2[i+j], (double)p_dstv[i+j]);
    //   }
    // }


    // struct timeval tvs_to, tve_to;
    // gettimeofday(&tvs_to, NULL);
    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
    p_srcv3.copy_to(&p_src3[i], ex::element_aligned);
    // gettimeofday(&tve_to, NULL);
    // copy_to_time += calc_time(tvs_to, tve_to);
  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1[i], p_src2[i], &p_src3[i]);
  }
  // std::cout << "vmlout" << p_dstv[0] << " " << p_dst[0] << endl;
  // getchar();
  // printf("\tcopy_from: %f\n\tfc: %f\n\tcopy_to: %f\n", copy_from_time, vml_f_time, copy_to_time);
}

template<typename T1, typename T2, typename T3, typename T4,
    T4 (*vml_func)(T1, T2, T3*)>
void vml_test_in3out1_2(int len, T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src1[i], p_src2[i], &p_src3[i]);
  }
  // std::cout << setprecision(20) <<  p_dst[9589] << endl;
  // getchar();
}


template<typename T1, typename T2, typename T3, typename T4,
    T4 (*vml_func)(T1, T2, T3*)>
void ssss(int len, T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst){
  for (int i = 0; i < len; i++)
  {
    p_dst[i] = vml_func(p_src1[i], p_src2[i], &p_src3[i]);
  }
  // std::cout << setprecision(20) <<  p_dst[9589] << endl;
  // getchar();
}

template<typename T1, typename T2, typename T3,
    simd<T3> (*llvm_func)(simd<T1>, simd<T2>*)>
void llvm_test_in1out2(int len, T1* p_src, T2* p_dst1, T3* p_dst2){
  int i_increase = simd<T1>::size();
  for (int i = 0; i < len; i+=i_increase)
  {
    simd<T1> p_srcv(&p_src[i], ex::element_aligned);
    simd<T2> p_dstv1;
    simd<T3> p_dstv2 = llvm_func(p_srcv, &p_dstv1);
    p_dstv1.copy_to(&p_dst1[i], ex::element_aligned);
    p_dstv2.copy_to(&p_dst2[i], ex::element_aligned);
  }
}

template<typename T1, typename T2, typename T3,
    T3 (*vml_func)(T1, T2*)>
void vml_test_in1out2(int len, T1* p_src, T2* p_dst1, T3* p_dst2){
  for (int i = 0; i < len; i++)
  {
    p_dst2[i] = vml_func(p_src[i], &p_dst1[i]);
  }
}
