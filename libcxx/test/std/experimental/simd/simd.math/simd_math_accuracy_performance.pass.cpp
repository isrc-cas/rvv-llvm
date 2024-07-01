#include <experimental/simd>
#include "test_tool.h"
#include <cmath>
#include <iomanip>

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

// add by me


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
void llvm_test_in3out1_2(int len, T1* p_src1, T2* p_src2, T3* p_src3, T4* p_dst)
{
  int i = 0;
  int i_increase = simd<T1>::size();
  const T1* p_src1i = p_src1;
  simd<T1> p_srcv1;
  simd<T2> p_srcv2;
  ex::samesize<T3, simd<T1>> p_srcv3;
  simd<T4> p_dstv;
  for (; i < len-i_increase; i+=i_increase)
  {

    p_srcv1.copy_from(&p_src1i[i], ex::element_aligned);
    p_srcv2.copy_from(&p_src2[i], ex::element_aligned);
    p_srcv3.copy_from(&p_src3[i], ex::element_aligned);

    p_dstv = llvm_func(p_srcv1, p_srcv2, &p_srcv3);

    p_dstv.copy_to(&p_dst[i], ex::element_aligned);
    p_srcv3.copy_to(&p_src3[i], ex::element_aligned);

  }
  for (; i < len; i++){
    p_dst[i] = vml_func(p_src1[i], p_src2[i], &p_src3[i]);
  }
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


int main() {
  {
    cout << "\n" << "***Sin long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sin, sin>, vml_test_single<long double, long double, sin>>(1);
  }

  {
    cout << "\n" << "***Cos long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cos, cos>, vml_test_single<long double, long double, cos>>(1);
  }

  {
    cout << "\n" << "***Tan long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tan, tan>, vml_test_single<long double, long double, tan>>(1);
  }

  {
    cout << "\n" << "***Asin double***" << "\n" << endl;
    test_accuracy_for_single<double, double, llvm_test_single<double, double, ex::asin, asin>, vml_test_single<double, double, asin>>(1);

    cout << "\n" << "***Asin long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::asin, asin>, vml_test_single<long double, long double, asin>>(1);
  }

  {
    cout << "\n" << "***Atan float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::atan, atan>, vml_test_single<float, float, atan>>(1);
    cout << "\n" << "***Atan double***" << "\n" << endl;
    test_accuracy_for_single< double, double, llvm_test_single< double, double, ex::atan, atan>, vml_test_single< double, double, atan>>(1);
    cout << "\n" << "***Atan long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);
  }

  {
    cout << "\n" << "***Atan2 float***" << "\n" << endl;
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::atan2, atan2>, vml_test_in2out1<float, float, float, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::atan2, atan2>, vml_test_in2out1<double, double, double, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
  }

  {
    cout << "\n" << "***Exp long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long  double, ex::exp, exp>, vml_test_single<long double, long double, exp>>(1);
  }

  {
    cout << "\n" << "***Exp2 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::exp2, exp2>, vml_test_single<long double, long double, exp2>>(1);
  }

  {
    cout << "\n" << "***Log long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log, log>, vml_test_single<long double, long double, log>>(1);
  }

  {
    cout << "\n" << "***Log10 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log10, log10>, vml_test_single<long double, long double, log10>>(1);
  }

  {
    cout << "\n" << "***Log2 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log2, log2>, vml_test_single<long double, long double, log2>>(1);
  }

  {
    cout << "\n" << "***Log1p long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log1p, log1p>, vml_test_single<long double, long double, log1p>>(1);
  }

  {
    cout << "\n" << "***Logb long double***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::logb, logb>, vml_test_single<float, float, logb>>(1);
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::logb, logb>, vml_test_single<double, double, logb>>(1);
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
  }

  {
    cout << "\n" << "***Ilogb long double***" << "\n" << endl;
    test_accuracy_for_single<long double,int, llvm_test_single_for_ilogb<long double, int, ex::ilogb, ilogb>, vml_test_single<long double, int, ilogb>>(1);
  }

  {
    cout << "\n" << "***Pow float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::pow, pow>, vml_test_in2out1<float, float, float, pow>>(1);
    cout << "\n" << "***Pow double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::pow, pow>,  vml_test_in2out1<double, double, double, pow>>(1);
    cout << "\n" << "***Pow long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::pow, pow>, vml_test_in2out1<long double, long double, long double, pow>>(1);
  }

  {
    cout << "\n" << "***Sqrt long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sqrt, sqrt>, vml_test_single<long double, long double, sqrt>>(1);
  }

  {
    cout << "\n" << "***Cbrt float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::cbrt, cbrt>, vml_test_single<float, float, cbrt>>(1);
    cout << "\n" << "***Cbrt double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::cbrt, cbrt>, vml_test_single<double, double, cbrt>>(1);
    cout << "\n" << "***Cbrt long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);
  }

  {
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::hypot, hypot>, vml_test_in2out1<float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::hypot, hypot>, vml_test_in2out1<double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);
  
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_accuracy_for_in3out1<float, float, float, float, llvm_test_in3out1<float, float, float, float, ex::hypot, hypot>, vml_test_in3out1<float, float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_accuracy_for_in3out1<double, double, double, double, llvm_test_in3out1<double, double, double, double, ex::hypot, hypot>, vml_test_in3out1<double, double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_accuracy_for_in3out1<long double, long double, long double, long double, llvm_test_in3out1<long double, long double, long double, long double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
  }

  {
    cout << "\n" << "***Expm1 float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::expm1, expm1>, vml_test_single<float, float, expm1>>(1);
    cout << "\n" << "***Expm1 double***" << "\n" << endl;
    test_accuracy_for_single<double, double, llvm_test_single<double, double, ex::expm1, expm1>, vml_test_single<double, double, expm1>>(1);
    cout << "\n" << "***Expm1 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);
  }

  {
    cout << "\n" << "***Sinh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sinh, sinh>, vml_test_single<long double, long double, sinh>>(1);
  }

  {
    cout << "\n" << "***Cosh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cosh, cosh>, vml_test_single<long double, long double, cosh>>(1);
  }

  {
    cout << "\n" << "***Tanh float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::tanh, tanh>, vml_test_single<float, float, tanh>>(1);
    cout << "\n" << "***Tanh double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::tanh, tanh>, vml_test_single<double, double, tanh>>(1);
    cout << "\n" << "***Tanh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);
  }

  {
    cout << "\n" << "***Asinh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::asinh, asinh>, vml_test_single<long double, long double, asinh>>(1);
  }

  {
    cout << "\n" << "***Acosh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::acosh, acosh>, vml_test_single<long double, long double, acosh>>(1);
  }

  {
    cout << "\n" << "***Atanh float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::atanh, atanh>, vml_test_single<float, float, atanh>>(1);
    cout << "\n" << "***Atanh double***" << "\n" << endl;
    test_accuracy_for_single<double, double, llvm_test_single<double, double, ex::atanh, atanh>, vml_test_single<double, double, atanh>>(1);
    cout << "\n" << "***Atanh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1);
  }

  {
    cout << "\n" << "***Erf float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::erf, erf>, vml_test_single<float, float, erf>>(1);
    cout << "\n" << "***Erf double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::erf, erf>, vml_test_single<double, double, erf>>(1);
    cout << "\n" << "***Erf long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
  }

  {
    cout << "\n" << "***Erfc float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::erfc, erfc>, vml_test_single<float, float, erfc>>(1);
    cout << "\n" << "***Erfc double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::erfc, erfc>, vml_test_single<double, double, erfc>>(1);
    cout << "\n" << "***Erfc long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
  }

  {
    cout << "\n" << "***Lgamma float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::lgamma, lgamma>, vml_test_single<float, float, lgamma>>(1);
    cout << "\n" << "***Lgamma double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::lgamma, lgamma>, vml_test_single<double, double, lgamma>>(1);
    cout << "\n" << "***Lgamma long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);
  }

  {
    cout << "\n" << "***Tgamma float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::tgamma, tgamma>, vml_test_single<float, float, tgamma>>(1);
    cout << "\n" << "***Tgamma double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::tgamma, tgamma>, vml_test_single<double, double, tgamma>>(1);
    cout << "\n" << "***Tgamma long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);
  }

  {
    cout << "\n" << "***Frexp long double***" << "\n" << endl;
    test_accuracy_for_in2out1_2<long double, int, long double, llvm_test_in2out1_2<long double, int, long double, ex::frexp, frexp>, vml_test_in2out1_2<long double, int, long double, frexp>>(1);
  }

  {
    cout << "\n" << "***Ldexp long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::ldexp, ldexp>, vml_test_in2out1<long double, int, long double, ldexp>>(1);
  }

  { 
    cout << "\n" << "***Scalbn float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,int,float, llvm_test_in2out1<float, int, float, ex::scalbn, scalbn>, vml_test_in2out1<float, int, float, scalbn>>(1);
    cout << "\n" << "***Scalbn double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,int,double, llvm_test_in2out1<double, int, double, ex::scalbn, scalbn>, vml_test_in2out1<double, int, double, scalbn>>(1);
    cout << "\n" << "***Scalbn long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,int,long double, llvm_test_in2out1<long double, int, long double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);
  }

  { 
    cout << "\n" << "***Scalbln float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,long int,float, llvm_test_in2out1<float, long int, float, ex::scalbln, scalbln>, vml_test_in2out1<float, long int, float, scalbln>>(1);
    cout << "\n" << "***Scalbln double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,long int,double, llvm_test_in2out1<double, long int, double, ex::scalbln, scalbln>, vml_test_in2out1<double, long int, double, scalbln>>(1);
    cout << "\n" << "***Scalbln long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,long int ,long double, llvm_test_in2out1<long double, long int, long double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);
  }

  {
    cout << "\n" << "***Nearbyint float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::nearbyint, nearbyint>, vml_test_single<float, float, nearbyint>>(1);
    cout << "\n" << "***Nearbyint double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::nearbyint, nearbyint>, vml_test_single<double, double, nearbyint>>(1);
    cout << "\n" << "***Nearbyint long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);
  }

  
  {
    cout << "\n" << "***Rint float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::rint, rint>, vml_test_single<float, float, rint>>(1);
    cout << "\n" << "***Rint double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::rint, rint>, vml_test_single<double, double, rint>>(1);
    cout << "\n" << "***Rint long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);
  }

  {
    cout << "\n" << "***Lrint float***" << "\n" << endl;
    test_accuracy_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lrint, lrint>, vml_test_single<float, long int, lrint>>(1);
    cout << "\n" << "***Lrint double***" << "\n" << endl;
    test_accuracy_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lrint, lrint>, vml_test_single<double, long int, lrint>>(1);
    cout << "\n" << "***Lrint long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lrint, lrint>, vml_test_single<long double, long int, lrint>>(1);
  }

  {
    cout << "\n" << "***Llrint float***" << "\n" << endl;
    test_accuracy_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llrint, llrint>, vml_test_single<float, long long int, llrint>>(1);
    cout << "\n" << "***Llrint double***" << "\n" << endl;
    test_accuracy_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llrint, llrint>, vml_test_single<double, long long int, llrint>>(1);
    cout << "\n" << "***Llrint long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llrint, llrint>, vml_test_single<long double, long long int, llrint>>(1);
  }

  {
    cout << "\n" << "***Lround float***" << "\n" << endl;
    test_accuracy_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lround, lround>, vml_test_single<float, long int, lround>>(1);
    cout << "\n" << "***Lround double***" << "\n" << endl;
    test_accuracy_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lround, lround>, vml_test_single<double, long int, lround>>(1);
    cout << "\n" << "***Lround long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lround, lround>, vml_test_single<long double, long int, lround>>(1);
  }

  {
    cout << "\n" << "***Llround float***" << "\n" << endl;
    test_accuracy_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llround, llround>, vml_test_single<float, long long int, llround>>(1);
    cout << "\n" << "***Llround double***" << "\n" << endl;
    test_accuracy_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llround, llround>, vml_test_single<double, long long int, llround>>(1);
    cout << "\n" << "***Llround long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llround, llround>, vml_test_single<long double, long long int, llround>>(1);
  }

  {
    cout << "\n" << "***Trunc float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::trunc, trunc>, vml_test_single<float, float, trunc>>(1);
    cout << "\n" << "***Trunc double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::trunc, trunc>, vml_test_single<double, double, trunc>>(1);
    cout << "\n" << "***Trunc long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long double, llvm_test_single<long double, long double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);
  }

  {
    cout << "\n" << "***Remquo float***" << "\n" << endl;
    test_accuracy_for_in3out1_2<float, float, int, float, llvm_test_in3out1_2<float, float, int, float, ex::remquo, remquo>, vml_test_in3out1_2<float, float, int, float, remquo>>(1);
    cout << "\n" << "***Remquo double***" << "\n" << endl;
    test_accuracy_for_in3out1_2<double, double, int, double, llvm_test_in3out1_2<double, double, int, double, ex::remquo, remquo>, vml_test_in3out1_2<double, double, int, double, remquo>>(1);
    cout << "\n" << "***Remquo long double***" << "\n" << endl;
    test_accuracy_for_in3out1_2<long double, long double, int, long double, llvm_test_in3out1_2<long double, long double, int, long double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);
  }

  {
    cout << "\n" << "***Copysign float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::copysign, copysign>, vml_test_in2out1<float, float, float, copysign>>(1);
    cout << "\n" << "***Copysign double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::copysign, copysign>, vml_test_in2out1<double, double, double, copysign>>(1);
    cout << "\n" << "***Copysign long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
  }

  // {
  //    nan, nanf, nanl
  // }

  { 
    cout << "\n" << "***Nextafter float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::nextafter, nextafter>, vml_test_in2out1<float, float, float, nextafter>>(1);
    cout << "\n" << "***Nextafter double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::nextafter, nextafter>, vml_test_in2out1<double, double, double, nextafter>>(1);
    cout << "\n" << "***Nextafter long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);
  }

  {
    cout << "\n" << "***Nexttoward float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,long double, float, llvm_test_in2out1<float, long double, float, ex::nexttoward, nexttoward>, vml_test_in2out1<float, long double, float, nexttoward>>(1);
    cout << "\n" << "***Nexttoward double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double, long double, double, llvm_test_in2out1<double, long double, double, ex::nexttoward, nexttoward>, vml_test_in2out1<double, long double, double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::nexttoward, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
  }

  {
    cout << "\n" << "***Fdim float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::fdim, fdim>, vml_test_in2out1<float, float, float, fdim>>(1);
    cout << "\n" << "***Fdim double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::fdim, fdim>, vml_test_in2out1<double, double, double, fdim>>(1);
    cout << "\n" << "***Fdim long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);
  }

  {
    cout << "\n" << "***Fpclassify float***" << "\n" << endl; // pass
    test_accuracy_for_single<float,int, llvm_test_single_for_ilogb<float, int, ex::fpclassify, fpclassify>,  vml_test_single<float, int, fpclassify>>(1);
    cout << "\n" << "***Fpclassify double***" << "\n" << endl; // pass
    test_accuracy_for_single<double,int, llvm_test_single_for_ilogb<double, int, ex::fpclassify, fpclassify>,  vml_test_single<double, int, fpclassify>>(1);
    cout << "\n" << "***Fpclassify long double***" << "\n" << endl; // pass
    test_accuracy_for_single<long double, int, llvm_test_single_for_ilogb<long double, int, ex::fpclassify, fpclassify>, vml_test_single<long double, int, fpclassify>>(1);
  }

  
  {
    cout << "\n" << "***Signbit float***" << "\n" << endl; // pass
    test_accuracy_for_single<float,bool, llvm_test_single_bool<float, bool, ex::signbit, signbit>, vml_test_single<float, bool, signbit>>(1);
    cout << "\n" << "***Signbit double***" << "\n" << endl; // pass
    test_accuracy_for_single<double,bool, llvm_test_single_bool<double, bool, ex::signbit, signbit>, vml_test_single<double, bool, signbit>>(1);
    cout << "\n" << "***Signbit long double***" << "\n" << endl; // pass
    test_accuracy_for_single<long double, bool, llvm_test_single_bool<long double, bool, ex::signbit, signbit>, vml_test_single<long double, bool, signbit>>(1);
  }

  {
    cout << "\n" << "***Isgreater float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreater, isgreater>, vml_test_in2out1<float, float, bool, isgreater>>(1);
    cout << "\n" << "***Isgreater double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreater, isgreater>, vml_test_in2out1<double, double, bool, isgreater>>(1);
    cout << "\n" << "***Isgreater long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreater, isgreater>, vml_test_in2out1<long double, long double, bool, isgreater>>(1);
  }

  {
    cout << "\n" << "***Isgreaterequal float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<float, float, bool, isgreaterequal>>(1);
    cout << "\n" << "***Isgreaterequal double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<double, double, bool, isgreaterequal>>(1);
    cout << "\n" << "***Isgreaterequal long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double,bool,  llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<long double, long double, bool, isgreaterequal>>(1);
  }
  
  {
    cout << "\n" << "***isless float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isless, isless>, vml_test_in2out1<float, float, bool, isless>>(1);
    cout << "\n" << "***isless double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isless, isless>, vml_test_in2out1<double, double, bool, isless>>(1);
    cout << "\n" << "***isless long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isless, isless>, vml_test_in2out1<long double, long double, bool, isless>>(1);
  }

  {
    cout << "\n" << "***islessequal float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::islessequal, islessequal>, vml_test_in2out1<float, float, bool, islessequal>>(1);
    cout << "\n" << "***islessequal double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double,bool,  llvm_test_in2out1_for_bool<double, double, bool, ex::islessequal, islessequal>, vml_test_in2out1<double, double, bool, islessequal>>(1);
    cout << "\n" << "***islessequal long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessequal, islessequal>, vml_test_in2out1<long double, long double, bool, islessequal>>(1);
  }

  {
    cout << "\n" << "***islessgreater float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float,bool,  llvm_test_in2out1_for_bool<float, float, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<float, float, bool, islessgreater>>(1);
    cout << "\n" << "***islessgreater double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<double, double, bool, islessgreater>>(1);
    cout << "\n" << "***islessgreater long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<long double, long double, bool, islessgreater>>(1);
  }

  {
    cout << "\n" << "***isunordered float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float,bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isunordered, isunordered>, vml_test_in2out1<float, float, bool, isunordered>>(1);
    cout << "\n" << "***isunordered double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double,  bool,ex::isunordered, isunordered>, vml_test_in2out1<double, double, bool, isunordered>>(1);
    cout << "\n" << "***isunordered long double***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool,ex::isunordered, isunordered>, vml_test_in2out1<long double, long double, bool, isunordered>>(1);
  }



  /********************************** performance ***********************************/
  {
    cout << "\n" << "***Sin long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::sin, sin>, vml_test_single<long double, long double, sin>>(1);
  }

  {
    cout << "\n" << "***Cos long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::cos, cos>, vml_test_single<long double, long double, cos>>(1);
  }

  {
    cout << "\n" << "***Tan long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::tan, tan>, vml_test_single<long double, long double, tan>>(1);
  }

  {
    cout << "\n" << "***Asin long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::asin, asin>, vml_test_single<long double, long double, asin>>(1);
  }

  {
    cout << "\n" << "***Atan float***" << "\n" << endl;
    test_performance_for_single<float, float, llvm_test_single<float, float, ex::atan, atan>, vml_test_single<float, float, atan>>(1);
    cout << "\n" << "***Atan double***" << "\n" << endl;
    test_performance_for_single< double, double, llvm_test_single< double, double, ex::atan, atan>, vml_test_single< double, double, atan>>(1);
    cout << "\n" << "***Atan long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);
  }

  {
    cout << "\n" << "***Atan2 float***" << "\n" << endl;
    test_performance_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::atan2, atan2>, vml_test_in2out1<float, float, float, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_performance_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::atan2, atan2>, vml_test_in2out1<double, double, double, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_performance_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
  }

  {
    cout << "\n" << "***Exp long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long  double, ex::exp, exp>, vml_test_single<long double, long double, exp>>(1);
  }

  {
    cout << "\n" << "***Exp2 long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::exp2, exp2>, vml_test_single<long double, long double, exp2>>(1);
  }

  {
    cout << "\n" << "***Log long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::log, log>, vml_test_single<long double, long double, log>>(1);
  }

  {
    cout << "\n" << "***Log10 long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::log10, log10>, vml_test_single<long double, long double, log10>>(1);
  }

  {
    cout << "\n" << "***Log2 long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::log2, log2>, vml_test_single<long double, long double, log2>>(1);
  }

  {
    cout << "\n" << "***Log1p long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::log1p, log1p>, vml_test_single<long double, long double, log1p>>(1);
  }

  {
    cout << "\n" << "***Logb long double***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::logb, logb>, vml_test_single<float, float, logb>>(1);
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::logb, logb>, vml_test_single<double, double, logb>>(1);
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
  }

  {
    cout << "\n" << "***Ilogb long double***" << "\n" << endl;
    test_performance_for_single<long double,int, llvm_test_single_for_ilogb<long double, int, ex::ilogb, ilogb>, vml_test_single<long double, int, ilogb>>(1);
  }

  {
    cout << "\n" << "***Pow float***" << "\n" << endl;
    test_performance_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::pow, pow>, vml_test_in2out1<float, float, float, pow>>(1);
    cout << "\n" << "***Pow double***" << "\n" << endl;
    test_performance_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::pow, pow>,  vml_test_in2out1<double, double, double, pow>>(1);
    cout << "\n" << "***Pow long double***" << "\n" << endl;
    test_performance_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::pow, pow>, vml_test_in2out1<long double, long double, long double, pow>>(1);
  }

  {
    cout << "\n" << "***Sqrt long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::sqrt, sqrt>, vml_test_single<long double, long double, sqrt>>(1);
  }

  {
    cout << "\n" << "***Cbrt float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::cbrt, cbrt>, vml_test_single<float, float, cbrt>>(1);
    cout << "\n" << "***Cbrt double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::cbrt, cbrt>, vml_test_single<double, double, cbrt>>(1);
    cout << "\n" << "***Cbrt long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);
  }

  {
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_performance_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::hypot, hypot>, vml_test_in2out1<float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_performance_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::hypot, hypot>, vml_test_in2out1<double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_performance_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);
  
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_performance_for_in3out1<float, float, float, float, llvm_test_in3out1<float, float, float, float, ex::hypot, hypot>, vml_test_in3out1<float, float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_performance_for_in3out1<double, double, double, double, llvm_test_in3out1<double, double, double, double, ex::hypot, hypot>, vml_test_in3out1<double, double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_performance_for_in3out1<long double, long double, long double, long double, llvm_test_in3out1<long double, long double, long double, long double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
  }

  {
    cout << "\n" << "***Expm1 float***" << "\n" << endl;
    test_performance_for_single<float, float, llvm_test_single<float, float, ex::expm1, expm1>, vml_test_single<float, float, expm1>>(1);
    cout << "\n" << "***Expm1 double***" << "\n" << endl;
    test_performance_for_single<double, double, llvm_test_single<double, double, ex::expm1, expm1>, vml_test_single<double, double, expm1>>(1);
    cout << "\n" << "***Expm1 long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);
  }

  {
    cout << "\n" << "***Sinh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::sinh, sinh>, vml_test_single<long double, long double, sinh>>(1);
  }

  {
    cout << "\n" << "***Cosh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::cosh, cosh>, vml_test_single<long double, long double, cosh>>(1);
  }

  {
    cout << "\n" << "***Tanh float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::tanh, tanh>, vml_test_single<float, float, tanh>>(1);
    cout << "\n" << "***Tanh double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::tanh, tanh>, vml_test_single<double, double, tanh>>(1);
    cout << "\n" << "***Tanh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);
  }

  {
    cout << "\n" << "***Asinh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::asinh, asinh>, vml_test_single<long double, long double, asinh>>(1);
  }

  {
    cout << "\n" << "***Acosh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::acosh, acosh>, vml_test_single<long double, long double, acosh>>(1);
  }

  {
    cout << "\n" << "***Atanh float***" << "\n" << endl;
    test_performance_for_single<float, float, llvm_test_single<float, float, ex::atanh, atanh>, vml_test_single<float, float, atanh>>(1);
    cout << "\n" << "***Atanh double***" << "\n" << endl;
    test_performance_for_single<double, double, llvm_test_single<double, double, ex::atanh, atanh>, vml_test_single<double, double, atanh>>(1);
    cout << "\n" << "***Atanh long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1);
  }

  {
    cout << "\n" << "***Erf float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::erf, erf>, vml_test_single<float, float, erf>>(1);
    cout << "\n" << "***Erf double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::erf, erf>, vml_test_single<double, double, erf>>(1);
    cout << "\n" << "***Erf long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
  }

  {
    cout << "\n" << "***Erfc float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::erfc, erfc>, vml_test_single<float, float, erfc>>(1);
    cout << "\n" << "***Erfc double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::erfc, erfc>, vml_test_single<double, double, erfc>>(1);
    cout << "\n" << "***Erfc long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
  }

  {
    cout << "\n" << "***Lgamma float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::lgamma, lgamma>, vml_test_single<float, float, lgamma>>(1);
    cout << "\n" << "***Lgamma double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::lgamma, lgamma>, vml_test_single<double, double, lgamma>>(1);
    cout << "\n" << "***Lgamma long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);
  }

  {
    cout << "\n" << "***Tgamma float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::tgamma, tgamma>, vml_test_single<float, float, tgamma>>(1);
    cout << "\n" << "***Tgamma double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::tgamma, tgamma>, vml_test_single<double, double, tgamma>>(1);
    cout << "\n" << "***Tgamma long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);
  }

  {
    cout << "\n" << "***Frexp long double***" << "\n" << endl;
    test_performance_for_in2out1_2<long double, int, long double, llvm_test_in2out1_2<long double, int, long double, ex::frexp, frexp>, vml_test_in2out1_2<long double, int, long double, frexp>>(1);
  }

  {
    cout << "\n" << "***Ldexp long double***" << "\n" << endl;
    test_performance_for_in2out1<long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::ldexp, ldexp>, vml_test_in2out1<long double, int, long double, ldexp>>(1);
  }

  { 
    cout << "\n" << "***Scalbn float***" << "\n" << endl;
    test_performance_for_in2out1<float,int,float, llvm_test_in2out1<float, int, float, ex::scalbn, scalbn>, vml_test_in2out1<float, int, float, scalbn>>(1);
    cout << "\n" << "***Scalbn double***" << "\n" << endl;
    test_performance_for_in2out1<double,int,double, llvm_test_in2out1<double, int, double, ex::scalbn, scalbn>, vml_test_in2out1<double, int, double, scalbn>>(1);
    cout << "\n" << "***Scalbn long double***" << "\n" << endl;
    test_performance_for_in2out1<long double,int,long double, llvm_test_in2out1<long double, int, long double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);
  }

  { 
    cout << "\n" << "***Scalbln float***" << "\n" << endl;
    test_performance_for_in2out1<float,long int,float, llvm_test_in2out1<float, long int, float, ex::scalbln, scalbln>, vml_test_in2out1<float, long int, float, scalbln>>(1);
    cout << "\n" << "***Scalbln double***" << "\n" << endl;
    test_performance_for_in2out1<double,long int,double, llvm_test_in2out1<double, long int, double, ex::scalbln, scalbln>, vml_test_in2out1<double, long int, double, scalbln>>(1);
    cout << "\n" << "***Scalbln long double***" << "\n" << endl;
    test_performance_for_in2out1<long double,long int ,long double, llvm_test_in2out1<long double, long int, long double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);
  }

  {
    cout << "\n" << "***Nearbyint float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::nearbyint, nearbyint>, vml_test_single<float, float, nearbyint>>(1);
    cout << "\n" << "***Nearbyint double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::nearbyint, nearbyint>, vml_test_single<double, double, nearbyint>>(1);
    cout << "\n" << "***Nearbyint long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);
  }

  
  {
    cout << "\n" << "***Rint float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::rint, rint>, vml_test_single<float, float, rint>>(1);
    cout << "\n" << "***Rint double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::rint, rint>, vml_test_single<double, double, rint>>(1);
    cout << "\n" << "***Rint long double***" << "\n" << endl;
    test_performance_for_single<long double, long double, llvm_test_single<long double, long double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);
  }

  {
    cout << "\n" << "***Lrint float***" << "\n" << endl;
    test_performance_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lrint, lrint>, vml_test_single<float, long int, lrint>>(1);
    cout << "\n" << "***Lrint double***" << "\n" << endl;
    test_performance_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lrint, lrint>, vml_test_single<double, long int, lrint>>(1);
    cout << "\n" << "***Lrint long double***" << "\n" << endl;
    test_performance_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lrint, lrint>, vml_test_single<long double, long int, lrint>>(1);
  }

  {
    cout << "\n" << "***Llrint float***" << "\n" << endl;
    test_performance_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llrint, llrint>, vml_test_single<float, long long int, llrint>>(1);
    cout << "\n" << "***Llrint double***" << "\n" << endl;
    test_performance_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llrint, llrint>, vml_test_single<double, long long int, llrint>>(1);
    cout << "\n" << "***Llrint long double***" << "\n" << endl;
    test_performance_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llrint, llrint>, vml_test_single<long double, long long int, llrint>>(1);
  }

  {
    cout << "\n" << "***Lround float***" << "\n" << endl;
    test_performance_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lround, lround>, vml_test_single<float, long int, lround>>(1);
    cout << "\n" << "***Lround double***" << "\n" << endl;
    test_performance_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lround, lround>, vml_test_single<double, long int, lround>>(1);
    cout << "\n" << "***Lround long double***" << "\n" << endl;
    test_performance_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lround, lround>, vml_test_single<long double, long int, lround>>(1);
  }

  {
    cout << "\n" << "***Llround float***" << "\n" << endl;
    test_performance_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llround, llround>, vml_test_single<float, long long int, llround>>(1);
    cout << "\n" << "***Llround double***" << "\n" << endl;
    test_performance_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llround, llround>, vml_test_single<double, long long int, llround>>(1);
    cout << "\n" << "***Llround long double***" << "\n" << endl;
    test_performance_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llround, llround>, vml_test_single<long double, long long int, llround>>(1);
  }

  {
    cout << "\n" << "***Trunc float***" << "\n" << endl;
    test_performance_for_single<float,float, llvm_test_single<float, float, ex::trunc, trunc>, vml_test_single<float, float, trunc>>(1);
    cout << "\n" << "***Trunc double***" << "\n" << endl;
    test_performance_for_single<double,double, llvm_test_single<double, double, ex::trunc, trunc>, vml_test_single<double, double, trunc>>(1);
    cout << "\n" << "***Trunc long double***" << "\n" << endl;
    test_performance_for_single<long double,long double, llvm_test_single<long double, long double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);
  }

  {
    cout << "\n" << "***Remquo float***" << "\n" << endl;
    test_performance_for_in3out1_2<float, float, int, float, llvm_test_in3out1_2<float, float, int, float, ex::remquo, remquo>, vml_test_in3out1_2<float, float, int, float, remquo>>(1);
    cout << "\n" << "***Remquo double***" << "\n" << endl;
    test_performance_for_in3out1_2<double, double, int, double, llvm_test_in3out1_2<double, double, int, double, ex::remquo, remquo>, vml_test_in3out1_2<double, double, int, double, remquo>>(1);
    cout << "\n" << "***Remquo long double***" << "\n" << endl;
    test_performance_for_in3out1_2<long double, long double, int, long double, llvm_test_in3out1_2<long double, long double, int, long double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);
  }

  {
    cout << "\n" << "***Copysign float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::copysign, copysign>, vml_test_in2out1<float, float, float, copysign>>(1);
    cout << "\n" << "***Copysign double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::copysign, copysign>, vml_test_in2out1<double, double, double, copysign>>(1);
    cout << "\n" << "***Copysign long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
  }

  // {
  //    nan, nanf, nanl
  // }

  { 
    cout << "\n" << "***Nextafter float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::nextafter, nextafter>, vml_test_in2out1<float, float, float, nextafter>>(1);
    cout << "\n" << "***Nextafter double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::nextafter, nextafter>, vml_test_in2out1<double, double, double, nextafter>>(1);
    cout << "\n" << "***Nextafter long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);
  }

  {
    cout << "\n" << "***Nexttoward float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,long double, float, llvm_test_in2out1<float, long double, float, ex::nexttoward, nexttoward>, vml_test_in2out1<float, long double, float, nexttoward>>(1);
    cout << "\n" << "***Nexttoward double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double, long double, double, llvm_test_in2out1<double, long double, double, ex::nexttoward, nexttoward>, vml_test_in2out1<double, long double, double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::nexttoward, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
  }

  {
    cout << "\n" << "***Fdim float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::fdim, fdim>, vml_test_in2out1<float, float, float, fdim>>(1);
    cout << "\n" << "***Fdim double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::fdim, fdim>, vml_test_in2out1<double, double, double, fdim>>(1);
    cout << "\n" << "***Fdim long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);
  }

  {
    cout << "\n" << "***Fpclassify float***" << "\n" << endl; // pass
    test_performance_for_single<float,int, llvm_test_single_for_ilogb<float, int, ex::fpclassify, fpclassify>,  vml_test_single<float, int, fpclassify>>(1);
    cout << "\n" << "***Fpclassify double***" << "\n" << endl; // pass
    test_performance_for_single<double,int, llvm_test_single_for_ilogb<double, int, ex::fpclassify, fpclassify>,  vml_test_single<double, int, fpclassify>>(1);
    cout << "\n" << "***Fpclassify long double***" << "\n" << endl; // pass
    test_performance_for_single<long double, int, llvm_test_single_for_ilogb<long double, int, ex::fpclassify, fpclassify>, vml_test_single<long double, int, fpclassify>>(1);
  }

  
  {
    cout << "\n" << "***Signbit float***" << "\n" << endl; // pass
    test_performance_for_single<float,bool, llvm_test_single_bool<float, bool, ex::signbit, signbit>, vml_test_single<float, bool, signbit>>(1);
    cout << "\n" << "***Signbit double***" << "\n" << endl; // pass
    test_performance_for_single<double,bool, llvm_test_single_bool<double, bool, ex::signbit, signbit>, vml_test_single<double, bool, signbit>>(1);
    cout << "\n" << "***Signbit long double***" << "\n" << endl; // pass
    test_performance_for_single<long double, bool, llvm_test_single_bool<long double, bool, ex::signbit, signbit>, vml_test_single<long double, bool, signbit>>(1);
  }

  {
    cout << "\n" << "***Isgreater float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreater, isgreater>, vml_test_in2out1<float, float, bool, isgreater>>(1);
    cout << "\n" << "***Isgreater double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreater, isgreater>, vml_test_in2out1<double, double, bool, isgreater>>(1);
    cout << "\n" << "***Isgreater long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreater, isgreater>, vml_test_in2out1<long double, long double, bool, isgreater>>(1);
  }

  {
    cout << "\n" << "***Isgreaterequal float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<float, float, bool, isgreaterequal>>(1);
    cout << "\n" << "***Isgreaterequal double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<double, double, bool, isgreaterequal>>(1);
    cout << "\n" << "***Isgreaterequal long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double,bool,  llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<long double, long double, bool, isgreaterequal>>(1);
  }
  
  {
    cout << "\n" << "***isless float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isless, isless>, vml_test_in2out1<float, float, bool, isless>>(1);
    cout << "\n" << "***isless double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isless, isless>, vml_test_in2out1<double, double, bool, isless>>(1);
    cout << "\n" << "***isless long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isless, isless>, vml_test_in2out1<long double, long double, bool, isless>>(1);
  }

  {
    cout << "\n" << "***islessequal float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::islessequal, islessequal>, vml_test_in2out1<float, float, bool, islessequal>>(1);
    cout << "\n" << "***islessequal double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double,bool,  llvm_test_in2out1_for_bool<double, double, bool, ex::islessequal, islessequal>, vml_test_in2out1<double, double, bool, islessequal>>(1);
    cout << "\n" << "***islessequal long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessequal, islessequal>, vml_test_in2out1<long double, long double, bool, islessequal>>(1);
  }

  {
    cout << "\n" << "***islessgreater float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float,bool,  llvm_test_in2out1_for_bool<float, float, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<float, float, bool, islessgreater>>(1);
    cout << "\n" << "***islessgreater double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<double, double, bool, islessgreater>>(1);
    cout << "\n" << "***islessgreater long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<long double, long double, bool, islessgreater>>(1);
  }

  {
    cout << "\n" << "***isunordered float***" << "\n" << endl; // pass
    test_performance_for_in2out1<float,float,bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isunordered, isunordered>, vml_test_in2out1<float, float, bool, isunordered>>(1);
    cout << "\n" << "***isunordered double***" << "\n" << endl; // pass
    test_performance_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double,  bool,ex::isunordered, isunordered>, vml_test_in2out1<double, double, bool, isunordered>>(1);
    cout << "\n" << "***isunordered long double***" << "\n" << endl; // pass
    test_performance_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool,ex::isunordered, isunordered>, vml_test_in2out1<long double, long double, bool, isunordered>>(1);
  }





///////////////////////////// ULP ////////////////////////////////////
  {
    cout << "\n" << "***Sin long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sin, sin>, vml_test_single<long double, long double, sin>>(1);
  }

  {
    cout << "\n" << "***Cos long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cos, cos>, vml_test_single<long double, long double, cos>>(1);
  }

  {
    cout << "\n" << "***Tan long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tan, tan>, vml_test_single<long double, long double, tan>>(1);
  }

  {
    cout << "\n" << "***Asin long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long  double, ex::asin, asin>, vml_test_single<long double, long double, asin>>(1, 2);
  }

  {
    cout << "\n" << "***Atan float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::atan, atan>, vml_test_single<double, double, atan>>(1);
    cout << "\n" << "***Atan double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);
    cout << "\n" << "***Atan long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);
  }

  {
    cout << "\n" << "***Atan2 float***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::atan2, atan2>, vml_test_in2out1<double, double, double, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
    cout << "\n" << "***Atan2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
  }

  {
    cout << "\n" << "***Exp long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long  double, ex::exp, exp>, vml_test_single<long double, long double, exp>>(1);
  }

  {
    cout << "\n" << "***Exp2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::exp2, exp2>, vml_test_single<long double, long double, exp2>>(1,1);
  }

  {
    cout << "\n" << "***Log long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log, log>, vml_test_single<long double, long double, log>>(1);
  }

  {
    cout << "\n" << "***Log10 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log10, log10>, vml_test_single<long double, long double, log10>>(1,1);
  }

  {
    cout << "\n" << "***Log2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log2, log2>, vml_test_single<long double, long double, log2>>(1);
  }

  {
    cout << "\n" << "***Log1p long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log1p, log1p>, vml_test_single<long double, long double, log1p>>(1);
  }

  {
    cout << "\n" << "***Logb float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::logb, logb>, vml_test_single<double, double, logb>>(1);
    cout << "\n" << "***Logb double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
    cout << "\n" << "***Logb long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
  }

  {
    cout << "\n" << "***Ilogb long double***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Pow float***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::pow, pow>, vml_test_in2out1<double, double, double, pow>>(1);
    cout << "\n" << "***Pow double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::pow, pow>,  vml_test_in2out1<long double, long double, long double, pow>>(1);
    cout << "\n" << "***Pow long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::pow, pow>,  vml_test_in2out1<long double, long double, long double, pow>>(1);
  }

  {
    cout << "\n" << "***Sqrt long double**" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sqrt, sqrt>, vml_test_single<long double, long double, sqrt>>(1);
  }

  {
    cout << "\n" << "***Cbrt float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::cbrt, cbrt>, vml_test_single<double, double, cbrt>>(1);
    cout << "\n" << "***Cbrt double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);
    cout << "\n" << "***Cbrt long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);
  }

  {
    cout << "\n" << "***Hypot float***" << "\n" << endl; 
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::hypot, hypot>, vml_test_in2out1< double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);
  
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<float, float, float, float, double, double, double, double, llvm_test_in3out1<float, float, float, float, ex::hypot, hypot>, vml_test_in3out1< double, double, double, double, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<double, double, double, double, long double, long double, long double, long double, llvm_test_in3out1<double, double, double, double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<long double, long double, long double, long double, long double, long double, long double, long double, llvm_test_in3out1<long double, long double, long double, long double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
  }

  {
    cout << "\n" << "***Expm1 float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::expm1, expm1>, vml_test_single<double, double, expm1>>(1);
    cout << "\n" << "***Expm1 double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);
    cout << "\n" << "***Expm1 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);
  }

  {
    cout << "\n" << "***Sinh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sinh, sinh>, vml_test_single<long double, long double, sinh>>(1);
  }

  {
    cout << "\n" << "***Cosh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cosh, cosh>, vml_test_single<long double, long double, cosh>>(1);
  }

  {
    cout << "\n" << "***Tanh float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::tanh, tanh>, vml_test_single<double, double, tanh>>(1);
    cout << "\n" << "***Tanh double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);
    cout << "\n" << "***Tanh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);
  }

  {
    cout << "\n" << "***Asinh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::asinh, asinh>, vml_test_single<long double, long double, asinh>>(1);
  }

  {
    cout << "\n" << "***Acosh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::acosh, acosh>, vml_test_single<long double, long double, acosh>>(1,1);
  }

  {
    cout << "\n" << "***Atanh float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::atanh, atanh>, vml_test_single<double, double, atanh>>(1, 2);
    cout << "\n" << "***Atanh double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1, 2);
    cout << "\n" << "***Atanh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1, 2);
  }

  {
    cout << "\n" << "***Erf float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::erf, erf>, vml_test_single<double, double, erf>>(1);
    cout << "\n" << "***Erf double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
    cout << "\n" << "***Erf long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
  }

  {
    cout << "\n" << "***Erfc float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::erfc, erfc>, vml_test_single<double, double, erfc>>(1);
    cout << "\n" << "***Erfc double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::erfc, erfc>, vml_test_single<long double, long double, erfc>>(1);
    cout << "\n" << "***Erfc long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::erfc, erfc>, vml_test_single<long double, long double, erfc>>(1);
  }

  {
    cout << "\n" << "***Tgamma float***" << endl;
    ulp_test_accuracy_for_single<float,float, double, double, llvm_test_single<float, float, ex::tgamma, tgamma>, vml_test_single<double,double, tgamma>>(1);
    cout << "\n" << "***Tgamma double***" << endl;
    ulp_test_accuracy_for_single<double,double, long double, long double, llvm_test_single<double, double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);
    cout << "\n" << "***Tgamma long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);
  }

  {
    cout << "\n" << "***Lgamma float***" << endl;
    ulp_test_accuracy_for_single<float,float, double,double,llvm_test_single<float, float, ex::lgamma, lgamma>, vml_test_single<double,double, lgamma>>(1);
    cout << "\n" << "***Lgamma double***" << endl;
    ulp_test_accuracy_for_single<double,double,long double, long double, llvm_test_single<double, double, ex::lgamma, lgamma>, vml_test_single<long double, long double,  lgamma>>(1);
    cout << "\n" << "***Lgamma long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);
  }

  {
    cout << "\n" << "***Frexp float***" << endl;
    ulp_test_accuracy_for_in2out1_2<long double, int, long double, long double, int, long double, llvm_test_in2out1_2<long double, int, long double, ex::frexp, frexp>, vml_test_in2out1_2<long double, int, long double, frexp>>(1);
  }

  {
    cout << "\n" << "***Ldexp float***" << endl;
    ulp_test_accuracy_for_in2out1<long double, int, long double, long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::ldexp, ldexp>, vml_test_in2out1<long double, int, long double, ldexp>>(1);
  }

  { 
    cout << "\n" << "***Scalbn float***" << endl;
    ulp_test_accuracy_for_in2out1<float, int, float, double, int, double, llvm_test_in2out1<float, int, float, ex::scalbn, scalbn>, vml_test_in2out1<double, int, double, scalbn>>(1);
    cout << "\n" << "***Scalbn double***" << endl;
    ulp_test_accuracy_for_in2out1<double, int, double, long double, int, long double, llvm_test_in2out1<double, int, double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);
    cout << "\n" << "***Scalbn long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, int, long double, long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);
  }

  { 
    cout << "\n" << "***Scalbln float***" << endl;
    ulp_test_accuracy_for_in2out1<float, long int, float, double, long int, double, llvm_test_in2out1<float, long int, float, ex::scalbln, scalbln>, vml_test_in2out1<double, long int, double, scalbln>>(1);
    cout << "\n" << "***Scalbln double***" << endl;
    ulp_test_accuracy_for_in2out1<double, long int, double, long double, long int, long double, llvm_test_in2out1<double, long int, double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);
    cout << "\n" << "***Scalbln long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long int, long double, long double, long int, long double, llvm_test_in2out1<long double, long int, long double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);
  }

  {
    cout << "\n" << "***Nearbyint float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::nearbyint, nearbyint>, vml_test_single<double, double, nearbyint>>(1);
    cout << "\n" << "***Nearbyint double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);
    cout << "\n" << "***Nearbyint long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);
  }

  {
    cout << "\n" << "***Rint float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::rint, rint>, vml_test_single<double, double, rint>>(1);
    cout << "\n" << "***Rint double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);
    cout << "\n" << "***Rint long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);
  }

  {
    cout << "\n" << "***Lrint***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Llrint***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Lround***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Llround***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Trunc float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::trunc, trunc>, vml_test_single<double, double, trunc>>(1);
    cout << "\n" << "***Trunc double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);
    cout << "\n" << "***Trunc long double***" << endl;
    ulp_test_accuracy_for_single<long double,long double, long double, long double, llvm_test_single<long double, long double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);
  }

  {
    cout << "\n" << "***Remquo float***" << endl;
    ulp_test_accuracy_for_in3out1_2<float, float, int, float, double, double, int, double, llvm_test_in3out1_2<float, float, int, float, ex::remquo, remquo>, vml_test_in3out1_2<double, double, int, double, remquo>>(1);
    cout << "\n" << "***Remquo double***" << endl;
    ulp_test_accuracy_for_in3out1_2<double, double, int, double, long double, long double, int, long double, llvm_test_in3out1_2<double, double, int, double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);
    cout << "\n" << "***Remquo long double***" << endl;
    ulp_test_accuracy_for_in3out1_2<long double, long double, int, long double, long double, long double, int, long double, llvm_test_in3out1_2<long double, long double, int, long double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);
  }

  {
    cout << "\n" << "***Copysign float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::copysign, copysign>, vml_test_in2out1<double, double, double,copysign>>(1);
    cout << "\n" << "***Copysign double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
    cout << "\n" << "***Copysign long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
  }

  // {
  //   , nan, nanf, nanl
  // }

  {
    cout << "\n" << "***Nextafter float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::nextafter, nextafter>, vml_test_in2out1<double, double, double, nextafter>>(1);
    cout << "\n" << "***Nextafter double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);
    cout << "\n" << "***Nextafter long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);
  }

  {
    cout << "\n" << "***Nexttoward float***" << endl;
    ulp_test_accuracy_for_in2out1<float, long double, float, double, long double, double, vml_test_in2out1<float, long double, float, nexttoward>, vml_test_in2out1<double, long double, double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward double***" << endl;
    ulp_test_accuracy_for_in2out1<double, long double, double, long double, long double, long double, vml_test_in2out1< double, long double, double, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, vml_test_in2out1<long double, long double, long double, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
  }

  {
    cout << "\n" << "***Fdim float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::fdim, fdim>, vml_test_in2out1<double, double, double, fdim>>(1);
    cout << "\n" << "***Fdim double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);
    cout << "\n" << "***Fdim long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);
  }

  {
    cout << "\n" << "***Fpclassify***" << "\n" << endl;
  }

  {
    cout << "\n" << "***Signbit***" << "\n" << endl; // pass
  }

  {
    cout << "\n" << "***Isgreater***" << "\n" << endl; // pass
  }

  {
    cout << "\n" << "***Isgreaterequal***" << "\n" << endl; // pass
  }

  
  {
    cout << "\n" << "***isless***" << "\n" << endl; // pass
  }

  {
    cout << "\n" << "***islessequal***" << "\n" << endl; // pass
  }

  {
    cout << "\n" << "***islessgreater***" << "\n" << endl; // pass
  }

  {
    cout << "\n" << "***isunordered***" << "\n" << endl; // pass
  }


}
