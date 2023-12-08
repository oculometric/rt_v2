#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "rt_maths.h"
#include "rt_vector.h"

struct rt_material
{
    uint8_t flags;
    rt_colour colour;
    rt_colour emission;
    float roughness;
};

struct rt_simplesky
{
    rt_colour upper {1,1,1};
    rt_colour horizon {0.6,0.6,0.6};
    rt_colour lower {0,0,0};
};

void sample_sky(const rt_simplesky *, const rt_vector3 &, rt_colour &);

struct rt_sun
{
    rt_colour colour {10,10,8};
    rt_vector3 direction {0,0-1};
    float cos_angle = 0.999;
};

void sample_sun(const rt_sun *, const rt_vector3 &, rt_colour &);

#endif