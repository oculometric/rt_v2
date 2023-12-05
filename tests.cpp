#include <iostream>
#include <chrono>
#include <climits>
#include "rt_vector.h"
#include "rt_graphics.h"
#include "bmp.h"

using namespace std;

int main()
{
    cout << "Starting renderer" << endl;

    rt_vbuf video_buffer = rt_vbuf("scene.cfg");
    video_buffer.render();

    cout << "done!" << endl;
}