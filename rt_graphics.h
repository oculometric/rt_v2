#ifndef RT_GRAPHICS_H
#define RT_GRAPHICS_H

#include "rt_raycast.h"
#include "rt_maths.h"
#include "rt_camera.h"

#define VBUF_COMPOSITE 0
#define VBUF_COLOUR 1
#define VBUF_NORMAL 2
#define VBUF_DEPTH 3

#define DITHER_NONE 0
#define DITHER_PATTERN 1
#define DITHER_NEIGHBOUR 2

#define VIEW_TRANSFORM_LINEAR 0
#define VIEW_TRANSFORM_FILMIC 1
#define VIEW_TRANSFORM_AGX 2

// rendering manager class
class rt_vbuf
{
private:
    // buffers for different passes
    rt_colour * colour_buffer;
    float * depth_buffer;
    rt_colour * normal_buffer;
    rt_colour * composite_buffer;

    // precalculated buffer length
    uint32_t buffer_length;

    // render a single pixel in the buffer
    // takes both pixel position and pixel index
    void render_pixel(rt_vector2 &, uint32_t);

public:
    // necessary scene objects
    rt_camera * camera;
    rt_simplesky * sky;
    rt_sun * sun;

    // graphics buffer which manages raycasting into a collection of objects
    rt_gbuf * graphics_buffer;

    // options for rendering
    // number of samples to perform for each pixel
    uint8_t samples_per_pixel;
    // method of dithering for compositing
    uint8_t dithering_mode;
    // render transform function to be used for compositing
    uint8_t view_transform;

    // render the scene into the buffers
    void render();
    // apply compositing effects to produce final image
    void composite();
    // blit the contents of a renderer buffer into a block of memory
    // RGB 24bpp for all except depth, which is 8bpp
    void blit(uint8_t *, uint8_t);
    // delete render buffers
    void clean_up();

    // initialise
    rt_vbuf();
};

#endif