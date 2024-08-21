#include <cmath>
#include <iostream>
#include <sys/time.h>
#include <iomanip>

// #define TEST_VECTOR_LEN_MIN_P 16
// #define TEST_VECTOR_LEN_MAX_P 163850

#define TEST_VECTOR_LEN_MIN_P 64
#define TEST_VECTOR_LEN_MAX_P 163850

#define TEST_VECTOR_LEN_MIN_A 99999 
#define TEST_VECTOR_LEN_MAX_A 100000

// #define TEST_VECTOR_LEN_MIN_A 9990
// #define TEST_VECTOR_LEN_MAX_A 10000

// #define TEST_VECTOR_LEN_MIN_A 90
// #define TEST_VECTOR_LEN_MAX_A 100

#define TIME_LOOPS 10
//#define long long long int

static int max_num;
static int max_ac_idx[100000];


#define LLVM_NAN_32F (float)(0.0 / 0.0)
#define LLVM_NAN_64F (double)(0.0 / 0.0)
#define LLVM_NAN_128F (long double)(0.0 / 0.0)

#define LLVM_INF_32F (float)(1.0 / 0.0)
#define LLVM_INF_64F (double)(1.0 / 0.0)
#define LLVM_INF_128F (long double)(1.0 / 0.0)

using namespace std; // if isnan() is not working, try the namespace

static inline double calc_time(const struct timeval tvs, const struct timeval tve)
{
    unsigned long elapsed = (tve.tv_sec-tvs.tv_sec)*1e6 + (tve.tv_usec-tvs.tv_usec);

    return elapsed*1e-3;
}


template<typename T1, typename T2>
static void compare_result(T1 *input1, T1 *input2, T2 *vml_out, T2 *openvml_i_out, int len)
{
    double precision = sizeof(T2) == 4 ? 1e-7 : 2e-16;
    printf("\nLen:%-7d\t\n", len);

    bool is_print = false;

    double accuracy = 0.0;
    double accuracy_sum = 0.0;
    double accuracy_max = 0.0;
    // double max_ac = vml_out[0];
    // double max_ac_right = openvml_i_out[0];
    double max_ac = 0;
    double max_ac_right = 0;
    max_num = 0;
    int j = 0;//index of the array of max_ac_idx 
    for(int i = 0; i < len; i++)
    {
        T2 vec1_value = vml_out[i];
        T2 vec2_value = openvml_i_out[i];

        // if(i < 30)
        // {
        //     std::cout << "   " <<i << std::endl;
        //     std::cout << "llvm_value is " << setprecision(40) <<  vec1_value << "  ";
        //     std::cout << "vml_value is " << setprecision(40) << vec2_value << "  " << std::endl;
        // }

        if(isnan(vec1_value) && isnan(vec2_value)){
            // cout<< i<< "     !!!!!!!!!!!!!!!!!!\n";
            continue;
        }

        if((isnan(vec1_value) ^ isnan(vec2_value)) == 1 && is_print)
        {
            std::cout << "ERROR: NAN";
            std::cout << "index is " << i <<"  ";
            std::cout << "input1 is " << input1[i] <<"  ";
            std::cout << "input2 is " << input2[i] <<"  ";
            std::cout << "llvm_value is " << vec1_value << "  ";
            std::cout << "vml_value is " << vec2_value << "  " << std::endl;
            cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
            // ASSERT_TRUE(0);
            assert(0);
        }

        if((isinf(vec1_value) ^ isinf(vec2_value)) == 1 && is_print)
        {
            std::cout << "ERROR: INF";
            std::cout << "index is " << i <<"  ";
            std::cout << "llvm_value is " << vec1_value << "  ";
            std::cout << "vml_value is " << vec2_value << "  " << std::endl;
            cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
            // ASSERT_TRUE(0);
            assert(0);
        }
        
        if(vec1_value <= 1e-16 && vec2_value <= 1e-16)
            accuracy = 0;
        else 
            accuracy = (fabs(vec1_value - vec2_value)) / fabs(vec2_value);
            // accuracy = (fabs(vec1_value - vec2_value));
        if(isnormal(accuracy))
            // accuracy_sum += accuracy, std::cout << "a cc is " << setprecision(20) << accuracy_sum << "  " << std::endl;;
            accuracy_sum += accuracy;

        // cout<< i<< "     ******************* "<< (vec1_value <= 1e-16) <<  (vec2_value <= 1e-16) << "\n";
        // printf("\taccuracy\t= %-.20lf\t\t \n", accuracy);
        if(accuracy > accuracy_max)
        {
            accuracy_max = accuracy;
            max_num = i;
            max_ac = vec1_value;
            max_ac_right = vec2_value;
        }
        
        ///////test if input number is 1.xxx then accuracy greater than 1e-7 ?
        //record idx if output accuracy > precision(1e-7);
        if(accuracy > precision && j < 100000)
        {
            max_ac_idx[j++] = i;
        }

        // if( accuracy > precision && is_print)
        // {
            //notPassed++;
            //printf("accuracy = %.15f", accuracy);
            //printf("\n%-7d\t", len);
            //printf("i = %d\n", i);
           
            // if(std::is_same<T2, long double>::value){
            //     printf("\tllvm_in = %-.17Lf\t\t \n", input1[i]);
            //     printf("\tvml_in = %-.17lf\t\t \n", input2[i]);
            //     printf("\tllvm_out = %-.17lf\t\t \n", vec1_value);
            //     printf("\tvml_out = %-.17lf\t\t \n", vec2_value);
            //     printf("index = %d llvm_out = %.40lf accuracy = %.17lf\t",i, vec1_value, accuracy);
            //     //   printf("index = %d p_src = %.40f\t",i, pow(2.718281828,vec2_value));
            //     printf("\tllvm_out - vml_out = %-.40lf\t\t \n",vec1_value - vec2_value);
            // }else{
            //     printf("\tllvm_in = %-.17lf\t\t \n", input1[i]);
            //     printf("\tvml_in = %-.17lf\t\t \n", input2[i]);
            //     printf("\tllvm_out = %-.17lf\t\t \n", vec1_value);
            //     printf("\tvml_out = %-.17lf\t\t \n", vec2_value);
            //     printf("index = %d llvm_out = %.40f accuracy = %.17f\t",i, vec1_value, accuracy);
            //     //   printf("index = %d p_src = %.40f\t",i, pow(2.718281828,vec2_value));
            //     printf("\tllvm_out - vml_out = %-.40lf\t\t \n",vec1_value - vec2_value);
            // }
            // //ASSERT_TRUE(0);
            // cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
        // }
    }

    // printf("\tprecision\t= %-.16lf\t\t\n", precision);
    // printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
    // printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
    // //print max accuracy output
    // printf("\tmax_err_idx\t= %d\n", max_num);
    // printf("\tmax_err llvm_out\t= %-.20lf\t\t \n", max_ac);
    // printf("\tmax_err std_out\t= %-.20lf\t\t \n", max_ac_right);

    if(std::is_same<T2, long double>::value){
        printf("\tprecision\t= %-.16lf\t\t\n", precision);
        printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
        printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
        //print max accuracy output
        printf("\tmax_err_idx\t= %d\n", max_num);
        printf("\tmax_err llvm_out\t= %-.40lf\t\t \n", max_ac);
        printf("\tmax_err std_out\t= %-.40lf\t\t \n", max_ac_right);
     }else{
        printf("\tprecision\t= %-.16lf\t\t\n", precision);
        printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
        printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
        //print max accuracy output
        printf("\tmax_err_idx\t= %d\n", max_num);
        printf("\tmax_err llvm_out\t= %-.20lf\t\t \n", max_ac);
        printf("\tmax_err std_out\t= %-.20lf\t\t \n", max_ac_right);
     }

    //judge average accuracy
    if(accuracy_sum/len > precision)
    {
        cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
            assert(0);
        // ASSERT_TRUE(0);
    }
}

template<typename T1, typename T2, typename T3, typename T4>
void compare_result(T1 *input1, T2 *input2, T3 *input3, T4 *vml_out, T4 *openvml_i_out, int len)
{
    double precision = sizeof(T2) == 4 ? 1e-7 : 2e-16;
    printf("\nLen:%-7d\t\n", len);
    // std::cout << setprecision(20) << vml_out[17706] << std::endl;
    // std::cout << setprecision(20) << openvml_i_out[17706] << std::endl;

    bool is_print = false;

    double accuracy = 0.0;
    double accuracy_sum = 0.0;
    double accuracy_max = 0.0;
    // double max_ac = vml_out[0];
    // double max_ac_right = openvml_i_out[0];
    double max_ac = 0;
    double max_ac_right = 0;
    max_num = 0;
    int j = 0;//index of the array of max_ac_idx 
    for(int i = 0; i < len; i++)
    {
        T4 vec1_value = vml_out[i];
        T4 vec2_value = openvml_i_out[i];

        // if(isnan((float)vec1_value) && isnan((float)vec2_value)){
        //     // cout<< i<< "     !!!!!!!!!!!!!!!!!!\n";
        //     continue;
        // }

        if((isnan(vec1_value) ^ isnan(vec2_value)) == 1 && is_print)
        {
            std::cout << "ERROR: NAN";
            std::cout << "index is " << i <<"  ";
            std::cout << "input1 is " << input1[i] <<"  ";
            std::cout << "input2 is " << input2[i] <<"  ";
            std::cout << "input3 is " << input3[i] <<"  ";
            std::cout << "llvm_value is " << vec1_value << "  ";
            std::cout << "vml_value is " << vec2_value << "  " << std::endl;
            cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
            // ASSERT_TRUE(0);
        }

        if((isinf(vec1_value) ^ isinf(vec2_value)) == 1 && is_print)
        {
            std::cout << "ERROR: INF";
            std::cout << "index is " << i <<"  ";
            std::cout << "llvm_value is " << vec1_value << "  ";
            std::cout << "vml_value is " << vec2_value << "  " << std::endl;
            cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
            // ASSERT_TRUE(0);
        }
        
        if(vec1_value <= 1e-16 && vec2_value <= 1e-16)
            accuracy = 0;
        else 
            accuracy = (fabs(vec1_value - vec2_value)) / fabs(vec2_value);
        if(isnormal(accuracy))
            accuracy_sum += accuracy;

        // cout<< i<< "     ******************* "<< (vec1_value <= 1e-16) <<  (vec2_value <= 1e-16) << "\n";
        // printf("\taccuracy\t= %-.20lf\t\t \n", accuracy);
        if(accuracy > accuracy_max)
        {
            accuracy_max = accuracy;
            max_num = i;
            max_ac = vec1_value;
            max_ac_right = vec2_value;
        }
        
        ///////test if input number is 1.xxx then accuracy greater than 1e-7 ?
        //record idx if output accuracy > precision(1e-7);
        if(accuracy > precision && j < 100000)
        {
            max_ac_idx[j++] = i;
        }

        // if( accuracy > precision && is_print)
        // {
            //notPassed++;
            //printf("accuracy = %.15f", accuracy);
            //printf("\n%-7d\t", len);
            //printf("i = %d\n", i);
           
            // if(std::is_same<T2, long double>::value){
            //     printf("\tllvm_in = %-.17Lf\t\t \n", input1[i]);
            //     printf("\tvml_in = %-.17lf\t\t \n", input2[i]);
            //     printf("\tllvm_out = %-.17lf\t\t \n", vec1_value);
            //     printf("\tvml_out = %-.17lf\t\t \n", vec2_value);
            //     printf("index = %d llvm_out = %.40lf accuracy = %.17lf\t",i, vec1_value, accuracy);
            //     //   printf("index = %d p_src = %.40f\t",i, pow(2.718281828,vec2_value));
            //     printf("\tllvm_out - vml_out = %-.40lf\t\t \n",vec1_value - vec2_value);
            // }else{
            //     printf("\tllvm_in = %-.17lf\t\t \n", input1[i]);
            //     printf("\tvml_in = %-.17lf\t\t \n", input2[i]);
            //     printf("\tllvm_out = %-.17lf\t\t \n", vec1_value);
            //     printf("\tvml_out = %-.17lf\t\t \n", vec2_value);
            //     printf("index = %d llvm_out = %.40f accuracy = %.17f\t",i, vec1_value, accuracy);
            //     //   printf("index = %d p_src = %.40f\t",i, pow(2.718281828,vec2_value));
            //     printf("\tllvm_out - vml_out = %-.40lf\t\t \n",vec1_value - vec2_value);
            // }
            // //ASSERT_TRUE(0);
            // cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
        // }
    }

    // printf("\tprecision\t= %-.16lf\t\t\n", precision);
    // printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
    // printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
    // //print max accuracy output
    // printf("\tmax_err_idx\t= %d\n", max_num);
    // printf("\tmax_err llvm_out\t= %-.20lf\t\t \n", max_ac);
    // printf("\tmax_err std_out\t= %-.20lf\t\t \n", max_ac_right);

    if(std::is_same<T2, long double>::value){
        printf("\tprecision\t= %-.16lf\t\t\n", precision);
        printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
        printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
        //print max accuracy output
        printf("\tmax_err_idx\t= %d\n", max_num);
        printf("\tmax_err llvm_out\t= %-.20lf\t\t \n", max_ac);
        printf("\tmax_err std_out\t= %-.20lf\t\t \n", max_ac_right);
     }else{
        printf("\tprecision\t= %-.16lf\t\t\n", precision);
        printf("\tave error\t= %-.16lf\t\t\n", accuracy_sum /len);
        printf("\tmax error\t= %-.16lf\t\t\n", accuracy_max);
        //print max accuracy output
        printf("\tmax_err_idx\t= %d\n", max_num);
        printf("\tmax_err llvm_out\t= %-.20lf\t\t \n", max_ac);
        printf("\tmax_err std_out\t= %-.20lf\t\t \n", max_ac_right);
     }

    //judge average accuracy
    if(accuracy_sum/len > precision)
    {
        cout<< "!!!!!!!!!!!!!!!!!Failed!!!!!!!!!!!!!!!!!" << '\n';
        // ASSERT_TRUE(0);
    }
}




///////////////////////////////////////data init functions //////////////////////////////////////////////////////
// template<typename T>
// static void data_init(T *pSrc1, T *pSrc2, unsigned int len, bool is_abs)
// {
//     unsigned int i;
//     for(i = 0; i < len ; i++)
//     {
//         T data;

//         if(is_abs == 0) {
//             //   data = (T)(drand48() * 3.27680f - 1.63840f);
//             data = (T)(drand48() * 2.0f - 1.0f); // 对于acos, asin, atanh的测试，生成[-1,1]的随机数
//         }
//         else
//           data = (T)(fabs(drand48() * 3.27680f - 1.63840f) + 1); // now it's gt 1, acosh,生成gt 1的随机数

//         // switch(i%4){
//         //     case 0:
//         //         data = LLVM_INF_32F;
//         //         break;
//         //     case 1:
//         //         data = LLVM_NAN_32F;
//         //         break;
//         //     case 2:
//         //         data = -LLVM_INF_32F;
//         //         break;
//         //     case 3:
//         //        data = 0;
//         //        break;
//         // }

//         pSrc1[i] = data;
//         pSrc2[i] = data;
//     }
// }

template<typename T>
static void data_init(T *pSrc1, T *pSrc2, unsigned int len, int is_abs, int type = 0)
{
    for(unsigned int i = 0; i < len ; i++)
    {
        T data;
        if(is_abs == 0) {
            if(typeid(T) == typeid(float) || typeid(T) == typeid(double) || typeid(T) == typeid(long double))
                data = (T)(drand48() * 3.27680f - 1.63840f);
            else if(typeid(T) == typeid(int8_t) || typeid(T) == typeid(char))
                data = (T)(rand() % 256 - 128);
            else if(typeid(T) == typeid(int16_t) || typeid(T) == typeid(short))
                data = (T)(rand() % 65536 - 32768);
            else if(typeid(T) == typeid(int32_t) || typeid(T) == typeid(int64_t))
                data = (T)(rand() % 4294967296 - 2147483648);
                // data = (T)(rand() % 65536 - 32768);
            else if(typeid(T) == typeid(uint32_t))
                data = (T)(rand() % 655360);
            else if(typeid(T) == typeid(uint64_t))
                data = (T)(rand() % 65536);
            else if(typeid(T) == typeid(uint16_t))
                data = (T)(rand() % 65536);
            else
                data = (T)(drand48() * 3.20f - 1.60f); //bfloat16
            // data = (T)(rand() % 256 - 128); // int8_t
                // data = (T)(rand() % 65536 - 32768); // int16_t
            // data = (T)(rand());
        }
        // else if (is_abs == 6)
        // {
        //     if(typeid(T) == typeid(float) || typeid(T) == typeid(double))
        //         data = NAN;
        // }
        // else if (is_abs == 4)
        // {
        //     if(typeid(T) == typeid(int8_t))
        //         data = INT8_MAX;
        //     if(typeid(T) == typeid(int16_t))
        //         data = INT16_MAX;
        //     if(typeid(T) == typeid(int32_t))
        //         data = INT32_MAX;
        //     if(typeid(T) == typeid(int64_t))
        //         data = INT64_MAX;
        // }
        // else if (is_abs == 5)
        // {
        //     if(typeid(T) == typeid(int8_t))
        //         data = INT8_MIN;
        //     if(typeid(T) == typeid(int16_t))
        //         data = INT16_MIN;
        //     if(typeid(T) == typeid(int32_t))
        //         data = INT32_MIN;
        //     if(typeid(T) == typeid(int64_t))
        //         data = INT64_MIN;
        // }
        
        else if(is_abs == 2)
        {
            data = (T)(drand48() * 2.0f - 1.0f); // 对于acos, asin, atanh的测试，生成[-1,1]的随机数
        }
        else 
        {
            if(typeid(T) == typeid(float) || typeid(T) == typeid(double))
                data = (T)(fabs(drand48() * 3.27680f) + 1); // gt 1, acosh也需要
            else if(typeid(T) == typeid(uint8_t))
                data = (T)(rand() % 256);
            else if(typeid(T) == typeid(uint16_t))
                data = (T)(rand() % 65536);
            else if(typeid(T) == typeid(uint32_t) || typeid(T) == typeid(uint64_t))
                data = (T)(rand() % 65536);
            else
                // data = (T)(fabs(drand48() * 3.27680f - 1.63840f) + 1);
                data = (T)(rand() % 256);
            // data = (T)(rand() % 65536);
        }
        pSrc1[i] = data;
        pSrc2[i] = data;
    }
            

    if(type == 1) {
        if(typeid(T) == typeid(int64_t))
        {
            int64_t srcA[] = {INT64_MAX, INT64_MIN, INT64_MIN, INT64_MAX, INT64_MAX, INT64_MIN, INT64_MIN, INT64_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint64_t))
        {
            uint64_t srcA[] = {155551466, 0, UINT64_MAX, UINT64_MAX, UINT64_MAX, 0, 1, 2};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int32_t))
        {
            int32_t srcA[16] = {0, INT32_MAX, INT32_MAX, INT32_MIN, INT32_MAX, INT32_MIN, -2147483646, 2147483646, INT32_MAX, INT32_MAX, INT32_MIN, 2147483646, 2147483646, 0, INT32_MAX, INT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int x = 0; x < Vlen; x++) 
            { 
                pSrc1[x] = srcA[x]; 
                pSrc2[x] = srcA[x]; 
                
            }
        }
        else if(typeid(T) == typeid(uint32_t))
        {
            uint32_t srcA[] = {UINT32_MAX, 1, UINT32_MAX, 3, 0, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 2, 3, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int16_t))
        {
            int16_t srcA[] = {0, 1, -1, INT16_MAX, INT16_MIN, INT16_MIN, INT16_MAX,
        INT16_MAX, INT16_MAX, INT16_MAX, INT16_MIN, -1, -32767, INT16_MIN, INT16_MIN, INT16_MAX, 32766, -32767,
        32766, -32767, INT16_MIN, INT16_MIN, INT16_MAX, -455, INT16_MIN, INT16_MAX, -455, INT16_MAX, INT16_MIN,
        INT16_MAX, INT16_MAX, 32766};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint16_t))
        {
            uint16_t srcA[] = {0, 11555, 2, 3, 4, 5, 6, 7,
        8, 65534, 10, 11, 65534, UINT16_MAX, UINT16_MAX, UINT16_MAX, 0, 1, 12596, 3, 4,
        12596, 6, 7, 8, 9, 10, 14545, 12, 0, 0, UINT16_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int8_t))
        {
            int8_t srcA[] = {0, 96, -1, INT8_MIN, -2, 3, -3, INT8_MAX, -4, 96, -5,
        6, -6, -89, INT8_MAX, 8, -8, INT8_MIN, -9, 10, -10, 11, -11, 12, INT8_MIN, INT8_MAX, INT8_MIN,
        INT8_MIN, INT8_MAX, INT8_MAX, INT8_MAX, INT8_MAX, 0, 96, -1, INT8_MIN, -2, 3, -3, INT8_MAX, -4,
        96, -5, 6, -6, -89, INT8_MAX, 8, -8, INT8_MIN, -9, 10, -10, 11, -11, 12, INT8_MIN, INT8_MAX,
        INT8_MIN, INT8_MIN, INT8_MAX, INT8_MAX, INT8_MAX, INT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint8_t))
        {
            uint8_t srcA[] = {UINT8_MAX, 0, 0, 0, UINT8_MAX, 23, 1, 12, 59, 0, 23, 23, 1,
        12, 59, 0, UINT8_MAX, 23, 1, 12, 59, 23, 1, 12, 59, 0, UINT8_MAX, 23, 1, UINT8_MAX, UINT8_MAX, UINT8_MAX,
        UINT8_MAX, 0, 0, 0, 0, 1, 1, 1, 1, 251, 254, 50, 250, 1, 2, 3, 4, 5, 6, 9, 8, 7, 0, 96, 12, 86, 86, 123,
        56, 0, UINT8_MAX, UINT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(float))
        {
            float srcA[] = {-FLT_MIN, -0.00000000029, -INFINITY, -NAN, 0.00000005, -0.0,
        FLT_MAX, FLT_MAX, -0.0, FLT_MIN, INFINITY, -INFINITY, -1.89999999999, NAN, FLT_MAX, -2.605286871738435e+38};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(double))
        {
            double srcA[] = {-1.89999999999, -NAN, -DBL_MIN, DBL_MAX, -0.000000000098,
        DBL_MAX, -DBL_MIN, DBL_MAX};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if (typeid(T) == typeid(long double))
        {
            long double srcA[] = {-1.89999999999, -NAN, -LDBL_MIN, LDBL_MAX, -0.000000000098,
        LDBL_MAX, -LDBL_MIN, LDBL_MAX};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }

    } else if(type == 2) {
        if(typeid(T) == typeid(int64_t))
        {
            int64_t srcA[] = {0, 0, INT64_MIN, INT64_MAX, 0, 0, INT64_MIN, INT64_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint64_t))
        {
            uint64_t srcA[] = {851897485, UINT64_MAX, 0, UINT64_MAX, UINT64_MAX, 0, 1, 2};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int32_t))
        {
            int32_t srcA[] = {-1, INT32_MIN, INT32_MIN, INT32_MAX, INT32_MAX, INT32_MIN, 2147483646, 2147483646, INT32_MAX, 2, INT32_MIN, INT32_MAX, INT32_MIN, 0, INT32_MAX, INT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint32_t))
        {
            uint32_t srcA[] = {UINT32_MAX, 1, 999, 3, UINT32_MAX, UINT32_MAX, 0, UINT32_MAX, 3, 0, UINT32_MAX, UINT32_MAX, UINT32_MAX, 3, 0, UINT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int16_t))
        {
            int16_t srcA[] = {0, 11555, -1, -32767, 32766, -32767, 32766, -32767,
        INT16_MIN, INT16_MIN, INT16_MAX, -455, INT16_MAX, INT16_MIN, INT16_MAX, INT16_MAX, 32766, -32767, 32766,
        -32767, INT16_MIN, INT16_MIN, INT16_MAX, -455, INT16_MIN, INT16_MAX, -455, INT16_MAX, INT16_MIN,
        INT16_MAX, INT16_MAX, 32766};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint16_t))
        {
            uint16_t srcA[] = {0, 1, 12596, 3, 4, 12596, 6, 7,
        8, 9, 10, 14545, 12, 0, 0, UINT16_MAX, 0, 1, 12596, 3, 4, 12596, 6, 7,
        8, 9, 10, 14545, 12, 0, 0, UINT16_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int8_t))
        {
            int8_t srcA[] = {0, 1, INT8_MAX, 2, -2, 3, -3, 4, -4, INT8_MAX, -5,
        6, -6, INT8_MAX, -7, 8, -8, INT8_MAX, -9, 10, -85, 11, -11, 12, 126, 126, INT8_MIN, INT8_MIN,
        INT8_MIN, INT8_MIN, INT8_MAX, INT8_MAX, 0, 96, -1, INT8_MIN, -2, 3, -3, INT8_MAX, -4, 96, -5, 6,
        -6, -89, INT8_MAX, 8, -8, INT8_MIN, -9, 10, -10, 11, -11, 12, INT8_MIN, INT8_MAX, INT8_MIN, INT8_MIN,
        INT8_MAX, INT8_MAX, INT8_MAX, INT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint8_t))
        {
            uint8_t srcA[] = {0, 0, 0, UINT8_MAX, 0, 255, 248, 96, 60, UINT8_MAX, 0, 214,
        248, 96, 60, UINT8_MAX, 0, 214, 248, 96, 60, 96, 60, UINT8_MAX, 0, 214, 248, 96, 60, 0, 0, UINT8_MAX, UINT8_MAX,
        0, 0, 0, 0, 1, 1, 1, 1, 251, 254, 50, 250, 1, 2, 3, 4, 5, 6, 9, 8, 7, 0, 96, 12, 86, 86, 123, 56, 0, UINT8_MAX,
        UINT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(float))
        {
            float srcA[] = {FLT_MIN, 0.00000000029, -INFINITY, 1.500001, -1.499999, INFINITY,
        FLT_MAX, FLT_MIN, INFINITY, FLT_MIN, NAN, -2.605286871738435e+38, 1.899999999998, -NAN, -FLT_MAX,
        2.605286871738435e+38};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(double))
        {
            double srcA[] = {INFINITY, 4294967298, DBL_MAX, -DBL_MAX,
        0.000000000098, DBL_MAX, DBL_MIN, -DBL_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if (typeid(T) == typeid(long double))
        {
            long double srcA[] = {INFINITY, 4294967298, LDBL_MAX, -LDBL_MAX,
        0.000000000098, LDBL_MAX, LDBL_MIN, -LDBL_MAX};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }


    } else if(type == 3) {
        if(typeid(T) == typeid(int64_t))
        {
            int64_t srcA[] = {INT64_MIN, INT64_MAX, 0, 0, 0, 0, INT64_MIN, INT64_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint64_t))
        {
            uint64_t srcA[] = {1, 3, UINT64_MAX, 0, UINT64_MAX, 0, 1, 2};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int32_t))
        {
            int32_t srcA[] =  {INT32_MIN, INT32_MAX, 2, INT32_MIN, INT32_MAX, INT32_MIN, INT32_MIN, 2147483646, INT32_MIN, INT32_MAX, INT32_MIN, -2147483646, 2147483646, 2147483646, INT32_MIN, INT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint32_t))
        {
            uint32_t srcA[] = {UINT32_MAX, 1, 2, 3, 0, 0, UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, UINT32_MAX, 3, UINT32_MAX, 3, 0, UINT32_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int16_t))
        {
            int16_t srcA[] = {0, 1, -1, 32766, -32767, -32767, 32766, -32767,
        INT16_MAX, INT16_MIN, INT16_MAX, 456, INT16_MAX, INT16_MIN, INT16_MIN, INT16_MAX, 32766, -32767,
        32766, -32767, INT16_MIN, INT16_MIN, INT16_MAX, -455, INT16_MIN, INT16_MAX, -455, INT16_MAX,
        INT16_MIN, INT16_MAX, INT16_MAX, 32766};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint16_t))
        {
            uint16_t srcA[] = {0, 1, 2, 3, 4, 5, 65534, 7,
        8, 9, 10, 11, 12, UINT16_MAX, 0, UINT16_MAX, 12596, 6, 7, 8, 9, 10, 14545, 12,
        0, 0, UINT16_MAX, 0, 1, 12596, 3, 4};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(int8_t))
        {
            int8_t srcA[] =  {0, INT8_MAX, -1, -89, -2, 3, -3, 4, INT8_MIN, 5, -5, -85,
        -6, INT8_MIN, -7, 8, -8, INT8_MAX, -9, 10, -10, 11, -89, 12, -127, 126, INT8_MAX, INT8_MIN, INT8_MIN,
        INT8_MAX, INT8_MIN, INT8_MAX, 0, 96, -1, INT8_MIN, -2, 3, -3, INT8_MAX, -4, 96, -5, 6, -6, -89, INT8_MAX,
        8, -8, INT8_MIN, -9, 10, -10, 11, -11, 12, INT8_MIN, INT8_MAX, INT8_MIN, INT8_MIN, INT8_MAX, INT8_MAX,
        INT8_MAX, INT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(uint8_t))
        {
            uint8_t srcA[] = {0, UINT8_MAX, 0, 0, 0, 0, 1, 1, 1, 1, 251, 254, 50, 250, 1, 2, 3,
        4, 5, 6, 9, 8, 7, 0, 96, 12, 86, 86, 123, 56, 0, UINT8_MAX, UINT8_MAX, UINT8_MAX, 0, 0, 0, 0, 1, 1, 1, 1, 251,
        254, 50, 250, 1, 2, 3, 4, 5, 6, 9, 8, 7, 0, 96, 12, 86, 86, 123, 56, 0, UINT8_MAX, UINT8_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(float))
        {
            float srcA[] = {FLT_MAX, INFINITY, -0.000000, -1.499999, -NAN, -2.500001,
        FLT_MAX, FLT_MAX, 0.0, FLT_MIN, -NAN, 8.99999999998, INFINITY, -FLT_MAX, -INFINITY};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if(typeid(T) == typeid(double))
        {
            double srcA[] = {DBL_MAX, -2.499999, DBL_MAX, INFINITY,
        -1.000000000098, DBL_MAX, DBL_MIN, DBL_MAX};
            int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }
        else if (typeid(T) == typeid(long double))
        {
            long double srcA[] = {LDBL_MAX, -2.499999, LDBL_MAX, INFINITY,
        -1.000000000098, LDBL_MAX, LDBL_MIN, LDBL_MAX};
        int Vlen = sizeof(srcA) / sizeof(srcA[0]); 
            for(int i = 0; i < Vlen; i++) 
            { 
                pSrc1[i] = srcA[i]; 
                pSrc2[i] = srcA[i]; 
            }
        }

    }
}
template <typename T, typename U>
struct is_Same {
    static const bool value = false;
};

template <typename T>
struct is_Same<T, T> {
    static const bool value = true;
};

template<typename T1, typename T2,
    void (*llvm_func)(int, const T1*, T2*), void (*vml_func)(int, const T1*, T2*)>
void test_accuracy_for_single(int element_per_data, int is_abs=0)    
{
    T1 *llvm_src;
    T1 *vml_src;
    T2 *llvm_dst;
    T2 *vml_dst;
    int i, j, len;
    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;
        llvm_src = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst  = (T2 *)malloc(sizeof(T2) * len);
        vml_src = (T1 *)malloc(sizeof(T1) * len);
        vml_dst  = (T2 *)malloc(sizeof(T2) * len);
        data_init<T1>(vml_src, llvm_src, len, is_abs);

        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;
        vml_func(i, (T1 *)vml_src, (T2 *)vml_dst);
        llvm_func(i, (T1 *)llvm_src, (T2 *)llvm_dst);

        compare_result<T1,T2>(vml_src, llvm_src, llvm_dst, vml_dst, len);
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);

        // if(is_Same<double, T2>::value)
        //     printf("double\n");
        // printf("%d  %d\n", is_Same<long double, T2>::value, is_Same<double, T2>::value);
        if(is_Same<long double, T2>::value)
            printf("\tmax_err input\t= %.20lf\n\tthere are %d results exceed the accuracy limit!!!\n", (double)vml_src[max_num], j);
        else
            printf("\tmax_err input\t= %.20Lf\n\tthere are %d results exceed the accuracy limit!!!\n", (long double)vml_src[max_num], j);
        free(llvm_src);
        free(llvm_dst);
        free(vml_src);
        free(vml_dst);
    }
}

// template<typename T1, typename T2, 
//     void (*llvm_func)(int, const T1*, T2*), void (*vml_func)(int, const T1*, T2*)>
// static void test_performance_for_single(int element_per_data, int is_abs = 0)    
// {
//     T1 *llvm_src;
//     T1 *vml_src;

//     T2 *llvm_dst;
//     T2 *vml_dst;

//     int i, k, len;

//     double openvml_i_time = 0.0f, vml_time = 0.0f;
//     printf("\n\n\nperformance test\n"); 
//     printf("\tlen\tllvm \tstandard\n"); 

//     for(i =TEST_VECTOR_LEN_MIN_P; i < TEST_VECTOR_LEN_MAX_P; i=i*2) 
//     {
//         len = i * element_per_data;
//         openvml_i_time = 0.0f, vml_time = 0.0f;
//         for(k = 0; k < TIME_LOOPS; k++)
//         {
//             llvm_src = (T1 *)malloc(sizeof(T1) * len);
//             llvm_dst  = (T2 *)malloc(sizeof(T2) * len);

//             vml_src = (T1 *)malloc(sizeof(T1) * len);
//             vml_dst  = (T2 *)malloc(sizeof(T2) * len);

//             data_init<T1>(vml_src, llvm_src, len, is_abs, 1);

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T1 *)llvm_src,  (T2 *)llvm_dst);
//                 gettimeofday(&tve, NULL);

//                 vml_time += calc_time(tvs, tve);
//             }

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 vml_func(i, (T1 *)vml_src, (T2 *)vml_dst);
//                 gettimeofday(&tve, NULL);

//                 openvml_i_time += calc_time(tvs, tve);
//             }

//             free(llvm_src);
//             free(llvm_dst);

//             free(vml_src);
//             free(vml_dst);
//         }
//         printf("\t%d\t%f\t%f\n", len, vml_time/k, openvml_i_time/k); 
//     }
// }

// template<typename T, typename T1, typename T2, 
//     void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const T*, T1*, T2*)>
// static void test_performance_for_in1out2(int element_per_data, int is_abs = 0)    
// {
//     T *llvm_src;
//     T *vml_src;

//     T1 *llvm_dst1;
//     T1 *vml_dst1;

//     T2 *llvm_dst2;

//     T2 *vml_dst2;

//     int i, k, len;

//     double openvml_i_time = 0.0f, vml_time = 0.0f;

//     printf("\n\n\nperformance test\n"); 
//     printf("\tlen\tllvm \tstandard\n"); 

//     for(i =TEST_VECTOR_LEN_MIN_P; i < TEST_VECTOR_LEN_MAX_P; i=i*2) 
//     {
//         len = i * element_per_data;
//         llvm_src = (T *)malloc(sizeof(T) * len);
//         llvm_dst1 = (T1 *)malloc(sizeof(T1) * len);
//         llvm_dst2  = (T2 *)malloc(sizeof(T2) * len);

//         vml_src = (T *)malloc(sizeof(T) * len);
//         vml_dst1 = (T1 *)malloc(sizeof(T1) * len);
//         vml_dst2  = (T2 *)malloc(sizeof(T2) * len);

//         data_init<T>(llvm_src, vml_src, len, is_abs, 1);

//         openvml_i_time = 0.0f, vml_time = 0.0f;
//         for(k = 0; k < TIME_LOOPS; k++)
//         {
//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T *)llvm_src, (T1 *)llvm_dst1, (T2 *)llvm_dst2);
//                 gettimeofday(&tve, NULL);

//                 vml_time += calc_time(tvs, tve);
//             }

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 vml_func(i, (T *)vml_src, (T1 *)vml_dst1, (T2 *)vml_dst2);
//                 gettimeofday(&tve, NULL);

//                 openvml_i_time += calc_time(tvs, tve);
//             }
//         }

//         free(llvm_src);
//         free(llvm_dst1);
//         free(llvm_dst2);

//         free(vml_src);
//         free(vml_dst1);
//         free(vml_dst2);
//         printf("\t%d\t%f\t%f\n", len, vml_time/k, openvml_i_time/k); 
//     }
// }


template<typename T, typename T1, typename T2, 
         void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const  T*, T1*, T2*)>
void test_accuracy_for_in2out1(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T *vml_src;

    T1 *llvm_src2;
    T1 *vml_src2;

    T2 *llvm_dst;
    T2 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst  = (T2 *)malloc(sizeof(T2) * len);

        vml_src = (T *)malloc(sizeof(T) * len);
        vml_src2 = (T1 *)malloc(sizeof(T1) * len);
        vml_dst  = (T2 *)malloc(sizeof(T2) * len);

        data_init<T>(llvm_src, vml_src, len, is_abs, 1);
        data_init<T1>(llvm_src2, vml_src2, len, is_abs, 2);
        
        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T *)vml_src, (T1 *)vml_src2, (T2 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;


        compare_result<T,T2>(vml_src, llvm_src, llvm_dst, vml_dst, len);
        
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_dst);
    }
}

template<typename T, typename T1, typename T2, typename T3, 
         void (*llvm_func)(int, const T*, T1*, T2*, T3*), void (*vml_func)(int, const  T*, T1*, T2*, T3*)>
void test_accuracy_for_in3out1(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T *vml_src;

    T1 *llvm_src2;
    T1 *vml_src2;

    T2 *llvm_src3;
    T2 *vml_src3;

    T3 *llvm_dst;
    T3 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_src3 = (T2 *)malloc(sizeof(T2) * len);
        llvm_dst  = (T3 *)malloc(sizeof(T3) * len);

        vml_src = (T *)malloc(sizeof(T) * len);
        vml_src2 = (T1 *)malloc(sizeof(T1) * len);
        vml_src3 = (T2 *)malloc(sizeof(T2) * len);
        vml_dst  = (T3 *)malloc(sizeof(T3) * len);

        data_init<T>(llvm_src, vml_src, len, is_abs, 1);
        data_init<T1>(llvm_src2, vml_src2, len, is_abs, 2);
        data_init<T1>(llvm_src3, vml_src3, len, is_abs, 3);
        
        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T *)vml_src, (T1 *)vml_src2, (T2 *)vml_src3, (T3 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_src3, (T3 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;


        compare_result<T,T2>(vml_src, llvm_src, llvm_dst, vml_dst, len);
        
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_src3);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_src3);
        free(vml_dst);
    }
}


template<typename T, typename T1, typename T2, typename T3, 
         void (*llvm_func)(int, T*, T1*, T2*, T3*), void (*vml_func)(int, T*, T1*, T2*, T3*)>
void test_accuracy_for_in3out1_2(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T *vml_src;

    T1 *llvm_src2;
    T1 *vml_src2;

    T2 *llvm_src3;
    T2 *vml_src3;

    T3 *llvm_dst;
    T3 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_src3 = (T2 *)malloc(sizeof(T2) * len);
        llvm_dst  = (T3 *)malloc(sizeof(T3) * len);

        vml_src = (T *)malloc(sizeof(T) * len);
        vml_src2 = (T1 *)malloc(sizeof(T1) * len);
        vml_src3 = (T2 *)malloc(sizeof(T2) * len);
        vml_dst  = (T3 *)malloc(sizeof(T3) * len);

        data_init<T>(llvm_src, vml_src, len, is_abs, 1);
        data_init<T1>(llvm_src2, vml_src2, len, is_abs, 2);
        data_init<T2>(llvm_src3, vml_src3, len, is_abs, 3);

        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;


        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T *)vml_src, (T1 *)vml_src2, (T2 *)vml_src3, (T3 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_src3, (T3 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;

        compare_result<T, T1, T2, T2>(vml_src, vml_src2, vml_src3, llvm_src3, vml_src3, len);
        compare_result<T, T1, T2, T3>(vml_src, vml_src2, vml_src3, llvm_dst, vml_dst, len);


        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_dst);
    }
}

// template<typename T, typename T1, typename T2, typename T3,
//     void (*llvm_func)(int, T*, T1*, T2*, T3*), void (*vml_func)(int, T*, T1*, T2*, T3*)>
// static void test_performance_for_in3out1_2(int element_per_data, int is_abs = 0)    
// {
//     T *llvm_src;
//     T *vml_src;

//     T1 *llvm_src1;
//     T1 *vml_src1;

//     T2 *llvm_dst1;
//     T2 *vml_dst1;

//     T3 *llvm_dst2;
//     T3 *vml_dst2;

//     int i, k, len;

//     double openvml_i_time = 0.0f, vml_time = 0.0f;
//     printf("\n\n\nperformance test\n"); 
//     printf("\tlen\topenvml  \tstandard\n"); 

//     for(i =TEST_VECTOR_LEN_MIN_P; i < TEST_VECTOR_LEN_MAX_P; i=i*2) 
//     {
//         len = i * element_per_data;
//         llvm_src = (T *)malloc(sizeof(T) * len);
//         llvm_src1 = (T1 *)malloc(sizeof(T1) * len);
//         llvm_dst1  = (T2 *)malloc(sizeof(T2) * len);
//         llvm_dst2  = (T3 *)malloc(sizeof(T3) * len);

//         vml_src = (T *)malloc(sizeof(T) * len);
//         vml_src1 = (T1 *)malloc(sizeof(T1) * len);
//         vml_dst1  = (T2 *)malloc(sizeof(T2) * len);
//         vml_dst2  = (T3 *)malloc(sizeof(T3) * len);

//         data_init<T>(llvm_src, vml_src, len, is_abs, 1);
//         data_init<T1>(llvm_src1, vml_src1, len, is_abs, 2);
//         data_init<T2>(llvm_dst1, vml_dst1, len, is_abs, 3);


//         openvml_i_time = 0.0f, vml_time = 0.0f;
//         for(k = 0; k < TIME_LOOPS; k++)
//         {
//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T *)llvm_src, (T1 *)llvm_src1, (T2 *)llvm_dst1, (T3 *)llvm_dst2);
//                 gettimeofday(&tve, NULL);

//                 vml_time += calc_time(tvs, tve);
//             }

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T *)vml_src, (T1 *)vml_src1, (T2 *)vml_dst1, (T3 *)vml_dst2);
//                 gettimeofday(&tve, NULL);

//                 openvml_i_time += calc_time(tvs, tve);
//             }
//         }

//         free(llvm_src);
//         free(llvm_dst1);
//         free(llvm_dst2);

//         free(vml_src);
//         free(vml_dst1);
//         free(vml_dst2);
//         printf("\t%d\t%f\t%f\n", len, vml_time/k, openvml_i_time/k); 
//     }
// }

// template<typename T, typename T1, typename T2, 
//     void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const T*, T1*, T2*)>
// static void test_performance_for_in2out1(int element_per_data, int is_abs = 0)    
// {
//     T *llvm_src;
//     T *vml_src;

//     T1 *llvm_src2;
//     T1 *vml_src2;

//     T2 *llvm_dst;
//     T2 *vml_dst;

//     int i, k, len;

//     double openvml_i_time = 0.0f, vml_time = 0.0f;

//     printf("\n\n\nperformance test\n"); 
//     printf("\tlen\tllvm \tstandard\n"); 

//     for(i =TEST_VECTOR_LEN_MIN_P; i < TEST_VECTOR_LEN_MAX_P; i=i*2) 
//     {
//         len = i * element_per_data;
//         llvm_src = (T *)malloc(sizeof(T) * len);
//         llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
//         llvm_dst  = (T2 *)malloc(sizeof(T2) * len);

//         vml_src = (T *)malloc(sizeof(T) * len);
//         vml_src2 = (T1 *)malloc(sizeof(T1) * len);
//         vml_dst  = (T2 *)malloc(sizeof(T2) * len);

//         data_init<T>(llvm_src, vml_src, len, is_abs, 1);
//         data_init<T1>(llvm_src2, vml_src2, len, is_abs, 1);

//         openvml_i_time = 0.0f, vml_time = 0.0f;
//         for(k = 0; k < TIME_LOOPS; k++)
//         {
//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 vml_func(i, (T *)vml_src, (T1 *)vml_src2, (T2 *)vml_dst);
//                 gettimeofday(&tve, NULL);

//                 openvml_i_time += calc_time(tvs, tve);
//             }

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_dst);
//                 gettimeofday(&tve, NULL);

//                 vml_time += calc_time(tvs, tve);
//             }
//         }

//         free(llvm_src);
//         free(llvm_src2);
//         free(llvm_dst);

//         free(vml_src);
//         free(vml_src2);
//         free(vml_dst);
//         printf("\t%d\t%f\t%f\n", len, vml_time/k, openvml_i_time/k);
//     }
// }


template<typename T, typename T1, typename T2, 
         void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const T*, T1*, T2*)>
void test_accuracy_for_in2out1_2(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T *vml_src;

    T1 *llvm_dst1;
    T1 *vml_dst1;

    T2 *llvm_dst2;
    T2 *vml_dst2;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_dst1  = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst2  = (T2 *)malloc(sizeof(T2) * len);

        vml_src = (T *)malloc(sizeof(T) * len);
        vml_dst1 = (T1 *)malloc(sizeof(T1) * len);
        vml_dst2  = (T2 *)malloc(sizeof(T2) * len);

        data_init<T>(llvm_src, vml_src, len, is_abs, 1);

        
        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;
        vml_func(i, (T *)vml_src, (T1 *)vml_dst1, (T2 *)vml_dst2);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_dst1, (T2 *)llvm_dst2);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;

        compare_result<T,T1>(llvm_src, vml_src, llvm_dst1, vml_dst1, len);
        compare_result<T,T2>(llvm_src, vml_src, llvm_dst2, vml_dst2, len);
        
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_dst1);
        free(llvm_dst2);

        free(vml_src);
        free(vml_dst1);
        free(vml_dst2);
    }
}

// template<typename T, typename T1, typename T2, 
//     void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const T*, T1*, T2*)>
// static void test_performance_for_in2out1_2(int element_per_data, int is_abs = 0)    
// {
//     T *llvm_src;
//     T *vml_src;

//     T1 *llvm_dst1;
//     T1 *vml_dst1;

//     T2 *llvm_dst2;

//     T2 *vml_dst2;

//     int i, k, len;

//     double openvml_i_time = 0.0f, vml_time = 0.0f;
//     printf("\n\n\nperformance test\n"); 
//     printf("\tlen\topenvml  \tstandard\n"); 

//     for(i =TEST_VECTOR_LEN_MIN_P; i < TEST_VECTOR_LEN_MAX_P; i=i*2) 
//     {
//         len = i * element_per_data;
//         llvm_src = (T *)malloc(sizeof(T) * len);
//         llvm_dst1 = (T1 *)malloc(sizeof(T1) * len);
//         llvm_dst2  = (T2 *)malloc(sizeof(T2) * len);

//         vml_src = (T *)malloc(sizeof(T) * len);
//         vml_dst1 = (T1 *)malloc(sizeof(T1) * len);
//         vml_dst2  = (T2 *)malloc(sizeof(T2) * len);

//         data_init<T>(llvm_src, vml_src, len, is_abs, 1);


//         openvml_i_time = 0.0f, vml_time = 0.0f;
//         for(k = 0; k < TIME_LOOPS; k++)
//         {
//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 llvm_func(i, (T *)llvm_src, (T1 *)llvm_dst1, (T2 *)llvm_dst2);
//                 gettimeofday(&tve, NULL);

//                 vml_time += calc_time(tvs, tve);
//             }

//             {
//                 struct timeval tvs, tve;

//                 gettimeofday(&tvs, NULL);
//                 vml_func(i, (T *)vml_src, (T1 *)vml_dst1, (T2 *)vml_dst2);
//                 gettimeofday(&tve, NULL);

//                 openvml_i_time += calc_time(tvs, tve);
//             }
//         }

//         free(llvm_src);
//         free(llvm_dst1);
//         free(llvm_dst2);

//         free(vml_src);
//         free(vml_dst1);
//         free(vml_dst2);
//         printf("\t%d\t%f\t%f\n", len, vml_time/k, openvml_i_time/k); 
//     }
// }



// ulp test
const double ULPERR = 2;


template<typename T, typename T1>
void data_init(T *pSrc1, T1 *pSrc2, unsigned int len, bool is_abs)
{
    unsigned int i;

    for(i = 0; i < len ; i++)
    {
        T data;

        if(is_abs == 0) {
              data = (T)(drand48() * 3.27680f - 1.63840f);
            // data = (T)(drand48() * 2.0f - 1.0f); // 对于acos的测试，生成[-1,1]的随机数
        }
        else
          data = (T)(fabs(drand48() * 3.27680f - 1.63840f) + 1); // now it's gt 1

        pSrc1[i] = data;
        pSrc2[i] = data;
    }

}


typedef union 
{
    struct{
        int p3;
        int p2;
        int p1;
        int p0;
    } s32s;
    struct{
        unsigned int p3;
        unsigned int p2;
        unsigned int p1;
        unsigned int p0;
    } u32s;
    struct
    {
        unsigned long p1;
        unsigned long p0;
    } u64s;
    struct{
        unsigned int mantissa3:32;
        unsigned int mantissa2:32;
        unsigned int mantissa1:32;
        unsigned int mantissa0:16;
        unsigned int exponent:15;
        unsigned int negative:1;
    }ieee;
    struct
    {
        uint64_t manl : 64;
        uint64_t manh : 48;
        unsigned int exp : 15;
        unsigned int sign : 1;
    } bits;
    struct
    {
        uint64_t manl : 64;
        uint64_t manh : 48;
        unsigned int expsign : 16;
    } xbits;
    long double f;
}Perf_128suf;

template <typename T, typename T2 = typename std::conditional<std::is_same<T, float>::value, uint32_t, uint64_t>::type>
static inline T2
asuint (T f)
{
  union
  {
    T f;
    T2 i;
  } u = { f };
  return u.i;
}

template <>  inline Perf_128suf
asuint (long double f)
{
  Perf_128suf tmp;
  tmp.f = f;
  return tmp;
}

template<typename T1, typename T2>
bool cmpeqasuint(T1 x1, T2 x2)
{
    if(is_same<T1, Perf_128suf>::value && is_same<T2, Perf_128suf>::value)
    {
        Perf_128suf tmp1, tmp2;
        tmp1.f = x1;
        tmp2.f = x2;
        return !!(tmp1.u64s.p0 == tmp2.u64s.p0 && tmp1.u64s.p1 == tmp2.u64s.p1);
    }
    else 
    {
        return x1 == x2;
    }
}



template <typename T>
static inline int ulpscale (T x) 
{
  int e = 0;
  if (is_same<T, float>::value) // c++17 can use if constexpr
  {
    e = asuint (x) >> 23 & 0xff;
    if (!e)
      e++;
    e = e - 0x7f - 23;
  }
  if (is_same<T, double>::value)
  {
    e = asuint(x) >> 52 & 0x7ff;
    if (!e)
      e++;
    e = e - 0x3ff - 52;
  }
  if (is_same<T, long double>::value)
  {
    Perf_128suf tmp;
    tmp.f = x;
    e = tmp.bits.exp;
    if (!e)
      e++;
    e = e - 0x3fff - 112;
  }
    return e;
}

template <typename T, typename T1>
double ulperr (T got, T1 p, int r = 0,
			   int ignore_zero_sign = 0)
{
  T want = (T)p;
  T d;
  T1 halfinf = std::numeric_limits<T1>::infinity() / 2;
  int ulpexp = ulpscale(want);
  double tail = isinf (want) ? scalbn(p - (long double)2 * halfinf, -ulpexp)
                            : scalbn(p - want, -ulpexp);

  double e;

  if (cmpeqasuint<T, T1>(got, want))
    return 0.0;
  if (isnan (got) && isnan (want))
    /* Ignore sign of NaN.  */
    return issignaling(got) == issignaling(want) ? 0 : INFINITY;
  if (signbit (got) != signbit (want))
    {
      /* Fall through to ULP calculation if ignoring sign of zero and at
	 exactly one of want and got is non-zero.  */
      if (ignore_zero_sign && want == got)
	return 0.0;
      if (!ignore_zero_sign || (want != 0 && got != 0))
	return INFINITY;
    }
  if (!isfinite (want) || !isfinite (got))
    {
      if (isnan (got) != isnan (want))
	return INFINITY;
      if (isnan (want))
	return 0;
      if (isinf (got))
	{
	  got = copysign(halfinf, got);
	  want *= 0.5f;
	}
      if (isinf (want))
	{
	  want = copysign(halfinf, want);
	  got *= 0.5f;
	}
    }
  if (r == FE_TONEAREST)
    {
      // TODO: incorrect when got vs want cross a powof2 boundary
      /* error = got > want
	      ? got - want - tail ulp - 0.5 ulp
	      : got - want - tail ulp + 0.5 ulp;  */
      d = got - want;
      e = d > 0 ? -tail - 0.5 : -tail + 0.5;
    }
  else
    {
      if ((r == FE_DOWNWARD && got < want) || (r == FE_UPWARD && got > want)
	  || (r == FE_TOWARDZERO && fabs (got) < fabs (want)))
	got = nextafter(got, want);
      d = got - want;
      e = -tail;
    }
  return scalbn(d, -ulpexp) + e;
}



template<typename T3, typename T4>
void compare_result_ulp(T3 *vml_out, T4 *openvml_i_out, int len)
{
  double maxerr = 0;
  uint64_t cnt = 0;
  uint64_t cnT2 = 0;
  uint64_t cnT4 = 0;
  uint64_t cnT3 = 0;
  uint64_t cntfail = 0;
  int idx = 0;
  int max_idx = 0;
  double avg_ulp = 0;
  double firsterr_ulp = 0;
  T3 vec1_valueidx{0};
  T4 vec2_valueidx{0};
  T3 vec1_max_valueidx{0};
  T4 vec2_max_valueidx{0};
  int fail = 0;
  int print = 0;

  int r = fegetround();
  if (r != FE_TONEAREST)
    fesetround (FE_TONEAREST);

  for (;;)
    {
      T3 vec1_value = vml_out[cnt];
      T4 vec2_value = openvml_i_out[cnt];
      
      cnt++;
      
      if(std::is_floating_point<T3>::value & std::is_floating_point<T4>::value)
      {
        int ok = cmpeqasuint<T3, T3>(vec1_value, (T3)vec2_value);
        if(ok) cnT2++;
        if (!ok)
        {
            double err = ulperr(vec1_value, vec2_value, 0, 0);
            double abserr = fabs (err);
            avg_ulp += abserr;
            // TODO: count errors below accuracy limit.
            if (abserr > 1)
            cnT4++;
            else if (abserr >= 0)
            cnT3++;
            if (abserr > ULPERR)
            {
                // print = 1;
                cntfail++;
                if (!fail)
                {
                fail = 1;
                idx = cnt - 1;
                vec1_valueidx = vec1_value;
                vec2_valueidx = vec2_value;
                firsterr_ulp = abserr;
                }
            }
            if (abserr > maxerr)
            {
            maxerr = abserr;
            max_idx = cnt - 1;
            vec1_max_valueidx = vec1_value;
            vec2_max_valueidx = vec2_value;
            }

            if (print)
            {
                std::cout << "\n\n-----------------------------------\n";
                std::cout << "idx is " << cnt - 1 << std::endl;
                std::cout << "vec1_value is " << vec1_value << "\t  turn uint is " << std::bitset<64>(asuint(vec1_value)) << endl;
                std::cout << "vec2_value is " << (T3)vec2_value << "\t  turn uint is " << std::bitset<64>(asuint((T3)vec2_value)) << endl;
                std::cout << "ULP limit is " << ULPERR << std::endl;
                std::cout << "maxerr ulp is " << maxerr << std::endl;
                assert(0);
            }

        }
      }
      if (cnt >= (uint64_t)(len))
	      break;
    }
  double cc = cnt;
  if (cntfail)
    printf ("\nFAIL ");
  else
    printf ("\nPASS ");
  printf (" round %d errlim %f maxerr %lf %s \n"
    "cnt %llu real true: %llu %g%%\t not real true: by ulp cnT3(0~1) %llu %g%% cnT4(>1) %llu "
	  "%g%% cntfail(>limit) %llu %g%%\n",
	  r, ULPERR,
	  maxerr, r == FE_TONEAREST ? "+0.5" : "+1.0",
	  (unsigned long long) cnt,
	  (unsigned long long) cnT2, 100.0 * cnT2 / cc, 
	  (unsigned long long) cnT3, 100.0 * cnT3 / cc,
	  (unsigned long long) cnT4, 100.0 * cnT4 / cc,
	  (unsigned long long) cntfail, 100.0 * cntfail / cc);
    std::cout << "max err vec1_value is " << fixed << setprecision(60) << vec1_max_valueidx << std::endl;
    std::cout << "max err vec2_value is " << fixed << setprecision(60) << vec2_max_valueidx << std::endl;
    std::cout << "max err idx is " << (int)max_idx << std::endl;

    if (r != FE_TONEAREST)
        fesetround (r);
    if((avg_ulp / cnt) > ULPERR)
    {
        std::cout << "first err vec1_value is " << vec1_valueidx << "\t  turn uint is " << std::bitset<64>(asuint(vec1_valueidx)) << endl;
        std::cout << "first err vec2_value is " << (T3)vec2_valueidx << "\t  turn uint is " << std::bitset<64>(asuint((T3)vec2_valueidx)) << endl;
        std::cout << "first err idx is " << idx << std::endl;
        std::cout << "first err idx_ulp is " << firsterr_ulp << std::endl;
        std::cout << "ULP limit is " << ULPERR << std::endl;
        std::cout << "maxerr idx is " << max_idx << std::endl;
        std::cout << "maxerr ulp is " << maxerr << std::endl;
        std::cout << "max err vec1_value is " << fixed << setprecision(20) << vec1_max_valueidx << std::endl;
        std::cout << "max err vec2_value is " << fixed << setprecision(20) << vec2_max_valueidx << std::endl;
        std::cout << "avg ulp is " << avg_ulp / cnt << std::endl;
        std::cout << "-----------------------------------\n";
        // ASSERT_TRUE(0);
        assert(0);
    }
}


template<typename T1, typename T2, typename T3, typename T4,
    void (*llvm_func)(int, const T1*, T2*), void (*vml_func)(int, const T3*, T4*)>
void ulp_test_accuracy_for_single(int element_per_data, int is_abs=0)    
{
    T1 *llvm_src;
    T3 *vml_src;
    T2 *llvm_dst;
    T4 *vml_dst;
    int i, j, len;
    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;
        llvm_src = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst  = (T2 *)malloc(sizeof(T2) * len);
        vml_src = (T3 *)malloc(sizeof(T3) * len);
        vml_dst  = (T4 *)malloc(sizeof(T4) * len);
        data_init<T1, T3>(llvm_src, vml_src, len, is_abs);

        llvm_src[0] = 1.00021207332611083984375;
        vml_src[0] = 1.00021207332611083984375;

        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;
        vml_func(i, (T3 *)vml_src, (T4 *)vml_dst);
        llvm_func(i, (T1 *)llvm_src, (T2 *)llvm_dst);

        // union 
        // {
        //     /* data */
        //     double a;
        //     uint64_t b;
        // }ff, ff1;
        // ff.a = vml_dst[16521];
        // ff1.a = vml_src[16521];
        
        // std::cout << ff.a << " " << ff.b << " " << ff1.a << " " << ff1.b << std::endl;
        // assert(0);

        compare_result_ulp<T2,T4>(llvm_dst, vml_dst, len);
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);

        // if(is_Same<double, T2>::value)
        //     printf("double\n");
        // printf("%d  %d\n", is_Same<long double, T2>::value, is_Same<double, T2>::value);
        if(is_Same<long double, T2>::value)
            printf("\tmax_err input\t= %.20lf\n\tthere are %d results exceed the accuracy limit!!!\n", (double)vml_src[max_num], j);
        else
            printf("\tmax_err input\t= %.20Lf\n\tthere are %d results exceed the accuracy limit!!!\n", (long double)vml_src[max_num], j);
        free(llvm_src);
        free(llvm_dst);
        free(vml_src);
        free(vml_dst);
    }
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5,
         void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const  T3*, T4*, T5*)>
void ulp_test_accuracy_for_in2out1(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T3 *vml_src;

    T1 *llvm_src2;
    T4 *vml_src2;

    T2 *llvm_dst;
    T5 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst  = (T2 *)malloc(sizeof(T2) * len);

        vml_src = (T3 *)malloc(sizeof(T3) * len);
        vml_src2 = (T4 *)malloc(sizeof(T4) * len);
        vml_dst  = (T5 *)malloc(sizeof(T5) * len);

        data_init<T, T3>(llvm_src, vml_src, len, is_abs);

        data_init<T1, T4>(llvm_src2, vml_src2, len, is_abs);

        
        // int j;
        //init array of max_ac_idx
        // for(j = 0; j < 100000; j++)
        //     max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T3 *)vml_src, (T4 *)vml_src2, (T5 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;


        compare_result_ulp<T2,T5>(llvm_dst, vml_dst, len);
        
        // for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_dst);
    }
}


template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, 
         void (*llvm_func)(int, const T*, T1*, T2*), void (*vml_func)(int, const T3*, T4*, T5*)>
void ulp_test_accuracy_for_in2out1_2(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T3 *vml_src;

    T1 *llvm_dst1;
    T4 *vml_dst1;

    T2 *llvm_dst2;
    T5 *vml_dst2;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_dst1  = (T1 *)malloc(sizeof(T1) * len);
        llvm_dst2  = (T2 *)malloc(sizeof(T2) * len);

        vml_src = (T3 *)malloc(sizeof(T3) * len);
        vml_dst1 = (T4 *)malloc(sizeof(T4) * len);
        vml_dst2  = (T5 *)malloc(sizeof(T5) * len);

        data_init<T, T3>(llvm_src, vml_src, len, is_abs);

        
        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;
        vml_func(i, (T3 *)vml_src, (T4 *)vml_dst1, (T5 *)vml_dst2);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_dst1, (T2 *)llvm_dst2);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst1 << "*****************" << *llvm_dst2 << "*****************" << *llvm_dst2 << endl;

        // compare_result_ulp<T1, T4>(llvm_dst1, vml_dst1, len);
        compare_result_ulp<T2,T5>(llvm_dst2, vml_dst2, len);
        
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_dst1);
        free(llvm_dst2);

        free(vml_src);
        free(vml_dst1);
        free(vml_dst2);
    }
}


template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, 
         void (*llvm_func)(int, const T*, T1*, T2*, T3*), void (*vml_func)(int, const  T4*, T5*, T6*, T7*)>
void ulp_test_accuracy_for_in3out1(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T4 *vml_src;

    T1 *llvm_src2;
    T5 *vml_src2;

    T2 *llvm_src3;
    T6 *vml_src3;

    T3 *llvm_dst;
    T7 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_src3 = (T2 *)malloc(sizeof(T2) * len);
        llvm_dst  = (T3 *)malloc(sizeof(T3) * len);

        vml_src = (T4 *)malloc(sizeof(T4) * len);
        vml_src2 = (T5 *)malloc(sizeof(T5) * len);
        vml_src3 = (T6 *)malloc(sizeof(T6) * len);
        vml_dst  = (T7 *)malloc(sizeof(T7) * len);

        data_init<T, T4>(llvm_src, vml_src, len, is_abs);
        data_init<T1, T5>(llvm_src2, vml_src2, len, is_abs);
        data_init<T2, T6>(llvm_src3, vml_src3, len, is_abs);
        
        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;

        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T4 *)vml_src, (T5 *)vml_src2, (T6 *)vml_src3, (T7 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_src3, (T3 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;


        compare_result_ulp<T3,T7>(llvm_dst, vml_dst, len);
        
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_src3);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_src3);
        free(vml_dst);
    }
}


template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, 
         void (*llvm_func)(int, T*, T1*, T2*, T3*), void (*vml_func)(int, T4*, T5*, T6*, T7*)>
void ulp_test_accuracy_for_in3out1_2(int element_per_data, int is_abs = 0)    
{
    T *llvm_src;
    T4 *vml_src;

    T1 *llvm_src2;
    T5 *vml_src2;

    T2 *llvm_src3;
    T6 *vml_src3;

    T3 *llvm_dst;
    T7 *vml_dst;

    int i, len;

    for(i =TEST_VECTOR_LEN_MIN_A; i < TEST_VECTOR_LEN_MAX_A; i++) 
    {
        len = i * element_per_data;

        llvm_src = (T *)malloc(sizeof(T) * len);
        llvm_src2 = (T1 *)malloc(sizeof(T1) * len);
        llvm_src3 = (T2 *)malloc(sizeof(T2) * len);
        llvm_dst  = (T3 *)malloc(sizeof(T3) * len);

        vml_src = (T4 *)malloc(sizeof(T4) * len);
        vml_src2 = (T5 *)malloc(sizeof(T5) * len);
        vml_src3 = (T6 *)malloc(sizeof(T6) * len);
        vml_dst  = (T7 *)malloc(sizeof(T7) * len);

        data_init<T, T4>(llvm_src, vml_src, len, is_abs);
        data_init<T1, T5>(llvm_src2, vml_src2, len, is_abs);
        data_init<T2, T6>(llvm_src3, vml_src3, len, is_abs);


        int j;
        //init array of max_ac_idx
        for(j = 0; j < 100000; j++)
            max_ac_idx[j] = -1;


        // cout << *vml_src << "*****1*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        vml_func(i, (T4 *)vml_src, (T5 *)vml_src2, (T6 *)vml_src3, (T7 *)vml_dst);
        // cout << *vml_src << "*****2*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;
        llvm_func(i, (T *)llvm_src, (T1 *)llvm_src2, (T2 *)llvm_src3, (T3 *)llvm_dst);
        // cout << *vml_src << "*****3*****************" << *llvm_src << "*****************" << *vml_src2 << "*****************" << *llvm_src2 << "*****************" << *vml_dst << "*****************" << *llvm_dst << endl;

        compare_result_ulp<T3,T7>(llvm_dst, vml_dst, len);
  
        for(j = 0; (j < 100000) && (max_ac_idx[j] != -1); j++);
        // printf("\tmax_err input\t= %.20f\n\tthere are %d results exceed the accuracy limit!!!\n", llvm_src[max_num], j);

        free(llvm_src);
        free(llvm_src2);
        free(llvm_dst);

        free(vml_src);
        free(vml_src2);
        free(vml_dst);
    }
}


