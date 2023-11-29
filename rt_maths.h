#ifndef RT_MATHS_H
#define RT_MATHS_H


#define M_PI        3.141592654
#define M_PI_2      1.570796327
#define M_2_PI      6.283185307

#define _TAN_F_1    0.3333333333
#define _TAN_F_2    0.05396825397
#define _TAN_F_3    0.02222222222
#define _TAN_F_4    0.008863235530
#define _TAN_F_5    0.003592128037
#define _TAN_F_6    0.001455834387

static inline float tanf(const float & f)
{
    float t = f;
    float acc = t;
    t *= f*f;
    acc += t*_TAN_F_1;
    t *= f*f;
    acc += t*_TAN_F_2;
    t *= f*f;
    acc += t*_TAN_F_3;
    t *= f*f;
    acc += t*_TAN_F_4;
    t *= f*f;
    acc += t*_TAN_F_5;
    t *= f*f;
    acc += t*_TAN_F_6;
    return acc;
}

#include <stdint.h>
#include <stddef.h>

// clamp a floating point value to be greater than or equal to 0
static inline float clamp0(const float & f) { return f < 0 ? 0 : f; }

#endif