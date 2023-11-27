#include "rt_geometry.h"

void precompute_tri_constants(rt_tri * tri)
{
    tri->v1_2 = tri->v2 - tri->v1;
    tri->v1_3 = tri->v3 - tri->v1;

    tri->n = tri->v1_2 % tri->v1_3;
    norm_self(tri->n);

    tri->d12_12 = tri->v1_2 ^ tri->v1_2;
    tri->d12_13 = tri->v1_2 ^ tri->v1_3;
    tri->d13_13 = tri->v1_3 ^ tri->v1_3;

    tri->inv_denom = 1.0 / ((tri->d12_12 * tri->d13_13) - (tri->d12_13 * tri->d12_13));
}