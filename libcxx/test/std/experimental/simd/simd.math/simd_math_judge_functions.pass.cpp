
#include "simd_math_head.h"

int main()
{

    ///////////////////////  ACC  /////////////////////////////

  {
    cout << "\n" << "***Isgreater float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreater, isgreater>, vml_test_in2out1<float, float, bool, isgreater>>(1);
    cout << "\n" << "***Isgreater double***" << endl;
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreater, isgreater>, vml_test_in2out1<double, double, bool, isgreater>>(1);
  }

  {
    cout << "\n" << "***Isgreaterequal float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<float, float, bool, isgreaterequal>>(1);
    cout << "\n" << "***Isgreaterequal double***" << endl;
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<double, double, bool, isgreaterequal>>(1);
  }
  
  {
    cout << "\n" << "***isless float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isless, isless>, vml_test_in2out1<float, float, bool, isless>>(1);
    cout << "\n" << "***isless double***" << endl;
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::isless, isless>, vml_test_in2out1<double, double, bool, isless>>(1);
  }

  {
    cout << "\n" << "***islessequal float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float, bool, llvm_test_in2out1_for_bool<float, float, bool, ex::islessequal, islessequal>, vml_test_in2out1<float, float, bool, islessequal>>(1);
    cout << "\n" << "***islessequal double***" << endl;
    test_accuracy_for_in2out1<double,double,bool,  llvm_test_in2out1_for_bool<double, double, bool, ex::islessequal, islessequal>, vml_test_in2out1<double, double, bool, islessequal>>(1);
  }

  {
    cout << "\n" << "***islessgreater float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float,bool,  llvm_test_in2out1_for_bool<float, float, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<float, float, bool, islessgreater>>(1);
    cout << "\n" << "***islessgreater double***" << endl;
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<double, double, bool, islessgreater>>(1);
  }

  {
    cout << "\n" << "***isunordered float***" << "\n" << endl; // pass
    test_accuracy_for_in2out1<float,float,bool, llvm_test_in2out1_for_bool<float, float, bool, ex::isunordered, isunordered>, vml_test_in2out1<float, float, bool, isunordered>>(1);
    cout << "\n" << "***isunordered double***" << endl;
    test_accuracy_for_in2out1<double,double, bool, llvm_test_in2out1_for_bool<double, double,  bool,ex::isunordered, isunordered>, vml_test_in2out1<double, double, bool, isunordered>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Isgreater long double***" << endl;
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreater, isgreater>, vml_test_in2out1<long double, long double, bool, isgreater>>(1);

    cout << "\n" << "***Isgreaterequal long double***" << endl;
    test_accuracy_for_in2out1<long double,long double,bool,  llvm_test_in2out1_for_bool<long double, long double, bool, ex::isgreaterequal, isgreaterequal>, vml_test_in2out1<long double, long double, bool, isgreaterequal>>(1);

    cout << "\n" << "***isless long double***" << endl;
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::isless, isless>, vml_test_in2out1<long double, long double, bool, isless>>(1);

    cout << "\n" << "***islessequal long double***" << endl;
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessequal, islessequal>, vml_test_in2out1<long double, long double, bool, islessequal>>(1);

    cout << "\n" << "***islessgreater long double***" << endl;
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool, ex::islessgreater, islessgreater>, vml_test_in2out1<long double, long double, bool, islessgreater>>(1);

    cout << "\n" << "***isunordered long double***" << endl;
    test_accuracy_for_in2out1<long double,long double, bool, llvm_test_in2out1_for_bool<long double, long double, bool,ex::isunordered, isunordered>, vml_test_in2out1<long double, long double, bool, isunordered>>(1);
    #endif
  }

}