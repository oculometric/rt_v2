#include <iostream>
#include <chrono>
#include <math.h>
#include <climits>
#include "rt_vector.h"

using namespace std;

void randomise_vector(rt_vector3& v)
{
    v.x = (((float)rand()/(float)INT_MAX)*2.0) - 1.0;
    v.y = (((float)rand()/(float)INT_MAX)*2.0) - 1.0;
    v.z = (((float)rand()/(float)INT_MAX)*2.0) - 1.0;
}

#define V_1 rt_vector3{ 2,6,-3 };
#define V_2 rt_vector3{ 1,-1,0 };
#define V_3 rt_vector3{ -1,0,0 };
#define V_4 rt_vector3{ 52,4,43 };
#define V_5 rt_vector3{ 1,1,1 };
#define V_6 rt_vector3{ 0,0,0 };

#define D_12 -4
#define D_25 0
#define D_26 0
#define D_45 99
#define D_43 -52
#define D_13 -2
#define D_51 5
#define D_32 -1

#define C_16 rt_vector3{ 0,0,0 };
#define C_32 rt_vector3{ 0,0,1 };
#define C_23 rt_vector3{ 0,0,-1 };
#define C_54 rt_vector3{ 39,9,-48 };
#define C_35 rt_vector3{ 0,1,-1 };
#define C_45 rt_vector3{ -39,-9,48 };
#define C_42 rt_vector3{ 43,43,-56 };
#define C_21 rt_vector3{ 3,3,8 };

#define M_1 7
#define M_2 1.41
#define M_3 1
#define M_4 67.59
#define M_5 1.73
#define M_6 0

#define N_1 rt_vector3{ 0.29,0.86,-0.43 };
#define N_2 rt_vector3{ 0.71,-0.71,0 };
#define N_3 rt_vector3{ -1,0,0 };
#define N_4 rt_vector3{ 0.77,0.06,0.64 };
#define N_5 rt_vector3{ 0.58,0.58,0.58 };
#define N_6 rt_vector3{ 0,0,0 };

#define R_12 rt_vector3{ 0,-8,3 };
#define R_15 rt_vector3{ 0,-4,5 };
#define R_54 rt_vector3{ 103,7,85 };
#define R_34 rt_vector3{ 105,8,86 };
#define R_43 rt_vector3{ -54,-4,-43 };
#define R_56 rt_vector3{ -1,-1,-1 };
#define R_41 rt_vector3{ -48,8,-49 };

#define OPERATIONS_TEST 100000000

void output_result(bool b)
{
    if (b) cout << "Pass" << endl;
    else cout << "Fail" << endl;
}

void describe(rt_vector3 v)
{
    cout << "(" << v.x << "," << v.y << "," << v.z << ")" << endl;
}

int main()
{
    // Test dot product
    cout << "Testing dot products" << endl;
    rt_vector3 a = V_1;
    rt_vector3 b = V_2;
    output_result((a ^ b) == D_12);
    a = V_2;
    b = V_5;
    output_result((a ^ b) == D_25);
    a = V_2;
    b = V_6;
    output_result((a ^ b) == D_26);
    a = V_4;
    b = V_5;
    output_result((a ^ b) == D_45);
    a = V_4;
    b = V_3;
    output_result((a ^ b) == D_43);
    a = V_1;
    b = V_3;
    output_result((a ^ b) == D_13);
    a = V_5;
    b = V_1;
    output_result((a ^ b) == D_51);
    a = V_3;
    b = V_2;
    output_result((a ^ b) == D_32);

    cout << "Testing cross products" << endl;
    a = V_1;
    b = V_6;
    rt_vector3 c = C_16;
    output_result((a % b) == c);
    a = V_3;
    b = V_2;
    c = C_32;
    output_result((a % b) == c);
    a = V_2;
    b = V_3;
    c = C_23;
    output_result((a % b) == c);
    a = V_5;
    b = V_4;
    c = C_54;
    output_result((a % b) == c);
    a = V_3;
    b = V_5;
    c = C_35;
    output_result((a % b) == c);
    a = V_4;
    b = V_5;
    c = C_45;
    output_result((a % b) == c);
    a = V_4;
    b = V_2;
    c = C_42;
    output_result((a % b) == c);
    a = V_2;
    b = V_1;
    c = C_21;
    output_result((a % b) == c);

    cout << "Testing magnitude and normalisation" << endl;
    a = V_1;
    b = N_1;
    output_result(mag(a) - M_1 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);
    a = V_2;
    b = N_2;
    output_result(mag(a) - M_2 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);
    a = V_3;
    b = N_3;
    output_result(mag(a) - M_3 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);
    a = V_4;
    b = N_4;
    output_result(mag(a) - M_4 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);
    a = V_5;
    b = N_5;
    output_result(mag(a) - M_5 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);
    a = V_6;
    b = N_6;
    output_result(mag(a) - M_6 < 0.01);
    c = norm_copy(a)-b;
    output_result(mag(c) < 0.01);

    cout << "Testing reflection" << endl;
    a = V_1;
    b = N_2;
    c = R_12;
    describe(a);
    describe(b);
    describe(reflect(a,b));
    describe(c);
    output_result(reflect(a,b) == c);
    a = V_1;
    b = N_5;
    c = R_15;
    output_result(reflect(a,b) == c);
    a = V_5;
    b = N_4;
    c = R_54;
    output_result(reflect(a,b) == c);
    a = V_3;
    b = N_4;
    c = R_34;
    output_result(reflect(a,b) == c);
    a = V_4;
    b = N_3;
    c = R_43;
    output_result(reflect(a,b) == c);
    a = V_5;
    b = N_6;
    c = R_56;
    output_result(reflect(a,b) == c);
    a = V_4;
    b = N_1;
    c = R_41;
    output_result(reflect(a,b) == c);

    cout << "Timing addition..." << endl;
    chrono::steady_clock::duration total = chrono::nanoseconds(0);
    chrono::steady_clock::duration start;
    chrono::steady_clock::duration end;
    rt_vector3 x;
    rt_vector3 y;
    rt_vector3 res;
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = x + y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing multiplication..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = x * y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing subtraction..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = x - y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing division..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = x / y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing dot product..." << endl;
    float resf = 0;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        resf = x ^ y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing cross product..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = x % y;
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing lerp..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        resf = (float)rand()/(float)INT_MAX;
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = lerp(x,y,resf);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing reflect..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = reflect(x,y);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing perturb..." << endl;
    total = chrono::nanoseconds(0);
    rt_vector2 p;
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        randomise_vector(y);
        p.u = (float)rand()/(float)INT_MAX;
        p.v = (float)rand()/(float)INT_MAX;
        start = chrono::high_resolution_clock::now().time_since_epoch();
        res = perturb(x, y, p);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "All tests done." << endl;
}