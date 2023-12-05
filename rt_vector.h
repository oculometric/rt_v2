#ifndef RT_VECTOR_H
#define RT_VECTOR_H

#include "rt_maths.h"

#include <iostream>

// represents a 3 dimensional vector, with associated operations
struct rt_vector3
{
    float x, y, z;

    // inline void operator=(const rt_vector3 & v) { this->x = v.x; this->y = v.y; this->z = v.z; }
    // inline void operator+=(const rt_vector3 & v) { this->x += v.x; this->y += v.y; this->z += v.z; }
    // inline void operator-=(const rt_vector3 & v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }
    // inline void operator*=(const rt_vector3 & v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }
    // inline void operator/=(const rt_vector3 & v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }
    // inline void operator*=(const float f) { this->x *= f; this->y *= f; this->z *= f; }
    // inline void operator/=(const float f) { this->x /= f; this->y /= f; this->z /= f; }
    // inline rt_vector3 operator -() { return rt_vector3{ -this->x, -this->y, -this->z }; }
};

static inline std::ostream& operator<<(std::ostream & stream, const rt_vector3 & v)
{
    stream << "(" << v.x << "," << v.y << "," << v.z << ")";
    return stream;
}

static inline void add(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { o.x=v.x+w.x; o.y=v.y+w.y; o.z=v.z+w.z; }
static inline void sub(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { o.x=v.x-w.x; o.y=v.y-w.y; o.z=v.z-w.z; }
static inline void mul(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { o.x=v.x*w.x; o.y=v.y*w.y; o.z=v.z*w.z; }
static inline void div(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { o.x=v.x/w.x; o.y=v.y/w.y; o.z=v.z/w.z; }
static inline void mul(const rt_vector3 & v, const float f, rt_vector3 & o) { o.x=v.x*f; o.y=v.y*f; o.z=v.z*f; }
static inline void div(const rt_vector3 & v, const float f, rt_vector3 & o) { o.x=v.x/f; o.y=v.y/f; o.z=v.z/f; }
static inline bool operator==(const rt_vector3 & v, const rt_vector3 & w) { return (v.x==w.x)&&(v.y==w.y)&&(v.z==w.z); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
static inline void dot(const rt_vector3 & v, const rt_vector3 & w, float & o) { o = (v.x*w.x)+(v.y*w.y)+(v.z*w.z); }
// cross product. this represents the magnitude of the two vectors multiplied together, 
// multiplied by the sine of the angle between them, multiplied by the vector which is normal to
// the two input vectors. when oriented with the two vectors pointing as upward as possible visually,
// left vector cross right vector will point directly into the camera
static inline void cross(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { o.x=(v.y*w.z)-(v.z*w.y); o.y=(v.z*w.x)-(v.x*w.z); o.z=(v.x*w.y)-(v.y*w.x); }

// square magnitude of a vector
static inline void mag_sq(const rt_vector3 & v, float & o) { o = (v.x*v.x)+(v.y*v.y)+(v.z*v.z); }
// magnitude of a vector
static inline void mag(const rt_vector3 & v, float & o) { mag_sq(v,o); o = sqrt(o); }
// normalise a vector, i.e. divide by its magnitude to make its length equal to 1
static inline void norm_self(rt_vector3 & v) { float m; mag(v, m); if (m > 0) div(v, m, v); }
static inline void norm_copy(const rt_vector3 & v, rt_vector3 & o) { float m; mag(v, m); o=v; if (m > 0) div(v, m, o); }

// a colour is an equivalent type
typedef rt_vector3 rt_colour;

// perform gamma correction on a colour, for view transform, and place the output
// in a block of chars
static inline void gamma_correct(const rt_vector3 & v, float exp, float gam, uint8_t * o)
{
    rt_vector3 g;
    mul(v, exp, g);
    g.x = powf(clamp0(g.x),gam);
    g.y = powf(clamp0(g.y),gam);
    g.z = powf(clamp0(g.z),gam);
    o[0] = (uint8_t)(g.x*UINT8_MAX);
    o[1] = (uint8_t)(g.y*UINT8_MAX);
    o[2] = (uint8_t)(g.z*UINT8_MAX);
}

// represents a 2 dimensional vector, with associated operations
struct rt_vector2
{
    float u,v;

    // inline void operator=(const rt_vector2 & a) { this->u = a.u; this->v = a.v; }
    // inline void operator+=(const rt_vector2 & a) { this->u += a.u; this->v += a.v; }
    // inline void operator-=(const rt_vector2 & a) { this->u -= a.u; this->v -= a.v; }
    // inline void operator*=(const rt_vector2 & a) { this->u *= a.u; this->v *= a.v; }
    // inline void operator/=(const rt_vector2 & a) { this->u /= a.u; this->v /= a.v; }
    // inline void operator*=(const float f) { this->u *= f; this->v *= f; }
    // inline void operator/=(const float f) { this->u /= f; this->v /= f; }
    // inline rt_vector2 operator -() { return rt_vector2{ -this->u, -this->v }; }
};

static inline void add(const rt_vector2 & a, const rt_vector2 & b, rt_vector2 & o) { o.u=a.u+b.u; o.v=a.v+b.v; }
static inline void sub(const rt_vector2 & a, const rt_vector2 & b, rt_vector2 & o) { o.u=a.u-b.u; o.v=a.v-b.v; }
static inline void mul(const rt_vector2 & a, const rt_vector2 & b, rt_vector2 & o) { o.u=a.u*b.u; o.v=a.v*b.v; }
static inline void div(const rt_vector2 & a, const rt_vector2 & b, rt_vector2 & o) { o.u=a.u/b.u; o.v=a.v/b.v; }
static inline void mul(const rt_vector2 & a, const float f, rt_vector2 & o) { o.u=a.u*f; o.v=a.v*f; }
static inline void div(const rt_vector2 & a, const float f, rt_vector2 & o) { o.u=a.u/f; o.v=a.v/f; }
static inline bool operator==(const rt_vector2 & a, const rt_vector2 & b) { return (a.u==b.u)&&(a.v==b.v); }

// linear interpolate between two vectors
static inline void lerp(const rt_vector3 & v, const rt_vector3 & w, const float f, rt_vector3 & o) { o.x=v.x+((w.x-v.x)*f); o.y=v.y+((w.y-v.y)*f); o.z=v.z+((w.z-v.z)*f); }
// reflect v in w, where w can be interpreted as a vector normal to a mirror in which v is reflected
// w must be normalised
static inline void reflect(const rt_vector3 & v, const rt_vector3 & w, rt_vector3 & o) { float f; dot(v,w,f); f = f * 2.0; mul(w, f, o); sub(v, o, o); }
// perturb v parallel to w by d.u, and perpendicular to w and v by d.v
// v and w should be normalised if the scaling of the offset d is to be correct
static inline void perturb(const rt_vector3 & v, const rt_vector3 & w, const rt_vector2 & d, rt_vector3 & o) { cross(v, w, o); o.x=v.x+(w.x*d.u)+(o.x*d.v); o.y=v.y+(w.y*d.u)+(o.y*d.v); o.z=v.z+(w.z*d.u)+(o.z*d.v); }

#endif