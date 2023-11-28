#include "rt_graphics.h"

void rt_vbuf::render()
{
    compute_vectors(camera);
    buffer_length = camera.view_size_pixels.u * camera.view_size_pixels.v;
    
    rt_vector2 pixel = rt_vector2{ 0,0 };
    for (uint32_t p = 0; p < buffer_length; p++)
    {
        //render_pixel(pixel, p);

        pixel.u += 1;
        if (pixel.u >= camera.view_size_pixels.u)
        {
            pixel.u = 0;
            pixel.v += 1;
        }
    }
}
