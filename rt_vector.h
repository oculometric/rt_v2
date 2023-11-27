#ifndef RT_VECTOR_H
#define RT_VECTOR_H

#include "rt_maths.h"

// represents a 3 dimensional vector, with associated operations
struct rt_vector3
{
    float x, y, z;

    inline void operator=(const rt_vector3 & v) { this->x = v.x; this->y = v.y; this->z = v.z; }
    inline void operator+=(const rt_vector3 & v) { this->x += v.x; this->y += v.y; this->z += v.z; }
    inline void operator-=(const rt_vector3 & v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }
    inline void operator*=(const rt_vector3 & v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }
    inline void operator/=(const rt_vector3 & v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }
    inline void operator*=(const float f) { this->x *= f; this->y *= f; this->z *= f; }
    inline void operator/=(const float f) { this->x /= f; this->y /= f; this->z /= f; }
    inline rt_vector3 operator -() { return rt_vector3{ -this->x, -this->y, -this->z }; }
};

static inline rt_vector3 operator+(const rt_vector3 & v, const rt_vector3 & w) { return rt_vector3{ v.x+w.x, v.y+w.y, v.z+w.z }; }
static inline rt_vector3 operator-(const rt_vector3 & v, const rt_vector3 & w) { return rt_vector3{ v.x-w.x, v.y-w.y, v.z-w.z }; }
static inline rt_vector3 operator*(const rt_vector3 & v, const rt_vector3 & w) { return rt_vector3{ v.x*w.x, v.y*w.y, v.z*w.z }; }
static inline rt_vector3 operator/(const rt_vector3 & v, const rt_vector3 & w) { return rt_vector3{ v.x/w.x, v.y/w.y, v.z/w.z }; }
static inline rt_vector3 operator*(const rt_vector3 & v, const float f) { return rt_vector3{ v.x/f, v.y/f, v.z/f }; }
static inline rt_vector3 operator/(const rt_vector3 & v, const float f) { return rt_vector3{ v.x/f, v.y/f, v.z/f }; }
static inline bool operator==(const rt_vector3 & v, const rt_vector3 & w) { return (v.x==w.x)&&(v.y==w.y)&&(v.z==w.z); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
static inline float operator^(const rt_vector3 & v, const rt_vector3 & w) { return (v.x*w.x)+(v.y*w.y)+(v.z*w.z); }
// cross product. this represents the magnitude of the two vectors multiplied together, 
// multiplied by the sine of the angle between them, multiplied by the vector which is normal to
// the two input vectors. when oriented with the two vectors pointing as upward as possible visually,
// left vector cross right vector will point directly into the camera
static inline rt_vector3 operator%(const rt_vector3 & v, const rt_vector3 & w) { return rt_vector3{ (v.y*w.z)-(v.z*w.y), (v.z*w.x)-(v.x*w.z), (v.x*w.y)-(v.y*w.x) }; }

// square magnitude of a vector
static inline float mag_sq(rt_vector3 & v) { return (v.x*v.x)+(v.y*v.y)+(v.z*v.z); }
// magnitude of a vector
static inline float mag(rt_vector3 & v) { return sqrt(mag_sq(v)); }
// normalise a vector, i.e. divide by its magnitude to make its length equal to 1
static inline void norm_self(rt_vector3 & v) { v /= mag(v); }
rt_vector3 norm_copy(rt_vector3 & v) { return v / mag(v); }

// a colour is an equivalent type
typedef rt_vector3 rt_colour;

// perform gamma correction on a colour, for view transform, and place the output
// in a block of chars
static inline void gamma_correct(const rt_vector3 & v, float exp, float gam, uint8_t * o)
{
    rt_vector3 g = v * exp;
    g = rt_vector3{ powf(clamp0(g.x),gam), powf(clamp0(g.y),gam), powf(clamp0(g.z),gam) };
    o[0] = (uint8_t)(g.x*UINT8_MAX);
    o[1] = (uint8_t)(g.y*UINT8_MAX);
    o[2] = (uint8_t)(g.z*UINT8_MAX);
}

// represents a 2 dimensional vector, with associated operations
struct rt_vector2
{
    float u,v;

    inline void operator=(const rt_vector2 & a) { this->u = a.u; this->v = a.v; }
    inline void operator+=(const rt_vector2 & a) { this->u += a.u; this->v += a.v; }
    inline void operator-=(const rt_vector2 & a) { this->u -= a.u; this->v -= a.v; }
    inline void operator*=(const rt_vector2 & a) { this->u *= a.u; this->v *= a.v; }
    inline void operator/=(const rt_vector2 & a) { this->u /= a.u; this->v /= a.v; }
    inline void operator*=(const float f) { this->u *= f; this->v *= f; }
    inline void operator/=(const float f) { this->u /= f; this->v /= f; }
    inline rt_vector2 operator -() { return rt_vector2{ -this->u, -this->v }; }
};

static inline rt_vector2 operator+(const rt_vector2 & a, const rt_vector2 & b) { return rt_vector2{ a.u+b.u, a.v+b.v }; }
static inline rt_vector2 operator-(const rt_vector2 & a, const rt_vector2 & b) { return rt_vector2{ a.u-b.u, a.v-b.v }; }
static inline rt_vector2 operator*(const rt_vector2 & a, const rt_vector2 & b) { return rt_vector2{ a.u*b.u, a.v*b.v }; }
static inline rt_vector2 operator/(const rt_vector2 & a, const rt_vector2 & b) { return rt_vector2{ a.u/b.u, a.v/b.v }; }
static inline rt_vector2 operator*(const rt_vector2 & a, const float f) { return rt_vector2{ a.u/f, a.v/f}; }
static inline rt_vector2 operator/(const rt_vector2 & a, const float f) { return rt_vector2{ a.u/f, a.v/f}; }
static inline bool operator==(const rt_vector2 & a, const rt_vector2 & b) { return (a.u==b.u)&&(a.v==b.v); }

// linear interpolate between two vectors
static inline rt_vector3 lerp(const rt_vector3 & v, const rt_vector3 & w, const float f) { return rt_vector3{ v.x+((w.x-v.x)*f), v.y+((w.y-v.y)*f), v.z+((w.z-v.z)*f) }; }
// reflect v in w, where w can be interpreted as a vector normal to a mirror in which v is reflected
static inline rt_vector3 reflect(const rt_vector3 & v, const rt_vector3 & w) { rt_vector3 u = v-(w*((v.x*w.x)+(v.y*w.y)+(v.z*w.z))*2); return u/mag(u); }
// perturb v parallel to w by d.u, and perpendicular to w and v by d.v
// v and w should be normalised if the scaling of the offset d is to be correct
static inline rt_vector3 perturb(const rt_vector3 & v, const rt_vector3 & w, const rt_vector2 & d) { return v+(w*d.u)+((v%w)*d.v); }

#endif