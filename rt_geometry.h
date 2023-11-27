#ifndef RT_GEOMETRY_H
#define RT_GEOMETRY_H

#include "rt_vector.h"
#include "rt_material.h"

// represents a ray starting at an origin and travelling infinitely in a direction
struct rt_ray
{
    rt_vector3 origin, direction;
};

// represents a triangle
struct rt_tri
{
    // three vertex points, ABC
    rt_vector3 v1, v2, v3;
    // the vectors AB and AC
    rt_vector3 v1_2, v1_3;
    // normal vector (which points into the camera with A at the bottom and
    // B on the left, C on the right, aka numbered clockwise)
    rt_vector3 n;
    // constants for raycasting
    float d12_12;
    float d12_13;
    float d13_13;
    float inv_denom;
    // material which holds data about how this triangle should behave in the scene
    rt_material * material;
    // UV coordinates for vertices
    rt_vector2 uv1, uv2, uv3;
};

void precompute_tri_constants(rt_tri *);

// represents a bounding box with bottom corner min and upper corner max
struct rt_bounds
{
    rt_vector3 min, max;
};

#define BVH_DIVISIONS_PER_LEVEL 2

#endif