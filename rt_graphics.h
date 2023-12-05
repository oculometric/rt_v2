#ifndef RT_GRAPHICS_H
#define RT_GRAPHICS_H

#include "rt_raycast.h"
#include "rt_maths.h"
#include "rt_camera.h"

#define VBUF_COMPOSITE 0
#define VBUF_COLOUR 1
#define VBUF_NORMAL 2
#define VBUF_DEPTH 3
#define VBUF_INDEX 4

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
    rt_colour * colour_buffer = NULL;
    float * depth_buffer = NULL;
    rt_colour * normal_buffer = NULL;
    rt_colour * composite_buffer = NULL;
    uint16_t * index_buffer = NULL;

    // precalculated buffer length
    uint32_t buffer_length;

    // render one full sample of a single pixel in the buffer
    // takes both pixel position and pixel index
    void render_pixel(const rt_vector2 &, const uint32_t);

    // trace a ray through the scene, recursively
    void sample(const rt_ray &, const uint32_t, const uint16_t, rt_colour &);

    // slightly disturb a pixel position to randomise sampling
    void randomise_subpixel(rt_vector2 &);

    rt_raycast_result rcr;

public:
    // necessary scene objects
    rt_camera * camera = NULL;
    rt_simplesky * sky = NULL;
    rt_sun * sun = NULL;

    // graphics buffer which manages raycasting into a collection of objects
    rt_gbuf * graphics_buffer = NULL;

    // which buffers the config requested output for
    uint8_t buffers_to_output;

    // options for rendering
    // number of samples to perform for each pixel
    uint8_t samples_per_pixel = 1;
    // method of dithering for compositing
    uint8_t dithering_mode = 0;
    // render transform function to be used for compositing
    uint8_t view_transform = 0;
    // maximum depth of rays to trace
    uint16_t sample_depth = 2;
    // universal global illumination background colour (should be set to zero for photorealism)
    rt_colour background_colour{ 0,0,0 };

    // render the scene into the buffers
    void render();
    // apply compositing effects to produce final image
    void composite();
    // blit the contents of a renderer buffer into a block of memory
    // RGB 24bpp for all except depth, which is 8bpp
    void blit(uint8_t *, uint8_t);
    // delete render buffers
    void clean_up();
    // output buffers, according to buffers_to_output
    void output_buffers();

    // initialise
    rt_vbuf();
    // initialise from config file
    rt_vbuf(const char *);
};

#endif