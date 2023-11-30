#ifndef RT_MATHS_H
#define RT_MATHS_H

#include <stdint.h>
#include <stddef.h>

#define M_E         2.718281828

#define M_PI        3.141592654
#define M_PI_2      1.570796327
#define M_2_PI      6.283185307

#define _TAN_F_1    0.3333333333
#define _TAN_F_2    0.05396825397
#define _TAN_F_3    0.02222222222
#define _TAN_F_4    0.008863235530
#define _TAN_F_5    0.003592128037
#define _TAN_F_6    0.001455834387

#define _ASIN_F_1   0.1666666667
#define _ASIN_F_2   0.04464285714
#define _ASIN_F_3   0.03038194444
#define _ASIN_F_4   0.02237215909
#define _ASIN_F_5   0.01735276442
#define _ASIN_F_6   0.01396484375

static inline void tanf(const float & f, float & o)
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
    o = acc;
}

static inline void asinf(const float & f, float & o)
{
    float t = f;
    float acc = t;
    t *= f*f;
    acc += t*_ASIN_F_1;
    t *= f*f;
    acc += t*_ASIN_F_2;
    t *= f*f;
    acc += t*_ASIN_F_3;
    t *= f*f;
    acc += t*_ASIN_F_4;
    t *= f*f;
    acc += t*_ASIN_F_5;
    t *= f*f;
    acc += t*_ASIN_F_6;
    o = acc;
}

static inline float expf(const float & f)
{
	if (f > 0) return 0;

	float x = -f;
	float r = 1.0f, c = 1.0f; 
	for (int i = 1; i < 8; ++i) {
		c *= x / i;
		r += c;
	}

	return 1.0f / r;
}

static inline float logf(const float & f)
{
    if (f < 0 || f > 1) return 0;

	if (f == 0.0f) {
		return 0; // should be infinity
	}

	float p = 0.0f;
    float x = f;
	while (x < 0.5f) {
		x *= M_E;
		++p;
	}

	x -= 1.0f;
	float r = 0.0f, c = -1.0f;
	for (int i = 1; i < 8; ++i) {
		c *= -x;
		r += c / i;
	}

	return r - p;
}

static inline float powf(float base, float & exponent)
{
    if (base <= 0) return 0;
        return 0.0f;
	return expf(exponent * logf(base));
}

static inline void absf(const float & a, float & o) { o = a > 0 ? a : -a; }

static inline void sqrt(const float & x, float & o)
{
    if (x < 0)
    {
        o = 0;
        return;
    }
    if (x < 2)
    {
        o = x;
        return;
    }
 
    float y = x;
    float z = (y + (x / y)) / 2;
    float w = y-z; absf(w,w);
 
    while (w >= 0.00001) {
        y = z;
        z = (y + (x / y)) / 2;
        w = y-z; absf(w,w);
    }
    o = z;
}

// TODO: powf, sqrt, asinf, acosf (possibly eliminate this)

// clamp a floating point value to be greater than or equal to 0
static inline float clamp0(const float & f) { return f < 0 ? 0 : f; }

#endif