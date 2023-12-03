#include "rt_material.h"

#include <iostream>


void sample_sky(rt_simplesky & sky, rt_vector3 & dir, rt_colour & col)
{
    float mixer = asinf(dir.z);
    mixer = (mixer/M_PI)*2;
    if (mixer > 0) lerp (sky.horizon, sky.upper, mixer, col);
    else if (mixer < 0) lerp (sky.horizon, sky.lower, -mixer, col);
    else col = sky.horizon;
}

void sample_sun(rt_sun & sun, rt_vector3 & dir, rt_colour & col)
{
    float d;
    dot(sun.direction, dir, d);
    col = d >= sun.cos_angle ? sun.colour : rt_vector3{ 0,0,0 };
}