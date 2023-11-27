#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "rt_vector.h"
#include "rt_geometry.h"

struct rt_camera
{
    float near_clip;
    float far_clip;

    float fog_start;
    float fog_end;

    float field_of_view;

    rt_vector3 position;
    rt_vector3 look_direction;

    rt_vector2 view_size_pixels;

    float exposure;
    float gamma;

    // from here on these values should not be set directly
    rt_vector3 view_horizontal_vector;
    rt_vector3 view_vertical_vector;
    rt_vector3 view_topleft_vector;
};

void compute_vectors(rt_camera &);

void ray_for_pixel(rt_camera &, rt_vector2 &, rt_ray &);

#endif