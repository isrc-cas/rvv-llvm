#include "simd_math_head.h"


int main()
{
    ///////////////////////  ACC  /////////////////////////////
  {
    cout << "\n" << "***Tanh float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::tanh, tanh>, vml_test_single<float, float, tanh>>(1);
    cout << "\n" << "***Tanh double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::tanh, tanh>, vml_test_single<double, double, tanh>>(1);
  }

  {
    cout << "\n" << "***Atanh float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::atanh, atanh>, vml_test_single<float, float, atanh>>(1);
    cout << "\n" << "***Atanh double***" << "\n" << endl;
    test_accuracy_for_single<double, double, llvm_test_single<double, double, ex::atanh, atanh>, vml_test_single<double, double, atanh>>(1);
  }

    ///////////////////////  ULP  /////////////////////////////
  {
    cout << "\n" << "***Tanh float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::tanh, tanh>, vml_test_single<double, double, tanh>>(1);
  }

  {
    cout << "\n" << "***Atanh float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::atanh, atanh>, vml_test_single<double, double, atanh>>(1, 2);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Sinh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sinh, sinh>, vml_test_single<long double, long double, sinh>>(1);

    cout << "\n" << "***Cosh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cosh, cosh>, vml_test_single<long double, long double, cosh>>(1);

    cout << "\n" << "***Tanh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);

    cout << "\n" << "***Asinh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::asinh, asinh>, vml_test_single<long double, long double, asinh>>(1);

    cout << "\n" << "***Acosh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::acosh, acosh>, vml_test_single<long double, long double, acosh>>(1);

    cout << "\n" << "***Atanh long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1);

    cout << "\n" << "***Sinh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sinh, sinh>, vml_test_single<long double, long double, sinh>>(1);

    cout << "\n" << "***Cosh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cosh, cosh>, vml_test_single<long double, long double, cosh>>(1);

    cout << "\n" << "***Tanh double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);
    cout << "\n" << "***Tanh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tanh, tanh>, vml_test_single<long double, long double, tanh>>(1);

    cout << "\n" << "***Asinh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::asinh, asinh>, vml_test_single<long double, long double, asinh>>(1);

    cout << "\n" << "***Acosh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::acosh, acosh>, vml_test_single<long double, long double, acosh>>(1,1);

    cout << "\n" << "***Atanh double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1, 2);
    cout << "\n" << "***Atanh long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::atanh, atanh>, vml_test_single<long double, long double, atanh>>(1, 2);

    #endif
  }

}