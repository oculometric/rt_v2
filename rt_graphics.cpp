#include "rt_graphics.h"
#include "rt_camera.h"

#include <iostream>
using namespace std;

void rt_vbuf::render_pixel(rt_vector2 & pixel, uint32_t buffer_pos)
{
    rt_colour colour{ 1,1,1 };
    rt_ray ray;
    rt_raycast_result rcr;

    // initial raycast
    ray_for_pixel(camera, pixel, ray);
    graphics_buffer.raycast(ray, rcr);

    if (rcr.hit_tri != NULL)
    {
        cout << "HIT!" << endl;
        // if intersected, reflect and recast
        colour = /*rcr.hit_tri->material->diffuse_colour*/ rt_colour{0.5,0.5,0.5};
        reflect(ray.direction, rcr.hit_tri->n, ray.direction);
        ray.origin = rcr.point;

        depth_buffer[buffer_pos] = rcr.distance;
        normal_buffer[buffer_pos] = rcr.hit_tri->n;
        
        rcr.hit_tri = NULL;
        graphics_buffer.raycast(ray, rcr);

        // if intersected again, terminate raycast
        rt_colour backup_colour;
        if (rcr.hit_tri != NULL)
        {
            mul(colour, /*rcr.hit_tri->material->diffuse_colour*/ rt_colour{0.5,0.5,0.5}, colour);
            reflect(ray.direction, rcr.hit_tri->n, ray.direction);
            sample_sky(background_illumination, ray.direction, backup_colour);
            mul(colour, backup_colour, colour);
        }
        else
        {
            rt_colour sun_colour;
            sample_sky(sky, ray.direction, backup_colour);
            sample_sky(sky, ray.direction, sun_colour);
            add(sun_colour, backup_colour, backup_colour);
            mul(colour, backup_colour, colour);
        }
    }
    else
    {
        // otherwise, sample the sky and terminate raycast
        sample_sky(sky, ray.direction, colour);
        depth_buffer[buffer_pos] = camera.far_clip;
        normal_buffer[buffer_pos] = ray.direction;
    }

    colour_buffer[buffer_pos] = colour;
}

void rt_vbuf::render()
{
    compute_vectors(camera);
    buffer_length = camera.view_size_pixels.u * camera.view_size_pixels.v;

    graphics_buffer.cull_backfaces = false;
    graphics_buffer.near_clip = camera.near_clip;
    graphics_buffer.far_clip = camera.far_clip;
    
    colour_buffer = new rt_colour[buffer_length];
    normal_buffer = new rt_colour[buffer_length];
    composite_buffer = new rt_colour[buffer_length];
    depth_buffer = new float[buffer_length];
    // TODO: fix the obvious memory leak here


    // TODO: multiple samples per pixel
    rt_vector2 pixel = rt_vector2{ 0,0 };
    for (uint32_t p = 0; p < buffer_length; p++)
    {
        render_pixel(pixel, p);

        pixel.u += 1;
        if (pixel.u >= camera.view_size_pixels.u)
        {
            pixel.u = 0;
            pixel.v += 1;
        }
    }

    // TODO: composite, apply view transform
}

void rt_vbuf::blit(uint8_t * out_buffer, uint8_t buffer_selection)
{
    if (buffer_selection == VBUF_DEPTH)
    {
        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[i] = (uint8_t)(((depth_buffer[i]-camera.near_clip)/(camera.far_clip-camera.near_clip))*255);
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
