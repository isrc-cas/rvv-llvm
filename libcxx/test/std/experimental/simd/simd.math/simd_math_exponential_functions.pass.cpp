#include "simd_math_head.h"


int main()
{
    ///////////////////////  ACC  /////////////////////////////
  {
    cout << "\n" << "***Expm1 float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::expm1, expm1>, vml_test_single<float, float, expm1>>(1);
    cout << "\n" << "***Expm1 double***" << "\n" << endl;
    test_accuracy_for_single<double, double, llvm_test_single<double, double, ex::expm1, expm1>, vml_test_single<double, double, expm1>>(1);
  }
  
    ///////////////////////  ULP  /////////////////////////////

  {
    cout << "\n" << "***Expm1 float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::expm1, expm1>, vml_test_single<double, double, expm1>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Exp long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long  double, ex::exp, exp>, vml_test_single<long double, long double, exp>>(1);
    cout << "\n" << "***Exp2 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::exp2, exp2>, vml_test_single<long double, long double, exp2>>(1);
    
    cout << "\n" << "***Expm1 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);

    cout << "\n" << "***Exp long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long  double, ex::exp, exp>, vml_test_single<long double, long double, exp>>(1);
    
    cout << "\n" << "***Exp2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::exp2, exp2>, vml_test_single<long double, long double, exp2>>(1,1);

    cout << "\n" << "***Expm1 double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);
    cout << "\n" << "***Expm1 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::expm1, expm1>, vml_test_single<long double, long double, expm1>>(1);

    #endif
  }
}