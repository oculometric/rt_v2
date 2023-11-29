#include "rt_geometry.h"

void precompute_tri_constants(rt_tri * tri)
{
    sub(tri->v2, tri->v1, tri->v1_2);
    sub(tri->v3, tri->v1, tri->v1_3);

    cross(tri->v1_2, tri->v1_3, tri->n);
    norm_self(tri->n);

    tri->d12_12 = mag_sq(tri->v1_2);
    tri->d12_13 = dot(tri->v1_2, tri->v1_3);
    tri->d13_13 = mag_sq(tri->v1_3);

    tri->inv_denom = 1.0 / ((tri->d12_12 * tri->d13_13) - (tri->d12_13 * tri->d12_13));
}