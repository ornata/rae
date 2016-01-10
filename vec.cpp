#include "vec.hpp"
#include <iostream>

/* ------- Basic operations on vec3 types ------- */

vec3& vec3::operator +=(const vec3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

vec3& vec3::operator *=(const vec3 &other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

vec3& vec3::operator /=(const vec3 &other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;

    return *this;
}

vec3& vec3::operator -=(const vec3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

vec3& vec3::operator *=(float other)
{
    x *= other;
    y *= other;
    z *= other;

    return *this;
}

vec3& vec3::operator /=(float other)
{
    x /= other;
    y /= other;
    z /= other;

    return *this;
}

vec3 operator *(vec3 v, float k)
{
    return v *= k;
}

vec3 operator *(float k, vec3 v)
{
    return v *= k;
}

vec3 operator *(vec3 v1, vec3 v2)
{
    return v1 *= v2;
}

vec3 operator /(vec3 v, float k)
{
    return v /= k;
}

vec3 operator /(vec3 v1, vec3 v2)
{
    return v1 /= v2;
}

vec3 operator +(vec3 v1, vec3 v2)
{
    return v1 += v2;
}

/* ------- Basic operations on vec2 types ------- */
vec2& vec2::operator +=(const vec2 &other)
{
    x += other.x;
    y += other.y;

    return *this;
}

vec2& vec2::operator *=(const vec2 &other)
{
    x *= other.x;
    y *= other.y;

    return *this;
}

vec2& vec2::operator /=(const vec2 &other)
{
    x /= other.x;
    y /= other.y;

    return *this;
}

vec2& vec2::operator -=(const vec2 &other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

vec2& vec2::operator *=(float other)
{
    x *= other;
    y *= other;

    return *this;
}

vec2& vec2::operator /=(float other)
{
    x /= other;
    y /= other;

    return *this;
}

vec2 operator *(vec2 v, float k)
{
    return v *= k;
}

vec2 operator *(float k, vec2 v)
{
    return v *= k;
}

vec2 operator *(vec2 v1, vec2 v2)
{
    return v1 *= v2;
}

vec2 operator /(vec2 v, float k)
{
    return v /= k;
}

vec2 operator /(vec2 v1, vec2 v2)
{
    return v1 /= v2;
}

vec2 operator +(vec2 v1, vec2 v2)
{
    return v1 += v2;
}


/* ------- Output functions for vectors ------- */

std::ostream& operator <<(std::ostream &out, const vec3 &toString)
{
    out << toString.x << " " << toString.y << " " << toString.z;
    return out;
}

std::ostream& operator <<(std::ostream &out, const vec2 &toString)
{
    out << toString.x << " " << toString.y;
    return out;
}

/* ------- clamp functions ------- */

/* Make sure x is in [minVal, maxVal] */
float clamp(float x, float minVal, float maxVal)
{
    if (x < minVal) {
        x = minVal;
    }

    if (x > maxVal) {
        x = maxVal;
    }

    return x;
}

/* 
* Make sure each component of v is in the interval [m, M]
* for each component m of minVal and M of maxVal.
*/
vec2 clamp(vec2 v, vec2 minVal, vec2 maxVal)
{
    if (v.x < minVal.x) {
        v.x = minVal.x;
    }

    if (v.y < minVal.y) {
        v.y = minVal.y;
    }

    if (v.x > maxVal.x) {
        v.x = maxVal.x;
    }

    if (v.y > maxVal.y) {
        v.y = maxVal.y;
    }

    return v;
}

/* 
* Make sure each component of v is in the interval [m, M]
* for each component m of minVal and M of maxVal.
*/
vec3 clamp(vec3 v, vec3 minVal, vec3 maxVal)
{
    if (v.x < minVal.x) {
        v.x = minVal.x;
    }

    if (v.y < minVal.y) {
        v.y = minVal.y;
    }

    if (v.z < minVal.z) {
        v.z = minVal.z;
    }

    if (v.x > maxVal.x) {
        v.x = maxVal.x;
    }

    if (v.y > maxVal.y) {
        v.y = maxVal.y;
    }

    if (v.z > maxVal.z) {
        v.z = maxVal.z;
    }

    return v;
}

/* ------- mix functions ------- */

/*
* Return the linear blend of x, y, and a
*/
float mix(float x, float y, float a)
{
    return x * (1-a) + (y * a);
}

vec2 mix(vec2 v1, vec2 v2, vec2 a)
{
    return vec2(v1.x * (1-a.x) + (v2.x * a.x),
                v1.y * (1-a.y) + (v2.y * a.y));
}

vec3 mix(vec3 v1, vec3 v2, vec3 a)
{
    return vec3(v1.x * (1 - a.x) + (v2.x * a.x),
                v1.y * (1 - a.y) + (v2.y * a.y),
                v1.z * (1 - a.z) + (v2.z * a.z));
}
