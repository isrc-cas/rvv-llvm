
#include "simd_math_head.h"

int main()
{
    ///////////////////////  ACC  /////////////////////////////

  {
    cout << "\n" << "***Erf float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::erf, erf>, vml_test_single<float, float, erf>>(1);
    cout << "\n" << "***Erf double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::erf, erf>, vml_test_single<double, double, erf>>(1);
  }

  {
    cout << "\n" << "***Erfc float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::erfc, erfc>, vml_test_single<float, float, erfc>>(1);
    cout << "\n" << "***Erfc double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::erfc, erfc>, vml_test_single<double, double, erfc>>(1);
  }



    ///////////////////////  ULP  /////////////////////////////

  {
    cout << "\n" << "***Erf float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::erf, erf>, vml_test_single<double, double, erf>>(1);
  }

  {
    cout << "\n" << "***Erfc float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::erfc, erfc>, vml_test_single<double, double, erfc>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Erf long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
    cout << "\n" << "***Erfc long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
    
    cout << "\n" << "***Erf double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);
    cout << "\n" << "***Erf long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::erf, erf>, vml_test_single<long double, long double, erf>>(1);

    cout << "\n" << "***Erfc double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::erfc, erfc>, vml_test_single<long double, long double, erfc>>(1);
    cout << "\n" << "***Erfc long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::erfc, erfc>, vml_test_single<long double, long double, erfc>>(1);

    #endif
  }


}