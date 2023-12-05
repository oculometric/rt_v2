#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "rt_vector.h"
#include "rt_geometry.h"

struct rt_camera
{
    float near_clip = 0.02f;
    float far_clip = 100.0f;

    float fog_start = 0.02f;
    float fog_end = 90.0f;

    float field_of_view = 90.0f;

    rt_vector3 position {0,0,0};
    rt_vector3 look_direction {1,0,0};

    rt_vector2 view_size_pixels {4096,3072};

    float exposure = 1;
    float gamma = 2.2;

    // from here on these values should not be set directly
    rt_vector3 view_horizontal_vector;
    rt_vector3 view_vertical_vector;
    rt_vector3 view_topleft_vector;
};

void compute_vectors(rt_camera *);

void ray_for_pixel(const rt_camera *, const rt_vector2 &, rt_ray &);

#endif