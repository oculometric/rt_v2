#include "rt_geometry.h"
#include "rt_buffer.h"

// holds the result of a raycast check
struct rt_raycast_result
{
    rt_tri* hit_tri;    // pointer to the triangle that was hit
    float distance;     // distance along the ray at which the hit ocurred
    float r_dot_n;      // direction of the ray, dotted with the triangle normal
                        // if > 1, ray collides with the back of a face
                        // if < 1, ray collides with front of a face
};

// class which holds and manages a graphics buffer, including handling
// a triangle buffer, BVH tree structure, and casting rays into this structure
class rt_gbuf
{
private:
    // pointer to the first triangle buffer node, linked-list
    rt_tri_node* triangle_buffer;
    // pointer to the first top-level bvh buffer node, linked-list
    rt_bvh_node* bvh_buffer;
    
public:
    // near and far clip planes for rays
    float near_clip;
    float far_clip;

    // insert a set of 3 vertices, forming a triangle, into the graphics buffer
    // remember that indexed clockwise, the normal will point into the camera
    rt_tri* insert_triangle(rt_vector3&, rt_vector3&, rt_vector3&);

    // cast a ray into the graphics buffer, and find the triangle closest to the origin which the ray intersects with
    // return value will have LSB set if there was an intersection found
    uint8_t raycast(rt_ray&, rt_raycast_result&);

    // test for intersection with a specific triangle
    uint8_t check_triangle(rt_tri*, rt_ray&, rt_raycast_result&);
};