#include "vec.hpp"
#include <iostream>
#include <cmath>

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

bool operator ==(const vec3 &v1, const vec3 &v2)
{
    if (v1.x == v2.x &&
        v1.y == v2.y &&
        v1.z == v2.z) {
        return true;
    }

    else {
        return false;
    }
}

bool operator !=(const vec3 &v1, const vec3 &v2)
{
    return !(v1 == v2);
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

bool operator ==(const vec2 &v1, const vec2 &v2)
{
    if (v1.x == v2.x &&
        v1.y == v2.y) {
        return true;
    }

    else {
        return false;
    }
}

bool operator !=(const vec2 &v1, const vec2 &v2)
{
    return !(v1 == v2);
}

/* ------- IO functions for vectors ------- */

std::ostream& operator <<(std::ostream &out, const vec3 &toString)
{
    out << toString.x << " " << toString.y << " " << toString.z;
    return out;
}

std::istream& operator>>(std::istream &in, vec3 &fromString)
{
    float tmp;

    in >> tmp;
    fromString.x = tmp;
    in >> tmp;
    fromString.y = tmp;
    in >> tmp;
    fromString.z = tmp;

    return in;
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

/* ---- mathematical vector operations ----- */

/* Return the cross product of v1 and v2 */
vec3 cross(const vec3 &v1, const vec3 &v2)
{
    vec3 tmp;

    tmp.x = v1.y * v2.z - v1.z * v2.y;
    tmp.y = v1.z * v2.x - v1.x * v2.z;
    tmp.z = v1.x * v2.y - v1.y * v2.x;

    return tmp;
}

/* Return the length of a 3D vector */
float length(const vec3 &v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/* Return a vector of length 1 in the direction of v */
vec3 makeUnitVector(vec3 v)
{
    return v/length(v);
}

float lengthSquared(vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float dot(const vec3 &v1, const vec3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float tripleProduct(const vec3 &v1, const vec3 &v2, const vec3 &v3)
{
    return dot(cross(v1, v2), v3);
}

/* ----- other functions ----- */
vec3 minVec(const vec3 &v1, const vec3 &v2)
{
    vec3 tmp = v1;

    if (v2.x < v1.x) {
        tmp.x = v2.x;
    }

    if (v2.y < v1.y) {
        tmp.y = v2.y;
    }

    if (v2.z < v1.z) {
        tmp.z = v2.z;
    }

    return tmp;
}

vec3 maxVec(const vec3 &v1, const vec3 &v2)
{
    vec3 tmp = v1;

    if (v2.x > v1.x) {
        tmp.x = v2.x;
    }

    if (v2.y > v1.y) {
        tmp.y = v2.y;
    }

    if (v2.z > v1.z) {
        tmp.z = v2.z;
    }

    return tmp;
}

float maxComponent(const vec3 &v)
{
    if (v.x > v.y && v.x > v.z) {
        return v.x;
    }

    else if (v.y > v.x && v.y > v.z) {
        return v.y;
    }

    else {
        return v.z;
    }
}

float minComponent(const vec3 &v)
{
    if (v.x < v.y && v.x < v.z) {
        return v.x;
    }

    else if (v.y < v.x && v.y < v.z) {
        return v.y;
    }

    else {
        return v.z;
    }
}

float maxComponentAbs(const vec3 &v)
{
    float tmp = fabs(v.x);

    if (fabs(v.y) > tmp) {
        tmp = fabs(v.y);
    }

    if (fabs(v.z) > tmp) {
        tmp = fabs(v.z);
    }

    return tmp;
}

float minComponentAbs(const vec3 &v)
{
    float tmp = fabs(v.x);

    if (fabs(v.y) < tmp) {
        tmp = fabs(v.y);
    }

    if (fabs(v.z) < tmp) {
        tmp = fabs(v.z);
    }

    return tmp;
}

int indexOfMinComponent(const vec3 &v)
{
    if (v.x < v.y && v.x < v.z) {
        return 0;
    }

    else if (v.y < v.x && v.y < v.z) {
        return 1;
    }

    else {
        return 2;
    }
}

int indexOfMaxComponent(const vec3 &v)
{
    if (v.x > v.y && v.x > v.z) {
        return 0;
    }

    else if (v.y > v.x && v.y > v.z) {
        return 1;
    }

    else {
        return 2;
    }
}

int indexOfMinComponentAbs(const vec3 &v)
{

    int idx = 0;
    float tmp = fabs(v.x);

    if (fabs(v.y) < tmp) {
        tmp = fabs(v.y);
        idx = 1;
    }

    if (fabs(v.z) < tmp) {
        idx = 2;
    }

    return idx;
}

int indexOfMaxComponentAbs(const vec3 &v)
{
    int idx = 0;
    float tmp = fabs(v.x);

    if (fabs(v.y) > tmp) {
        tmp = fabs(v.y);
        idx = 1;
    }

    if (fabs(v.z) > tmp) {
        idx = 2;
    }

    return idx;
}
