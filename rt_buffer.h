#ifndef RT_BUFFER_H
#define RT_BUFFER_H

#include "rt_geometry.h"

struct rt_tri_node
{
    rt_tri_node* last = NULL;
    rt_tri* tri = NULL;
    rt_tri_node* next = NULL;
};

struct rt_bvh_node;

struct rt_bvh_node
{
    rt_bvh_node* last = NULL;
    rt_bvh_node* children = NULL;
    rt_tri_node* tri_buffer = NULL;
    rt_bvh_node* next = NULL;
    rt_bounds bounds;
};

#endif