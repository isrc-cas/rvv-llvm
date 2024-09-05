
#include "simd_math_head.h"

int main()
{
    ///////////////////////  ACC  /////////////////////////////
  {
    cout << "\n" << "***Nearbyint float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::nearbyint, nearbyint>, vml_test_single<float, float, nearbyint>>(1);
    cout << "\n" << "***Nearbyint double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::nearbyint, nearbyint>, vml_test_single<double, double, nearbyint>>(1);
  }

  
  {
    cout << "\n" << "***Rint float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::rint, rint>, vml_test_single<float, float, rint>>(1);
    cout << "\n" << "***Rint double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::rint, rint>, vml_test_single<double, double, rint>>(1);
  }

  {
    cout << "\n" << "***Lrint float***" << "\n" << endl;
    test_accuracy_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lrint, lrint>, vml_test_single<float, long int, lrint>>(1);
    cout << "\n" << "***Lrint double***" << "\n" << endl;
    test_accuracy_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lrint, lrint>, vml_test_single<double, long int, lrint>>(1);
  }

  {
    cout << "\n" << "***Llrint float***" << "\n" << endl;
    test_accuracy_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llrint, llrint>, vml_test_single<float, long long int, llrint>>(1);
    cout << "\n" << "***Llrint double***" << "\n" << endl;
    test_accuracy_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llrint, llrint>, vml_test_single<double, long long int, llrint>>(1);
  }

  {
    cout << "\n" << "***Lround float***" << "\n" << endl;
    test_accuracy_for_single<float,long int, llvm_test_single_for_ilogb<float, long int, ex::lround, lround>, vml_test_single<float, long int, lround>>(1);
    cout << "\n" << "***Lround double***" << "\n" << endl;
    test_accuracy_for_single<double,long int, llvm_test_single_for_ilogb<double, long int, ex::lround, lround>, vml_test_single<double, long int, lround>>(1);
  }

  {
    cout << "\n" << "***Llround float***" << "\n" << endl;
    test_accuracy_for_single<float,long long int, llvm_test_single_for_ilogb<float, long long int, ex::llround, llround>, vml_test_single<float, long long int, llround>>(1);
    cout << "\n" << "***Llround double***" << "\n" << endl;
    test_accuracy_for_single<double,long long int, llvm_test_single_for_ilogb<double, long long int, ex::llround, llround>, vml_test_single<double, long long int, llround>>(1);
  }

  {
    cout << "\n" << "***Trunc float***" << "\n" << endl;
    test_accuracy_for_single<float,float, llvm_test_single<float, float, ex::trunc, trunc>, vml_test_single<float, float, trunc>>(1);
    cout << "\n" << "***Trunc double***" << "\n" << endl;
    test_accuracy_for_single<double,double, llvm_test_single<double, double, ex::trunc, trunc>, vml_test_single<double, double, trunc>>(1);
  }


  { 
    cout << "\n" << "***Nextafter float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,float,float, llvm_test_in2out1<float, float, float, ex::nextafter, nextafter>, vml_test_in2out1<float, float, float, nextafter>>(1);
    cout << "\n" << "***Nextafter double***" << "\n" << endl;
    test_accuracy_for_in2out1<double,double,double, llvm_test_in2out1<double, double, double, ex::nextafter, nextafter>, vml_test_in2out1<double, double, double, nextafter>>(1);
  }

  {
    cout << "\n" << "***Nexttoward float***" << "\n" << endl;
    test_accuracy_for_in2out1<float,long double, float, llvm_test_in2out1<float, long double, float, ex::nexttoward, nexttoward>, vml_test_in2out1<float, long double, float, nexttoward>>(1);
  }


    ///////////////////////  ULP  /////////////////////////////


  {
    cout << "\n" << "***Nearbyint float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::nearbyint, nearbyint>, vml_test_single<double, double, nearbyint>>(1);
  }

  {
    cout << "\n" << "***Rint float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::rint, rint>, vml_test_single<double, double, rint>>(1);
  }

  {
    cout << "\n" << "***Trunc float***" << endl;
    ulp_test_accuracy_for_single<float, float, double, double, llvm_test_single<float, float, ex::trunc, trunc>, vml_test_single<double, double, trunc>>(1);
  }


  {
    cout << "\n" << "***Nextafter float***" << endl;
    ulp_test_accuracy_for_in2out1<float, float, float, double, double, double, llvm_test_in2out1<float, float, float, ex::nextafter, nextafter>, vml_test_in2out1<double, double, double, nextafter>>(1);
  }

  {
    // long double test
    #ifdef LDOUBLE_TEST
    cout << "\n" << "***Nearbyint long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);

    cout << "\n" << "***Rint long double***" << "\n" << endl;
    test_accuracy_for_single<long double, long double, llvm_test_single<long double, long double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);

    cout << "\n" << "***Lrint long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lrint, lrint>, vml_test_single<long double, long int, lrint>>(1);

    cout << "\n" << "***Llrint long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llrint, llrint>, vml_test_single<long double, long long int, llrint>>(1);

    cout << "\n" << "***Lround long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long int, llvm_test_single_for_ilogb<long double, long int, ex::lround, lround>, vml_test_single<long double, long int, lround>>(1);

    cout << "\n" << "***Llround long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long long int, llvm_test_single_for_ilogb<long double, long long int, ex::llround, llround>, vml_test_single<long double, long long int, llround>>(1);

    cout << "\n" << "***Trunc long double***" << "\n" << endl;
    test_accuracy_for_single<long double,long double, llvm_test_single<long double, long double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);

    cout << "\n" << "***Nextafter long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double,long double,long double, llvm_test_in2out1<long double, long double, long double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);

    cout << "\n" << "***Nexttoward double***" << "\n" << endl;
    test_accuracy_for_in2out1<double, long double, double, llvm_test_in2out1<double, long double, double, ex::nexttoward, nexttoward>, vml_test_in2out1<double, long double, double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward long double***" << "\n" << endl;
    test_accuracy_for_in2out1<long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::nexttoward, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);

    cout << "\n" << "***Nearbyint double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);
    cout << "\n" << "***Nearbyint long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::nearbyint, nearbyint>, vml_test_single<long double, long double, nearbyint>>(1);

    cout << "\n" << "***Rint double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);
    cout << "\n" << "***Rint long double***" << endl;
    ulp_test_accuracy_for_single<long double, long double, long double, long double, llvm_test_single<long double, long double, ex::rint, rint>, vml_test_single<long double, long double, rint>>(1);

    cout << "\n" << "***Trunc double***" << endl;
    ulp_test_accuracy_for_single<double, double, long double, long double, llvm_test_single<double, double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);
    cout << "\n" << "***Trunc long double***" << endl;
    ulp_test_accuracy_for_single<long double,long double, long double, long double, llvm_test_single<long double, long double, ex::trunc, trunc>, vml_test_single<long double, long double, trunc>>(1);

    cout << "\n" << "***Nextafter double***" << endl;
    ulp_test_accuracy_for_in2out1<double, double, double, long double, long double, long double, llvm_test_in2out1<double, double, double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);
    cout << "\n" << "***Nextafter long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, llvm_test_in2out1<long double, long double, long double, ex::nextafter, nextafter>, vml_test_in2out1<long double, long double, long double, nextafter>>(1);

    cout << "\n" << "***Nexttoward float***" << endl;
    ulp_test_accuracy_for_in2out1<float, long double, float, double, long double, double, vml_test_in2out1<float, long double, float, nexttoward>, vml_test_in2out1<double, long double, double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward double***" << endl;
    ulp_test_accuracy_for_in2out1<double, long double, double, long double, long double, long double, vml_test_in2out1< double, long double, double, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
    cout << "\n" << "***Nexttoward long double***" << endl;
    ulp_test_accuracy_for_in2out1<long double, long double, long double, long double, long double, long double, vml_test_in2out1<long double, long double, long double, nexttoward>, vml_test_in2out1<long double, long double, long double, nexttoward>>(1);
    #endif
  }
}