#ifndef RT_MATHS_H
#define RT_MATHS_H

#include <stdint.h>
#include <stddef.h>

#include <math.h>
// clamp a floating point value to be greater than or equal to 0
static inline float clamp0(const float & f) { return f < 0 ? 0 : f; }
static inline float max(const float a, const float b) { return a > b ? a : b; }
static inline float min(const float a, const float b) { return a < b ? a : b; }

#endif