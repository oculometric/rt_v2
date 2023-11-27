#include "rt_camera.h"

void compute_vectors(rt_camera & cam)
{
    // normalise look direction
    norm_self(cam.look_direction);

    // calculate the vector which takes you from the top-left to the top-right corner of the camera plane
    // normalise it since the camera plane is always 1 metre wide
    cam.view_horizontal_vector = rt_vector3{ 0,0,1 } % cam.look_direction;
    norm_self(cam.view_horizontal_vector);
    // divide by the width in pixels to save a step later
    cam.view_horizontal_vector /= cam.view_size_pixels.u;

    // calculate the vector which takes you from the top-left to the bottom-left corner of the camera plane
    // normalise it, but then multiply it by the aspect ratio
    cam.view_vertical_vector = cam.view_horizontal_vector % cam.look_direction;
    norm_self(cam.view_vertical_vector);
    cam.view_vertical_vector *= cam.view_size_pixels.v / cam.view_size_pixels.u;
    // divide by the height in pixels to save a step later
    cam.view_vertical_vector /= cam.view_size_pixels.v;

    // calculate the vector which takes you from the camera origin to the top left corner
    cam.view_topleft_vector = (cam.look_direction * tanf(cam.field_of_view / 2)) - (cam.view_horizontal_vector + cam.view_vertical_vector);
    cam.view_topleft_vector /= 2;
}

void ray_for_pixel(rt_camera & cam, rt_vector2 & uv, rt_ray & ray)
{
    ray.origin = cam.position;
    ray.direction = cam.view_topleft_vector + (cam.view_horizontal_vector * uv.u) + (cam.view_horizontal_vector * uv.v);
}