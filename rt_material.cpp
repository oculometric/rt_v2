#include "rt_material.h"

void sample_sky(rt_simplesky & sky, rt_vector3 & dir, rt_colour & col)
{
    float mixer = (asinf(dir.z) / M_PI) * 2;
    if (mixer > 0) lerp (sky.horizon, sky.upper, mixer, col);
    if (mixer < 0) lerp (sky.horizon, sky.lower, -mixer, col);
    col = sky.horizon;
}

void sample_sun(rt_sun & sun, rt_vector3 & dir, rt_colour & col)
{
    col = acosf(dot(sun.direction, dir)) <= sun.angle ? sun.colour : rt_vector3{ 0,0,0 };
}