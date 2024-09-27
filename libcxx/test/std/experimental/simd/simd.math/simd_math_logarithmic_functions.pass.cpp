#include "simd_math_head.h"


int main()
{
    ///////////////////////  ACC  /////////////////////////////
  {
    cout << "\n" << "***Logb float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::logb, logb>, vml_test_single<float, float, logb>>(1);
    cout << "\n" << "***Logb double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::logb, logb>, vml_test_single<double, double, logb>>(1);
  }

    ///////////////////////  ULP  /////////////////////////////
  {
    cout << "\n" << "***Logb float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::logb, logb>, vml_test_single<double, double, logb>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Log long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log, log>, vml_test_single<long double, long double, log>>(1);

    cout << "\n" << "***Log10 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log10, log10>, vml_test_single<long double, long double, log10>>(1);

    cout << "\n" << "***Log2 long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log2, log2>, vml_test_single<long double, long double, log2>>(1);

    cout << "\n" << "***Log1p long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::log1p, log1p>, vml_test_single<long double, long double, log1p>>(1);

    cout << "\n" << "***Logb long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);

    cout << "\n" << "***Ilogb long double***" << "\n" << endl;
    test_accuracy_for_single<long double,int, llvm_test_single_for_ilogb<long double, int, ex::ilogb, ilogb>, vml_test_single<long double, int, ilogb>>(1);

    cout << "\n" << "***Log long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log, log>, vml_test_single<long double, long double, log>>(1);

    cout << "\n" << "***Log10 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log10, log10>, vml_test_single<long double, long double, log10>>(1,1);

    cout << "\n" << "***Log2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log2, log2>, vml_test_single<long double, long double, log2>>(1);

    cout << "\n" << "***Log1p long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::log1p, log1p>, vml_test_single<long double, long double, log1p>>(1);

    cout << "\n" << "***Logb double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
    cout << "\n" << "***Logb long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::logb, logb>, vml_test_single<long double, long double, logb>>(1);
    #endif
  }

  

}