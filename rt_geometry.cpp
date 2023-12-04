#include "rt_geometry.h"

#include <iostream>
#include <fstream>

void precompute_tri_constants(rt_object * obj)
{
    // reallocate necessary buffers
    if (obj->edge_vectors != NULL) delete[] obj->edge_vectors;
    if (obj->edge_dots != NULL) delete[] obj->edge_dots;
    if (obj->inv_denoms != NULL) delete[] obj->inv_denoms;
    if (obj->normals != NULL) delete[] obj->normals;

    uint16_t triangles_num = obj->triangles_count/3;

    obj->edge_vectors = new rt_vector3[2*triangles_num];
    obj->edge_dots = new float[3*triangles_num];
    obj->inv_denoms = new float[triangles_num];
    obj->normals = new rt_vector3[triangles_num];

    // iterate over triangles and calculate constants
    rt_vector3 v1,v2,v3,v1_2,v1_3,n;
    float d12_12,d12_13,d13_13;
    for (uint32_t i = 0; i < triangles_num; i++)
    {
        v1 = obj->vertices[obj->triangles[i]];
        v2 = obj->vertices[obj->triangles[i+1]];
        v3 = obj->vertices[obj->triangles[i+2]];

        // calculate v1->v2, v1->v3
        sub(v2, v1, v1_2);
        obj->edge_vectors[i] = v1_2;
        sub(v3, v1, v1_3);
        obj->edge_vectors[i+1] = v1_3;

        // calculate normal
        cross(v1_2, v1_3, n);
        obj->normals[i] = n;

        // calculate v1_2^v1_2, v1_2^v1_3, v1_3^v1_3
        mag_sq(v1_2, d12_12);
        obj->edge_dots[i] = d12_12;
        dot(v1_2, v1_3, d12_13);
        obj->edge_dots[i+1] = d12_13;
        mag_sq(v1_3, d13_13);
        obj->edge_dots[i+2] = d13_13;

        // calculate inverse denominator
        obj->inv_denoms[i] = 1.0 / ((d12_12 * d13_13) - (d12_13 * d12_13));
    }

    // calculate object bounding box
    obj->bounds.min = obj->vertices[0];
    obj->bounds.max = obj->vertices[0];
    for (uint32_t i = 0; i < obj->vertices_count; i++)
    {
        v1 = obj->vertices[i];
        if (v1.x > obj->bounds.max.x) obj->bounds.max.x = v1.x;
        if (v1.y > obj->bounds.max.y) obj->bounds.max.y = v1.y;
        if (v1.z > obj->bounds.max.z) obj->bounds.max.z = v1.z;

        if (v1.y < obj->bounds.min.y) obj->bounds.min.y = v1.y;
        if (v1.z < obj->bounds.min.z) obj->bounds.min.z = v1.z;
        if (v1.x < obj->bounds.min.x) obj->bounds.min.x = v1.x;
    }

    sub(obj->bounds.max, obj->bounds.min, obj->bounds.size);
    div(obj->bounds.size, 2, obj->bounds.size);

    add(obj->bounds.min, obj->bounds.size, obj->bounds.center);
}

using namespace std;

rt_object * load_obj_file(const char * path)
{
    ifstream file;
    file.open(path);
    if (!file.is_open()) return NULL;

    uint16_t found_vertices = 0;
    uint16_t found_triangles = 0;

    string line;
    while(getline(file, line))
    {

    }
}
