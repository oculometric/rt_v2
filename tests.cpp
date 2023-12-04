#include <iostream>
#include <chrono>
#include <climits>
#include "rt_vector.h"
#include "rt_graphics.h"
#include "bmp.h"

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

void perform_timing()
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
        dot(x,y, resf);
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
        mag_sq(x, resf);
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
        mag(x, resf);
        end = chrono::high_resolution_clock::now().time_since_epoch();
        total += end-start;
    }
    cout << "Time per operation (us): " << (float)(total.count()/OPERATIONS_TEST)/1000.0 << endl;

    cout << "All tests done." << endl;
}

int main()
{
    cout << "Generating 32x32 greyscale map..." << endl;
    uint8_t buffer[32*32];
    for (int i = 0; i < 32*32; i++)
    {
        buffer[i] = i%256;
        //buffer[i] = i%2 * 255;
    }

    write_bmp(buffer, 32, 32, 1, "demo.bmp");

    cout << "Generating demo image..." << endl;
    uint8_t among[8*8] = 
    {
        0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF,
        0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00
    };

    write_bmp(among, 8, 8, 1, "among.bmp");

    uint8_t fcb[64*64*3];
    for (int i = 0; i < 64*64; i++)
    {
        fcb[(i*3)] = i%255;
        fcb[(i*3) + 1] = (i%2 * 255) + i/4;
        fcb[(i*3) + 2] = i%4 * 63;
    }

    write_bmp(fcb, 64, 64, 3, "fullcolour.bmp");


    //perform_timing();


    cout << "Starting renderer" << endl;

    rt_vbuf video_buffer;
    video_buffer.sky = new rt_simplesky{ rt_colour{0.17254901961, 0.49019607843, 0.67058823529}, rt_colour{0.99215686275, 0.65882352941, 0.53333333333}, rt_colour{0,0,0}};
    video_buffer.camera->position = rt_vector3{-10,0,0};
    video_buffer.camera->look_direction = rt_vector3{1,0,0};
    video_buffer.camera->field_of_view = 90;
    video_buffer.camera->view_size_pixels = rt_vector2{640,480};

    rt_object * teapot = load_obj_file("teapot.obj");
    video_buffer.graphics_buffer->insert_object(teapot);

    video_buffer.render();
    
    uint8_t output_buffer[640*480*3];

    video_buffer.blit(output_buffer, VBUF_COLOUR);
    write_bmp(output_buffer, 640, 480, 3, "colour.bmp");

    video_buffer.blit(output_buffer, VBUF_NORMAL);
    write_bmp(output_buffer, 640, 480, 3, "normal.bmp");

    video_buffer.blit(output_buffer, VBUF_DEPTH);
    write_bmp(output_buffer, 640, 480, 1, "depth.bmp");

    video_buffer.blit(output_buffer, VBUF_COMPOSITE);
    write_bmp(output_buffer, 640, 480, 3, "composite.bmp");

}