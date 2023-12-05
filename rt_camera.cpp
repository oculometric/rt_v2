#include "rt_camera.h"

void compute_vectors(rt_camera * cam)
{
    // normalise look direction
    norm_self(cam->look_direction);

    // calculate the vector which takes you from the top-left to the top-right corner of the camera plane
    // normalise it since the camera plane is always 1 metre wide
    cross(rt_vector3{ 0,0,1 }, cam->look_direction, cam->view_horizontal_vector);
    norm_self(cam->view_horizontal_vector);

    // calculate the vector which takes you from the top-left to the bottom-left corner of the camera plane
    // normalise it, but then multiply it by the aspect ratio
    cross(cam->view_horizontal_vector, cam->look_direction, cam->view_vertical_vector);
    norm_self(cam->view_vertical_vector);
    mul(cam->view_vertical_vector, cam->view_size_pixels.v / cam->view_size_pixels.u, cam->view_vertical_vector);
    

    // calculate the vector which takes you from the camera origin to the top left corner
    float f = cam->field_of_view / 2;
    f = tanf(f);
    mul(cam->look_direction, f, cam->view_topleft_vector);
    sub(cam->view_topleft_vector, cam->view_horizontal_vector, cam->view_topleft_vector);
    sub(cam->view_topleft_vector, cam->view_vertical_vector, cam->view_topleft_vector);

    div(cam->view_topleft_vector, 2, cam->view_topleft_vector);

    // divide by the width in pixels to save a step later
    div(cam->view_horizontal_vector, cam->view_size_pixels.u, cam->view_horizontal_vector);
    // divide by the height in pixels to save a step later
    div(cam->view_vertical_vector, cam->view_size_pixels.v, cam->view_vertical_vector);
}

void ray_for_pixel(const rt_camera * cam, const rt_vector2 & uv, rt_ray & ray)
{
    ray.origin = cam->position;
    rt_vector3 tmp;
    mul(cam->view_horizontal_vector, uv.u, tmp);
    add(cam->view_topleft_vector, tmp, ray.direction);
    mul(cam->view_vertical_vector, uv.v, tmp);
    add(ray.direction, tmp, ray.direction);
    norm_self(ray.direction);
    div(rt_vector3{1,1,1}, ray.direction, ray.direction_inverse);
}