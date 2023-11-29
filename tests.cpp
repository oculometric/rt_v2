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

#define OPERATIONS_TEST 10000000

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
        add(x,y,res);
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
        mul(x,y,res);
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
        sub(x,y,res);
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
        div(x,y,res);
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
        resf = dot(x,y);
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
        cross(x,y,res);
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
        lerp(x,y,resf,res);
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
        reflect(x,y,res);
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
        perturb(x, y, p, res);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing mag_sq..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        resf = mag_sq(x);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "Timing mag..." << endl;
    total = chrono::nanoseconds(0);
    for (int i = 0; i < OPERATIONS_TEST; i++)
    {
        randomise_vector(x);
        start = chrono::high_resolution_clock::now().time_since_epoch();
        resf = mag(x);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "All tests done." << endl;
}