#include "simd_math_head.h"


int main()
{
    ///////////////////////  ACC  /////////////////////////////

  {
    cout << "\n" << "***Pow float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::pow, pow>, vml_test_in2out1<float, float, float, pow>>(1);
    cout << "\n" << "***Pow double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::pow, pow>,  vml_test_in2out1<double, double, double, pow>>(1);
  }

  {
    cout << "\n" << "***Cbrt float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::cbrt, cbrt>, vml_test_single<float, float, cbrt>>(1);
    cout << "\n" << "***Cbrt double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::cbrt, cbrt>, vml_test_single<double, double, cbrt>>(1);
  }

  {
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::hypot, hypot>, vml_test_in2out1<float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::hypot, hypot>, vml_test_in2out1<double, double, double, hypot>>(1);
  
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    test_accuracy_for_in3out1<float, float, float, float, llvm_test_in3out1<float, float, float, float, ex::hypot, hypot>, vml_test_in3out1<float, float, float, float, hypot>>(1);
    cout << "\n" << "***Hypot double***" << "\n" << endl;
    test_accuracy_for_in3out1<double, double, double, double, llvm_test_in3out1<double, double, double, double, ex::hypot, hypot>, vml_test_in3out1<double, double, double, double, hypot>>(1);
  }
   ///////////////////////  ULP  /////////////////////////////
{
    cout << "\n" << "***Pow float***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::pow, pow>, vml_test_in2out1<double, double, double, pow>>(1);
  }

  {
    cout << "\n" << "***Cbrt float***" << "\n" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::cbrt, cbrt>, vml_test_single<double, double, cbrt>>(1);
  }

  {
    cout << "\n" << "***Hypot float***" << "\n" << endl; 
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::hypot, hypot>, vml_test_in2out1< double, double, double, hypot>>(1);
  
    cout << "\n" << "***Hypot float***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<float, float, float, float, double, double, double, double, llvm_test_in3out1<float, float, float, float, ex::hypot, hypot>, vml_test_in3out1< double, double, double, double, hypot>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Pow long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::pow, pow>, vml_test_in2out1<long double, long double, long double, pow>>(1);

    cout << "\n" << "***Sqrt long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::sqrt, sqrt>, vml_test_single<long double, long double, sqrt>>(1);

    cout << "\n" << "***Cbrt long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);

    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);

    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    test_accuracy_for_in3out1<long double, long double, long double, long double, llvm_test_in3out1<long double, long double, long double, long double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);

    cout << "\n" << "***Pow double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::pow, pow>,  vml_test_in2out1<long double, long double, long double, pow>>(1);
    cout << "\n" << "***Pow long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::pow, pow>,  vml_test_in2out1<long double, long double, long double, pow>>(1);

    cout << "\n" << "***Sqrt long double**" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::sqrt, sqrt>, vml_test_single<long double, long double, sqrt>>(1);

    cout << "\n" << "***Cbrt double***" << "\n" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);
    cout << "\n" << "***Cbrt long double***" << "\n" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::cbrt, cbrt>, vml_test_single<long double, long double, cbrt>>(1);

    cout << "\n" << "***Hypot double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::hypot, hypot>, vml_test_in2out1<long double, long double, long double, hypot>>(1);

    cout << "\n" << "***Hypot double***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<double, double, double, double, long double, long double, long double, long double, llvm_test_in3out1<double, double, double, double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
    cout << "\n" << "***Hypot long double***" << "\n" << endl;
    ulp_test_accuracy_for_in3out1<long double, long double, long double, long double, long double, long double, long double, long double, llvm_test_in3out1<long double, long double, long double, long double, ex::hypot, hypot>, vml_test_in3out1<long double, long double, long double, long double, hypot>>(1);
    #endif
  }
  
}