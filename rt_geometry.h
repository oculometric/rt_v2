#ifndef RT_GEOMETRY_H
#define RT_GEOMETRY_H

#include <vector>
#include <string>

#include "rt_vector.h"
#include "rt_material.h"

// represents a ray starting at an origin and travelling infinitely in a direction
struct rt_ray
{
    rt_vector3 origin, direction;
    rt_vector3 direction_inverse;
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
    rt_vector3 * vertices = NULL;
    // stores indices of vertices used by each triangle, length should be 3 times the number of triangles
    uint16_t * triangles = NULL;
    // list of pointers to materials used on this object
    rt_material ** materials = NULL;
    // indices of materials used by triangles, length should be equal to number of triangles
    uint16_t * material_indices = NULL;
    // stores UV coordinates for vertices, length should be 3 times the number of triangles
    rt_vector2 * uvs = NULL;
    // stores normals for vertices, used for surface smoothing. length should be 3 times number of triangles
    rt_vector3 * vertex_normals = NULL;

    // stores normal data for each triangle, length should be equal to number of triangles. never directly touch this
    rt_vector3 * normals = NULL;
    // stores the 1->2 and 1->3 vectors for each triangle, length should be 2 times number of triangles. never directly touch this
    rt_vector3 * edge_vectors = NULL;
    // stores constants for raycasting, length should be 3 times number of triangles. never directly touch this
    float * edge_dots = NULL;
    // inverse denominators, length should be equal to number of triangles. never directly touch this
    float * inv_denoms = NULL;
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

// split a string at a delimiter
void split(const std::string, std::vector<std::string> &, const char);

// calculate the uv coordinate for a barycentric point, interpolated across a triangle
void calculate_uv(const rt_object *, const uint16_t, const rt_vector3 &, rt_vector2 &);

// calculate the normal vector for a barycentric point, interpolated across a triangle
void calculate_normal(const rt_object *, const uint16_t, const rt_vector3 &, rt_vector3 &);

#endif