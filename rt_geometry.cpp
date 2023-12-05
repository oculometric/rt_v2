#include "rt_geometry.h"

#include <iostream>
#include <fstream>
using namespace std;
void precompute_tri_constants(rt_object * obj)
{
    // reallocate necessary buffers
    if (obj->edge_vectors != NULL) { delete[] obj->edge_vectors; obj->edge_vectors = NULL; }
    if (obj->edge_dots != NULL) { delete[] obj->edge_dots; obj->edge_dots = NULL; }
    if (obj->inv_denoms != NULL) { delete[] obj->inv_denoms; obj->inv_denoms = NULL; }
    if (obj->normals != NULL) { delete[] obj->normals; obj->normals = NULL; }

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
        v1 = obj->vertices[obj->triangles[(i*3)]];
        v2 = obj->vertices[obj->triangles[(i*3)+1]];
        v3 = obj->vertices[obj->triangles[(i*3)+2]];

        // calculate v1->v2, v1->v3
        sub(v2, v1, v1_2);
        obj->edge_vectors[(i*2)] = v1_2;
        sub(v3, v1, v1_3);
        obj->edge_vectors[(i*2)+1] = v1_3;

        // calculate normal
        cross(v1_2, v1_3, n);
        obj->normals[i] = n;

        // assign vertex normals, if they're blank (i.e. {0,0,0})
        if (obj->vertex_normals[(i*3)] == rt_vector3{0,0,0}) obj->vertex_normals[(i*3)] = n;
        if (obj->vertex_normals[(i*3)+1] == rt_vector3{0,0,0}) obj->vertex_normals[(i*3)+1] = n;
        if (obj->vertex_normals[(i*3)+2] == rt_vector3{0,0,0}) obj->vertex_normals[(i*3)+2] = n;

        // calculate v1_2^v1_2, v1_2^v1_3, v1_3^v1_3
        mag_sq(v1_2, d12_12);
        obj->edge_dots[(i*3)] = d12_12;
        dot(v1_2, v1_3, d12_13);
        obj->edge_dots[(i*3)+1] = d12_13;
        mag_sq(v1_3, d13_13);
        obj->edge_dots[(i*3)+2] = d13_13;

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

rt_object * load_obj_file(const char * path)
{
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) return NULL;

    uint16_t found_vertices = 0;
    uint16_t found_triangles = 0;
    uint16_t found_uvs = 0;
    uint16_t found_vnorms = 0;

    // prepass to count how many of each we need to allocate for
    std::string line;
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'v' && line[1] == ' ')
        {
            found_vertices++;
        }
        if (line[0] == 'f' && line[1] == ' ')
        {
            found_triangles++;
        }
        if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            found_uvs++;
        }
        if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            found_vnorms++;
        }
    }

    // allocate space for data
    rt_object * obj = new rt_object;

    obj->vertices = new rt_vector3[found_vertices];
    obj->vertices_count = found_vertices;
    obj->vertices_capacity = found_vertices;

    obj->triangles = new uint16_t[found_triangles*3];
    obj->triangles_count = found_triangles*3;
    obj->triangles_capacity = found_triangles*3;

    obj->material_indices = new uint16_t[found_triangles];

    obj->uvs = new rt_vector2[found_triangles*3];
    
    obj->vertex_normals = new rt_vector3[found_triangles*3];

    rt_vector2 * uv_temp = new rt_vector2[found_uvs];
    rt_vector3 * vnorm_temp = new rt_vector3[found_vnorms];

    // process file for realsies, but ignoring faces for now
    file.clear();
    file.seekg(0, std::ios::beg);
    int v = 0;
    int vt = 0;
    int vn = 0;
    int f = 0;
    rt_vector3 tmp3;
    rt_vector2 tmp2;
    std::vector<std::string> split_line;
    std::vector<std::string> split_section;
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'v' && line[1] == ' ')
        {   // read vertex data
            split(line, split_line, ' ');
            if (split_line.size() < 4) { v++; continue; }
            tmp3.x = stof(split_line[1]);
            tmp3.y = stof(split_line[2]);
            tmp3.z = stof(split_line[3]);
            obj->vertices[v] = tmp3;
            v++;
        }
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {   // read uv data
            split(line, split_line, ' ');
            if (split_line.size() < 3) { vt++; continue; }
            tmp2.u = stof(split_line[1]);
            tmp2.v = stof(split_line[2]);
            uv_temp[vt] = tmp2;
            vt++;
        }
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {   // read vertex normal data
            split(line, split_line, ' ');
            if (split_line.size() < 4) { vn++; continue; }
            tmp3.x = stof(split_line[1]);
            tmp3.y = stof(split_line[2]);
            tmp3.z = stof(split_line[3]);
            norm_self(tmp3);
            vnorm_temp[vn] = tmp3;
            vn++;
        }
    }

    // TODO: handle materials

    // process faces
    file.clear();
    file.seekg(0, std::ios::beg);
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'f' && line[1] == ' ')
        {   // read face data. we only handle triangles here
            split(line, split_line, ' ');
            if (split_line.size() < 4) { f++; continue; }
            // process first vertex data
            split(split_line[1], split_section, '/');
            obj->triangles[f*3] = stoi(split_section[0])-1;
            if (split_section.size() > 1)
            {
                if (split_section[1] != "") obj->uvs[f*3] = uv_temp[stoi(split_section[1])];
                else obj->uvs[f*3] = rt_vector2{0,0};
            }
            if (split_section.size() > 2)
            {
                if (split_section[2] != "") obj->vertex_normals[f*3] = vnorm_temp[stoi(split_section[2])];
                else obj->vertex_normals[f*3] = rt_vector3{0,0,0};
            }
            // process second vertex data
            split(split_line[2], split_section, '/');
            obj->triangles[(f*3)+1] = stoi(split_section[0])-1;
            if (split_section.size() > 1)
            {
                if (split_section[1] != "") obj->uvs[(f*3)+1] = uv_temp[stoi(split_section[1])];
                else obj->uvs[(f*3)+1] = rt_vector2{0,0};
            }
            if (split_section.size() > 2)
            {
                if (split_section[2] != "") obj->vertex_normals[(f*3)+1] = vnorm_temp[stoi(split_section[2])];
                else obj->vertex_normals[(f*3)+1] = rt_vector3{0,0,0};
            }
            // process third vertex data
            split(split_line[3], split_section, '/');
            obj->triangles[(f*3)+2] = stoi(split_section[0])-1;
            if (split_section.size() > 1)
            {
                if (split_section[1] != "") obj->uvs[(f*3)+2] = uv_temp[stoi(split_section[1])];
                else obj->uvs[(f*3)+2] = rt_vector2{0,0};
            }
            if (split_section.size() > 2)
            {
                if (split_section[2] != "") obj->vertex_normals[(f*3)+2] = vnorm_temp[stoi(split_section[2])];
                else obj->vertex_normals[(f*3)+2] = rt_vector3{0,0,0};
            }

            f++;
        }
    }

    file.close();
    delete[] uv_temp;
    delete[] vnorm_temp;

    // calculate other data about the mesh
    precompute_tri_constants(obj);

    return obj;
}

void split(const std::string line, std::vector<std::string> & split_line, const char delim)
{
    std::string section;
    split_line.clear();
    for (char c : line)
    {
        if (c == delim)
        {
            split_line.push_back(section);
            section.clear();
        }
        else
        {
            section += c;
        }
    }
    if (section != "") split_line.push_back(section);
}

void calculate_normal(const rt_object * obj, const uint16_t tri, const rt_vector3 & baryc, rt_vector3 & normal)
{
    rt_vector3 norm_tmp;
    mul(obj->vertex_normals[(tri*3)+0], baryc.x, norm_tmp);
    normal = norm_tmp;
    mul(obj->vertex_normals[(tri*3)+1], baryc.y, norm_tmp);
    add(normal, norm_tmp, normal);
    mul(obj->vertex_normals[(tri*3)+2], baryc.z, norm_tmp);
    add(normal, norm_tmp, normal);
}

void calculate_uv(const rt_object * obj, const uint16_t tri, const rt_vector3 & baryc, rt_vector2 & uv)
{
    rt_vector2 uv_tmp;
    mul(obj->uvs[(tri*3)+0], baryc.x, uv_tmp);
    uv = uv_tmp;
    mul(obj->uvs[(tri*3)+1], baryc.y, uv_tmp);
    add(uv, uv_tmp, uv);
    mul(obj->uvs[(tri*3)+2], baryc.z, uv_tmp);
    add(uv, uv_tmp, uv);
}