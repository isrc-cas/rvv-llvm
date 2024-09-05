
#include "simd_math_head.h"

int main()
{
    ///////////////////////  ACC  /////////////////////////////
  { 
    cout << "\n" << "***Scalbn float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,int,float, llvm_test_in2out1<float, int, float, ex::scalbn, scalbn>, vml_test_in2out1<float, int, float, scalbn>>(1);
    cout << "\n" << "***Scalbn double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,int,double, llvm_test_in2out1<double, int, double, ex::scalbn, scalbn>, vml_test_in2out1<double, int, double, scalbn>>(1);
  }

  { 
    cout << "\n" << "***Scalbln float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,long int,float, llvm_test_in2out1<float, long int, float, ex::scalbln, scalbln>, vml_test_in2out1<float, long int, float, scalbln>>(1);
    cout << "\n" << "***Scalbln double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,long int,double, llvm_test_in2out1<double, long int, double, ex::scalbln, scalbln>, vml_test_in2out1<double, long int, double, scalbln>>(1);
  }

  {
    cout << "\n" << "***Remquo float***" << "\n" << endl; 
    test_accuracy_for_in3out1_2<float, float, int, float, llvm_test_in3out1_2<float, float, int, float, ex::remquo, remquo>, vml_test_in3out1_2<float, float, int, float, remquo>>(1);
    cout << "\n" << "***Remquo double***" << "\n" << endl; 
    test_accuracy_for_in3out1_2<double, double, int, double, llvm_test_in3out1_2<double, double, int, double, ex::remquo, remquo>, vml_test_in3out1_2<double, double, int, double, remquo>>(1);
  }


  {
    cout << "\n" << "***Fdim float***" << "\n" << endl;
    test_accuracy_for_in2out1<float, float, float, llvm_test_in2out1<float, float, float, ex::fdim, fdim>, vml_test_in2out1<float, float, float, fdim>>(1);
    cout << "\n" << "***Fdim double***" << "\n" << endl; 
    test_accuracy_for_in2out1<double, double, double, llvm_test_in2out1<double, double, double, ex::fdim, fdim>, vml_test_in2out1<double, double, double, fdim>>(1);
  }

  {
    cout << "\n" << "***Fpclassify float***" << "\n" << endl;
    test_accuracy_for_single<float,int, llvm_test_single_for_ilogb<float, int, ex::fpclassify, fpclassify>,  vml_test_single<float, int, fpclassify>>(1);
    cout << "\n" << "***Fpclassify double***" << "\n" << endl; 
    test_accuracy_for_single<double,int, llvm_test_single_for_ilogb<double, int, ex::fpclassify, fpclassify>,  vml_test_single<double, int, fpclassify>>(1);
  }

    ///////////////////////  ULP  /////////////////////////////
    
  { 
    cout << "\n" << "***Scalbn float***" << endl;
    ulp_test_accuracy_for_in2out1<float, int, float, double, int, double, llvm_test_in2out1<float, int, float, ex::scalbn, scalbn>, vml_test_in2out1<double, int, double, scalbn>>(1);
  }

  { 
    cout << "\n" << "***Scalbln float***" << endl;
    ulp_test_accuracy_for_in2out1<float, long int, float, double, long int, double, llvm_test_in2out1<float, long int, float, ex::scalbln, scalbln>, vml_test_in2out1<double, long int, double, scalbln>>(1);
  }

  {
    cout << "\n" << "***Remquo float***" << endl;
    ulp_test_accuracy_for_in3out1_2<float, float, int, float, double, double, int, double, llvm_test_in3out1_2<float, float, int, float, ex::remquo, remquo>, vml_test_in3out1_2<double, double, int, double, remquo>>(1);
  }

  {
    cout << "\n" << "***Fdim float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::fdim, fdim>, vml_test_in2out1<double, double, double, fdim>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Frexp long double***" << "\n" << endl;
    test_accuracy_for_in2out1_2<long double, int, long double, llvm_test_in2out1_2<long double, int, long double, ex::frexp, frexp>, vml_test_in2out1_2<long double, int, long double, frexp>>(1);

    cout << "\n" << "***Ldexp long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::ldexp, ldexp>, vml_test_in2out1<long double, int, long double, ldexp>>(1);

    cout << "\n" << "***Scalbn long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,int,long double, llvm_test_in2out1<long double, int, long double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);

    cout << "\n" << "***Scalbln long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,long int ,long double, llvm_test_in2out1<long double, long int, long double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);

    cout << "\n" << "***Remquo long double***" << "\n" << endl; 
    test_accuracy_for_in3out1_2<long double, long double, int, long double, llvm_test_in3out1_2<long double, long double, int, long double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);

    cout << "\n" << "***Fdim double***" << "\n" << endl; 
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);

    cout << "\n" << "***Fpclassify long double***" << "\n" << endl; 
    test_accuracy_for_single<long double, int, llvm_test_single_for_ilogb<long double, int, ex::fpclassify, fpclassify>, vml_test_single<long double, int, fpclassify>>(1);

    cout << "\n" << "***Frexp float***" << endl;
    ulp_test_accuracy_for_in2out1_2<long double, int, long double, long double, int, long double, llvm_test_in2out1_2<long double, int, long double, ex::frexp, frexp>, vml_test_in2out1_2<long double, int, long double, frexp>>(1);

    cout << "\n" << "***Ldexp float***" << endl;
    ulp_test_accuracy_for_in2out1<long double, int, long double, long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::ldexp, ldexp>, vml_test_in2out1<long double, int, long double, ldexp>>(1);

    cout << "\n" << "***Scalbn double***" << endl;
    ulp_test_accuracy_for_in2out1<double, int, double, long double, int, long double, llvm_test_in2out1<double, int, double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);
    cout << "\n" << "***Scalbn long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, int, long double, long double, int, long double, llvm_test_in2out1<long double, int, long double, ex::scalbn, scalbn>, vml_test_in2out1<long double, int, long double, scalbn>>(1);

    cout << "\n" << "***Scalbln double***" << endl;
    ulp_test_accuracy_for_in2out1<double, long int, double, long double, long int, long double, llvm_test_in2out1<double, long int, double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);
    cout << "\n" << "***Scalbln long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long int, long double, long double, long int, long double, llvm_test_in2out1<long double, long int, long double, ex::scalbln, scalbln>, vml_test_in2out1<long double, long int, long double, scalbln>>(1);

    cout << "\n" << "***Remquo double***" << endl;
    ulp_test_accuracy_for_in3out1_2<double, double, int, double, long double, long double, int, long double, llvm_test_in3out1_2<double, double, int, double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);
    cout << "\n" << "***Remquo long double***" << endl;
    ulp_test_accuracy_for_in3out1_2<long double, long double, int, long double, long double, long double, int, long double, llvm_test_in3out1_2<long double, long double, int, long double, ex::remquo, remquo>, vml_test_in3out1_2<long double, long double, int, long double, remquo>>(1);

    cout << "\n" << "***Fdim double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);
    cout << "\n" << "***Fdim long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::fdim, fdim>, vml_test_in2out1<long double, long double, long double, fdim>>(1);

    #endif
  }



}