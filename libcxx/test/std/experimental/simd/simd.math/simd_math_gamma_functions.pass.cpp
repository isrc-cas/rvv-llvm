
#include "simd_math_head.h"

int main()
{
    ///////////////////////  ACC  /////////////////////////////
   
  {
    cout << "\n" << "***Lgamma float***" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::lgamma, lgamma>, vml_test_single<float, float, lgamma>>(1);
    cout << "\n" << "***Lgamma double***" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::lgamma, lgamma>, vml_test_single<double, double, lgamma>>(1);
    // test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);
  }

  {
    cout << "\n" << "***Tgamma float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::tgamma, tgamma>, vml_test_single<float, float, tgamma>>(1);
    cout << "\n" << "***Tgamma double***" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::tgamma, tgamma>, vml_test_single<double, double, tgamma>>(1);
  }

    ///////////////////////  ULP  /////////////////////////////


  {
    cout << "\n" << "***Tgamma float***" << endl;
    ulp_test_accuracy_for_single<float,float, double, double, llvm_test_single<float, float, ex::tgamma, tgamma>, vml_test_single<double,double, tgamma>>(1);
  }

  {
    cout << "\n" << "***Lgamma float***" << endl;
    ulp_test_accuracy_for_single<float,float, double,double,llvm_test_single<float, float, ex::lgamma, lgamma>, vml_test_single<double,double, lgamma>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Lgamma long double***" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);

    cout << "\n" << "***Tgamma long double***" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);

    cout << "\n" << "***Tgamma double***" << endl;
    ulp_test_accuracy_for_single<double,double, long double, long double, llvm_test_single<double, double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);
    cout << "\n" << "***Tgamma long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tgamma, tgamma>, vml_test_single<long double, long double, tgamma>>(1);

    cout << "\n" << "***Lgamma double***" << endl;
    ulp_test_accuracy_for_single<double,double,long double, long double, llvm_test_single<double, double, ex::lgamma, lgamma>, vml_test_single<long double, long double,  lgamma>>(1);
    cout << "\n" << "***Lgamma long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::lgamma, lgamma>, vml_test_single<long double, long double, lgamma>>(1);
    #endif
  }
}