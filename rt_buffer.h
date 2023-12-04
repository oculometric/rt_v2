#ifndef RT_BUFFER_H
#define RT_BUFFER_H

#include "rt_geometry.h"

template <typename T>
struct rt_buffer_node
{
    rt_buffer_node * last;
    rt_buffer_node * next;
    T data;
};

template <typename T>
uint32_t size(rt_buffer_node<T> *);

#endif