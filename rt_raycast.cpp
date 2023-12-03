#include "rt_raycast.h"

rt_tri * rt_gbuf::insert_triangle(rt_vector3 & v1, rt_vector3 & v2, rt_vector3 & v3)
{
    rt_tri * tri = new rt_tri;
    tri->v1 = v1;
    tri->v2 = v2;
    tri->v3 = v3;
    precompute_tri_constants(tri);

    rt_tri_node * tri_node = new rt_tri_node;
    tri_node->next = NULL;
    tri_node->tri = tri;

    if (triangle_buffer == NULL)
    {
        tri_node->last = NULL;
        triangle_buffer = tri_node;
    }
    else
    {
        rt_tri_node * last = triangle_buffer;
        while (last->next != NULL) last = last->next;
        last->next = tri_node;
        tri_node->last = last;
    }

    return tri;
}

uint8_t rt_gbuf::raycast(rt_ray & ray, rt_raycast_result & res)
{
    res.distance = far_clip;
    res.hit_tri = NULL;
    res.point = rt_vector3{0,0,0};
    res.r_dot_n = 0;
    
    float far_clip_tmp = far_clip;
    
    rt_tri_node * next_tri = triangle_buffer;
    uint8_t status = 0;
    uint8_t best_status = 0;
    while (next_tri != NULL)
    {
        status = check_triangle(next_tri->tri, ray, res);
        if (status == RAYCAST_VALID_INTERSECTION)
        {
            far_clip = res.distance;
        }
        if (status > best_status) status = best_status;
        next_tri = next_tri->next;
    }
    far_clip = far_clip_tmp;
    return status;
}

uint8_t rt_gbuf::check_triangle(rt_tri * tri, rt_ray & ray, rt_raycast_result & res)
{
    rt_vector3 tmp;
    // check for parallelness to the plane
    float dt;
    dot(tri->n, ray.direction, dt);
    if (dt == 0) return 0x00;
    float backfacing = dt > 0 ? 1 : -1;
    if (backfacing > 0 && cull_backfaces) return 0x02;

    // check that the triangle is within clip ranges
    sub(tri->v1, ray.origin, tmp);
    float dist;
    dot(tmp, tri->n, dist);
    dist /= dt;
    if (dist < near_clip || dist > far_clip) return 0x06;

    // compute and check barycentric coordinates
    mul(ray.direction, dist, tmp);
    add(tmp, ray.origin, tmp);
    rt_vector3 tmp2;
    sub(tmp, tri->v1, tmp2);
    float d1;
    float d2;
    dot(tmp2, tri->v1_2, d1);
    dot(tmp2, tri->v1_3, d2);

    float v = ((tri->d13_13 * d1) - (tri->d12_13 * d2)) * tri->inv_denom;
    if (v < 0 || v > 1) return 0x0E;
    float w = ((tri->d12_12 * d2) - (tri->d12_13 * d1)) * tri->inv_denom;
    if (w < 0 || w > 1) return 0x0E;
    float u = 1 - v - w;
    if (u < 0 || u > 1) return 0x0E;

    res.distance = dist;
    res.hit_tri = tri;
    res.r_dot_n = dt;
    res.point = tmp;

    return 0x0F;
}
