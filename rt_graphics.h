#ifndef RT_GRAPHICS_H
#define RT_GRAPHICS_H

#include "rt_raycast.h"
#include "rt_maths.h"
#include "rt_camera.h"

#define VBUF_COMPOSITE 0
#define VBUF_COLOUR 1
#define VBUF_NORMAL 2
#define VBUF_DEPTH 3

// TODO: dithering mode defines
// TODO: view transform mode defines

struct rt_options
{
    uint8_t samples_per_pixel;
    uint8_t dithering_mode;
    uint8_t view_transform;
};

class rt_vbuf
{
private:
    rt_colour * colour_buffer;
    float * depth_buffer;
    rt_colour * normal_buffer;
    rt_colour * composite_buffer;

    uint32_t buffer_length;

    void render_pixel(rt_vector2 &, uint32_t);

public:
    rt_camera camera;
    rt_simplesky sky;
    rt_simplesky background_illumination;
    rt_sun sun;
    rt_options options;
    rt_gbuf graphics_buffer;

    void render();
    void blit(uint8_t *, uint8_t);
};

#endif