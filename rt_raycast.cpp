#include "rt_raycast.h"

void rt_gbuf::insert_object(const rt_object * obj)
{
    rt_buffer_node<rt_object *> * next_node = new rt_buffer_node<rt_object *>;
    next_node->next = NULL;
    next_node->data = (rt_object *)obj;

    if (object_buffer == NULL)
    {
        // object buffer not initialised, so initialise it
        next_node->last = NULL;
        object_buffer = next_node;
    }
    else
    {
        // add new node to end of object buffer
        rt_buffer_node<rt_object *> * last_node = object_buffer;
        while (last_node->next != NULL) last_node = last_node->next;
        last_node->next = next_node;
        next_node->last = last_node;
    }
}

uint8_t rt_gbuf::raycast(const rt_ray & ray, rt_raycast_result & res)
{
    // clear raycast result
    res.distance = far_clip;
    res.hit_tri = 0;
    res.hit_obj = NULL;
    res.point = rt_vector3{0,0,0};
    res.r_dot_n = 0;
    
    // make a copy of the far clip
    float far_clip_tmp = far_clip;
    
    // iterate over object buffer, keeping track of raycast status
    rt_buffer_node<rt_object *> * next_object = object_buffer;
    uint8_t status = 0;
    uint8_t best_status = 0;
    while (next_object != NULL)
    {
        // check that ray will be able to intersect object at all
        if (check_bounds(next_object->data->bounds, ray))
        {
            // raycast against every triangle in the object
            for (uint16_t i = 0; i < next_object->data->triangles_count/3; i++)
            {
                status = check_triangle(next_object->data, i, ray, res);
                // if an intersection was found successfully, limit the far clip to be the distance to this triangle
                // this means that on the next intersection, triangles which are further away will be excluded earlier
                if (status == RAYCAST_VALID_INTERSECTION)
                {
                    far_clip = res.distance;
                }
                if (status > best_status) status = best_status;
            }
        }
        next_object = next_object->next;
    }
    // reset far clip
    far_clip = far_clip_tmp;

    // return the best raycast status that we found
    return best_status;
}

uint8_t rt_gbuf::check_triangle(const rt_object * obj, uint16_t tri_index, const rt_ray & ray, rt_raycast_result & res)
{
    rt_vector3 tmp;
    rt_vector3 n = obj->normals[tri_index];

    // check for parallelness to the plane
    float dt;
    dot(n, ray.direction, dt);
    if (dt == 0) return 0x00;
    float backfacing = dt > 0 ? 1 : -1;
    if (backfacing > 0 && cull_backfaces) return 0x02;

    rt_vector3 v1 = obj->vertices[obj->triangles[tri_index*3]];

    // check that the triangle is within clip ranges
    sub(v1, ray.origin, tmp);
    float dist;
    dot(tmp, n, dist);
    dist /= dt;
    if (dist < near_clip || dist > far_clip) return 0x06;

    // compute barycentric coordinates
    mul(ray.direction, dist, tmp);
    add(tmp, ray.origin, tmp);
    rt_vector3 tmp2;
    sub(tmp, v1, tmp2);
    float d1;
    float d2;
    dot(tmp2, obj->edge_vectors[tri_index*2], d1);
    dot(tmp2, obj->edge_vectors[(tri_index*2)+1], d2);

    float d12_13 = obj->edge_dots[(tri_index*3)+1];
    float inv_denom = obj->inv_denoms[tri_index];

    // check hit point is within triangle
    float v = ((obj->edge_dots[(tri_index*3)+2] * d1) - (d12_13 * d2)) * inv_denom;
    if (v < 0 || v > 1) return 0x0E;
    float w = ((obj->edge_dots[(tri_index*3)+0] * d2) - (d12_13 * d1)) * inv_denom;
    if (w < 0 || w > 1) return 0x0E;
    float u = 1 - v - w;
    if (u < 0 || u > 1) return 0x0E;

    // clobber result with successful hit info
    res.distance = dist;
    res.hit_tri = tri_index;
    res.hit_obj = (rt_object *)obj;
    res.r_dot_n = dt;
    res.point = tmp;

    return 0x0F;
}
