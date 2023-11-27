#ifndef RT_GRAPHICS_H
#define RT_GRAPHICS_H

#include "rt_raycast.h"
#include "rt_maths.h"

struct rt_camera
{
    float near_clip;
    float far_clip;

    float fog_start;
    float fog_end;

    float field_of_view;

    rt_vector3 position;
    rt_vector3 look_direction;

    float brightness;
    float gamma;
};

class rt_vbuf
{
private:
    rt_colour* colour_buffer;
    rt_colour* depth_buffer;
    rt_colour* normal_buffer;
    rt_colour* composite_buffer;

public:
    rt_camera camera;
    rt_gbuf graphics_buffer;

    void render();
    void blit(uint8_t*);
};

#endif