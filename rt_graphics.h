#ifndef RT_GRAPHICS_H
#define RT_GRAPHICS_H

#include "rt_raycast.h"
#include "rt_maths.h"
#include "rt_camera.h"

// TODO: dithering mode defines
// TODO: view transform mode defines

struct rt_options
{
    uint8_t samples_per_pixel;
    uint8_t dithering_mode;
    uint8_t view_transform;
    uint8_t sample_depth;
};

class rt_vbuf
{
private:
    rt_colour * colour_buffer;
    rt_colour * depth_buffer;
    rt_colour * normal_buffer;
    rt_colour * composite_buffer;

    uint32_t buffer_length;

    void render_pixel(rt_vector2 &, uint32_t);

public:
    rt_camera camera;
    rt_simplesky sky;
    rt_sun sun;
    rt_options options;
    rt_gbuf graphics_buffer;

    void render();
    void blit(uint8_t *, uint8_t, uint8_t);
};

#endif