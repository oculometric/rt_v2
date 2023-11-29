#include "rt_raycast.h"

uint8_t rt_gbuf::raycast(rt_ray &, rt_raycast_result &)
{
    return 0;
}

uint8_t rt_gbuf::check_triangle(rt_tri *tri, rt_ray &ray, rt_raycast_result &res)
{
    // check for parallelness to the plane
    float dot = tri->n ^ ray.direction;
    if (dot == 0) return 0x00;
    float backfacing = dot > 0 ? 1 : -1;
    if (backfacing > 0 && cull_backfaces) return 0x02;

    // check that the triangle is within clip ranges
    float dist = ((tri->v1 - ray.origin) ^ tri->n) / dot;
    if (dist < near_clip || dist > far_clip) return 0x06;

    // compute and check barycentric coordinates
    rt_vector3 p = (ray.direction * dist) + ray.origin;
    rt_vector3 v1p = p - tri->v1;
    float d1 = v1p ^ tri->v1_2;
    float d2 = v1p ^ tri->v1_3;

    float v = ((tri->d13_13 * d1) - (tri->d12_13 * d2)) * tri->inv_denom;
    if (v < 0 || v > 1) return 0x0E;
    float w = ((tri->d12_12 * d2) - (tri->d12_13 * d1)) * tri->inv_denom;
    if (w < 0 || w > 1) return 0x0E;
    float u = 1 - v - w;
    if (u < 0 || u > 1) return 0x0E;

    res.distance = dist;
    res.hit_tri = tri;
    res.r_dot_n = dot;
    res.point = p;

    return 0x0F;
}
