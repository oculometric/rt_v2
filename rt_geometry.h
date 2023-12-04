#ifndef RT_GEOMETRY_H
#define RT_GEOMETRY_H

#include "rt_vector.h"
#include "rt_material.h"

// represents a ray starting at an origin and travelling infinitely in a direction
struct rt_ray
{
    rt_vector3 origin, direction;
};

// represents a bounding box with bottom corner min and upper corner max
struct rt_bounds
{
    rt_vector3 min, max;
    rt_vector3 center,size;
};

// stores a block of triangles for the renderer to render
// for optimal performance, triangles should be distributed to objects
// according to their localisation in the scene
struct rt_object
{
    // lists all the vertices used by the object
    rt_vector3 * vertices;
    // stores indices of vertices used by each triangle, length should be 3 times the number of trianlges
    uint16_t * triangles;
    // stores normal data for each triangle, length should be equal to number of triangles
    rt_vector3 * normals;
    // list of pointers to materials used on this object
    rt_material ** materials;
    // indices of materials used by triangles, length should be equal to number of triangles
    uint16_t * material_indices;
    // stores UV coordinates for vertices, length should be 3 times the number of triangles
    rt_vector2 * uvs;

    // stores the 1->2 and 1->3 vectors for each triangle, length should be 2 times number of triangles. never directly touch this
    rt_vector3 * edge_vectors;
    // stores constants for raycasting, length should be 3 times number of triangles. never directly touch this
    float * edge_dots;
    // inverse denominators, length should be equal to number of triangles. never directly touch this
    float * inv_denoms;
    // bounding box of the object. never directly touch this
    rt_bounds bounds;

    // list length data for vertices and triangles. never directly touch these
    uint16_t vertices_count;
    uint16_t vertices_capacity;
    uint16_t triangles_count;
    uint16_t triangles_capacity;
};

// precompute information about triangles to accelerate rendering
// MUST be called before you start rendering
void precompute_tri_constants(rt_object *);

// load an object from a .obj file
rt_object * load_obj_file(const char *);

#endif