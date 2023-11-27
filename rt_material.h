#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "rt_maths.h"
#include "rt_vector.h"

struct rt_material
{
    uint8_t flags;
    rt_colour diffuse_colour;
    float roughness;
};

struct rt_simplesky
{
    rt_colour upper;
    rt_colour horizon;
    rt_colour lower;

    void sample(rt_vector3 &, rt_colour &);
};

struct rt_sun
{
    rt_colour colour;
    rt_vector3 direction;
};

#endif