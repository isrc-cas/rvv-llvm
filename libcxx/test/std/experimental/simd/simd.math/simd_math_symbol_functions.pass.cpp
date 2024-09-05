
#include "simd_math_head.h"

int main()
{
   

  {
    cout << "\n" << "***Copysign float***" << "\n" << endl;
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::copysign, copysign>, vml_test_in2out1<float, float, float, copysign>>(1);
    cout << "\n" << "***Copysign double***" << "\n" << endl; 
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::copysign, copysign>, vml_test_in2out1<double, double, double, copysign>>(1);
  }

  {
    cout << "\n" << "***Signbit float***" << "\n" << endl; // pass
    test_accuracy_for_single<float,bool, llvm_test_single_bool<float, bool, ex::signbit, signbit>, vml_test_single<float, bool, signbit>>(1);
    cout << "\n" << "***Signbit double***" << "\n" << endl; 
    test_accuracy_for_single<double,bool, llvm_test_single_bool<double, bool, ex::signbit, signbit>, vml_test_single<double, bool, signbit>>(1);
  }

  ///////////////////////  ULP  /////////////////////////////

  {
    cout << "\n" << "***Copysign float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::copysign, copysign>, vml_test_in2out1<double, double, double,copysign>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Copysign long double***" << "\n" << endl; 
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
    
    cout << "\n" << "***Signbit long double***" << "\n" << endl; 
    test_accuracy_for_single<long double, bool, llvm_test_single_bool<long double, bool, ex::signbit, signbit>, vml_test_single<long double, bool, signbit>>(1);
    
    cout << "\n" << "***Copysign double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
    cout << "\n" << "***Copysign long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::copysign, copysign>, vml_test_in2out1<long double, long double, long double, copysign>>(1);
    #endif
  }


}