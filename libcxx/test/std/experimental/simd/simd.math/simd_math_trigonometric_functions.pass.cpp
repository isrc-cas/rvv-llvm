
#include "simd_math_head.h"

int main()
{
    ///////////////////////  ACC  /////////////////////////////
  {
    cout << "\n" << "***Atan float***" << "\n" << endl;
    test_accuracy_for_single<float, float, llvm_test_single<float, float, ex::atan, atan>, vml_test_single<float, float, atan>>(1);
    cout << "\n" << "***Atan double***" << "\n" << endl;
    test_accuracy_for_single< double, double, llvm_test_single< double, double, ex::atan, atan>, vml_test_single< double, double, atan>>(1);
  }

  {
    cout << "\n" << "***Atan2 float***" << "\n" << endl;
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::atan2, atan2>, vml_test_in2out1<float, float, float, atan2>>(1);
    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::atan2, atan2>, vml_test_in2out1<double, double, double, atan2>>(1);
  }

    ///////////////////////  ULP  /////////////////////////////
  {
    cout << "\n" << "***Atan float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::atan, atan>, vml_test_single<double, double, atan>>(1);
  }

  {
    cout << "\n" << "***Atan2 float***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::atan2, atan2>, vml_test_in2out1<double, double, double, atan2>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Sin long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sin, sin>, vml_test_single<long double, long double, sin>>(1);

    cout << "\n" << "***Cos long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cos, cos>, vml_test_single<long double, long double, cos>>(1);

    cout << "\n" << "***Tan long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::tan, tan>, vml_test_single<long double, long double, tan>>(1);

    cout << "\n" << "***Asin long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::asin, asin>, vml_test_single<long double, long double, asin>>(1);

    cout << "\n" << "***Atan long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);

    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);

    cout << "\n" << "***Sin long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sin, sin>, vml_test_single<long double, long double, sin>>(1);

    cout << "\n" << "***Cos long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cos, cos>, vml_test_single<long double, long double, cos>>(1);

    cout << "\n" << "***Tan long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::tan, tan>, vml_test_single<long double, long double, tan>>(1);

    cout << "\n" << "***Asin long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long  double, ex::asin, asin>, vml_test_single<long double, long double, asin>>(1, 2);

    cout << "\n" << "***Atan double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);
    cout << "\n" << "***Atan long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::atan, atan>, vml_test_single<long double, long double, atan>>(1);

    cout << "\n" << "***Atan2 double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
    cout << "\n" << "***Atan2 long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::atan2, atan2>, vml_test_in2out1<long double, long double, long double, atan2>>(1);
    #endif
  }
}