#include "rt_graphics.h"
#include "rt_camera.h"

void rt_vbuf::render_pixel(rt_vector2 & pixel, uint32_t buffer_pos)
{
    rt_colour colour{ 1,1,1 };
    rt_ray ray;
    rt_raycast_result rcr{ NULL,0,0,{ 0,0,0 } };

    // initial raycast
    ray_for_pixel(camera, pixel, ray);
    graphics_buffer.raycast(ray, rcr);

    if (rcr.hit_tri != NULL)
    {
        // if intersected, reflect and recast
        colour *= rcr.hit_tri->material->diffuse_colour;
        ray.direction = reflect(ray.direction, rcr.hit_tri->n);
        ray.origin = rcr.point;

        depth_buffer[buffer_pos] = rcr.distance;
        normal_buffer[buffer_pos] = rcr.hit_tri->n;
        
        rcr.hit_tri = NULL;
        graphics_buffer.raycast(ray, rcr);

        // if intersected again, terminate raycast
        rt_colour backup_colour;
        if (rcr.hit_tri != NULL)
        {
            colour *= rcr.hit_tri->material->diffuse_colour;
            ray.direction = reflect(ray.direction, rcr.hit_tri->n);
            sample_sky(background_illumination, ray.direction, backup_colour);
            colour *= backup_colour;
        }
        else
        {
            rt_colour sun_colour;
            sample_sky(sky, ray.direction, backup_colour);
            sample_sky(sky, ray.direction, sun_colour);
            colour *= sun_colour + backup_colour;
        }
    }
    else
    {
        // otherwise, sample the sky and terminate raycast
        sample_sky(sky, ray.direction, colour);
    }
    
    colour_buffer[buffer_pos] = colour;
}

void rt_vbuf::render()
{
    compute_vectors(camera);
    buffer_length = camera.view_size_pixels.u * camera.view_size_pixels.v;
    
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
}
