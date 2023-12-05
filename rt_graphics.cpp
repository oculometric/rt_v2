#include "rt_graphics.h"
#include "rt_camera.h"

#include <iostream>
using namespace std;

void rt_vbuf::render_pixel(rt_vector2 & pixel, uint32_t buffer_pos)
{
    rt_colour colour{ 1,1,1 };
    rt_ray ray;
    rt_raycast_result rcr;
    rt_material * material;

    // initial raycast
    ray_for_pixel(camera, pixel, ray);
    graphics_buffer->raycast(ray, rcr);

    if (rcr.hit_obj != NULL)
    {
        // if intersected, reflect and recast
        colour = /*rcr.hit_tri->material->diffuse_colour*/ rt_colour{1,0,1};
    }
    else
    {
        // otherwise, sample the sky and terminate raycast
        sample_sky(sky, ray.direction, colour);
        depth_buffer[buffer_pos] = camera->far_clip;
        normal_buffer[buffer_pos] = ray.direction;
    }

    add(colour_buffer[buffer_pos], colour, colour_buffer[buffer_pos]);
}

void rt_vbuf::render()
{
    compute_vectors(camera);
    buffer_length = camera->view_size_pixels.u * camera->view_size_pixels.v;

    graphics_buffer->near_clip = camera->near_clip;
    graphics_buffer->far_clip = camera->far_clip;
    
    clean_up();

    colour_buffer = new rt_colour[buffer_length];
    normal_buffer = new rt_colour[buffer_length];
    composite_buffer = new rt_colour[buffer_length];
    depth_buffer = new float[buffer_length];

    rt_vector2 pixel = rt_vector2{ 0,0 };
    for (uint32_t p = 0; p < buffer_length; p++)
    {
        colour_buffer[p] = rt_vector3{0,0,0};
        
        for (uint32_t i = 0; i < samples_per_pixel; i++) render_pixel(pixel, p);
        div(colour_buffer[p], samples_per_pixel, colour_buffer[p]);

        pixel.u += 1;
        if (pixel.u >= camera->view_size_pixels.u)
        {
            pixel.u = 0;
            pixel.v += 1;
        }
    }

    composite();
}

void rt_vbuf::composite()
{
    // TODO: real compositing. for now we just copy from the colour buffer
    for (uint32_t i = 0; i < buffer_length; i++)
    {
        composite_buffer[i] = colour_buffer[i];
    }
}

void rt_vbuf::blit(uint8_t * out_buffer, uint8_t buffer_selection)
{
    if (buffer_selection == VBUF_DEPTH)
    {
        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[i] = (uint8_t)(((depth_buffer[i]-camera->near_clip)/(camera->far_clip-camera->near_clip))*255);
        }
    }
    else
    {
        rt_colour * in_buffer;
        if (buffer_selection == VBUF_COMPOSITE) in_buffer = composite_buffer;
        else if (buffer_selection == VBUF_COLOUR) in_buffer = colour_buffer;
        else if (buffer_selection == VBUF_NORMAL) in_buffer = normal_buffer;
        else return;

        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[(i*3) + 0] = (uint8_t)(in_buffer[i].x*255);
            out_buffer[(i*3) + 1] = (uint8_t)(in_buffer[i].y*255);
            out_buffer[(i*3) + 2] = (uint8_t)(in_buffer[i].z*255);
        }
    }
}

void rt_vbuf::clean_up()
{
    if (colour_buffer != NULL) delete[] colour_buffer;
    if (normal_buffer != NULL) delete[] normal_buffer;
    if (depth_buffer != NULL) delete[] depth_buffer;
    if (composite_buffer != NULL) delete[] composite_buffer;
}

rt_vbuf::rt_vbuf()
{
    camera = new rt_camera;
    sky = new rt_simplesky;
    sun = new rt_sun;
    graphics_buffer = new rt_gbuf;
}
