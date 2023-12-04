#include "rt_material.h"

#include <iostream>


void sample_sky(const rt_simplesky * sky, const rt_vector3 & dir, rt_colour & col)
{
    float mixer = asinf(dir.z);
    mixer = (mixer/M_PI)*2;
    if (mixer > 0) lerp (sky->horizon, sky->upper, mixer, col);
    else if (mixer < 0) lerp (sky->horizon, sky->lower, -mixer, col);
    else col = sky->horizon;
}

void sample_sun(const rt_sun * sun, const rt_vector3 & dir, rt_colour & col)
{
    float d;
    dot(sun->direction, dir, d);
    col = d >= sun->cos_angle ? sun->colour : rt_vector3{ 0,0,0 };
}