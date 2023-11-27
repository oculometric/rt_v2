#ifndef RT_MATHS_H
#define RT_MATHS_H

// TODO: replace these includes with own implementations
#include <math.h>
#include <stdint.h>

// clamp a floating point value to be greater than or equal to 0
inline float clamp0(float f) { return f < 0 ? 0 : f; }

#endif