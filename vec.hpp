#ifndef VEC_H
#define VEC_H

#include <iostream>

struct vec3 {
    union // allows us to refer to members of vec3 in different ways
    {
        float e[3];
        struct {
            float r, g, b;
        };
        struct {
            float x, y, z;
        };
    };

    vec3() 
    {
        x = 0;
        y = 0;
        z = 0;
    }

    vec3(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    vec3(float val)
    {
        x = val;
        y = val;
        z = val;
    }

    vec3 operator +() const { return *this; }
    vec3 operator -() const { return vec3(-x, -y, -z); }
    vec3& operator +=(const vec3 &other);
    vec3& operator *=(const vec3 &other);
    vec3& operator /=(const vec3 &other);
    vec3& operator -=(const vec3 &other);
    vec3& operator *=(float other);
    vec3& operator /=(float other);
};

struct vec2 {
    union
    {
        float e[2];
        struct {
            float x, y;
        };
    };

    vec2() {
        x = 0;
        y = 0;
    }

    vec2(float x_, float y_) {
        x = x_;
        y = y_;
    }

    vec2(float val) {
        x = val;
        y = val;
    }

    vec2 operator +() const { return *this; }
    vec2 operator -() const { return vec2(-x, -y); }
    vec2& operator +=(const vec2 &other);
    vec2& operator -=(const vec2 &other);
    vec2& operator *=(const vec2 &other);
    vec2& operator /= (const vec2 &other);
    vec2& operator /=(float other);
    vec2& operator *=(float other);
};

vec3 operator *(vec3 v, float k);
vec3 operator *(float k, vec3 v);
vec3 operator *(vec3 v1, vec3 v2);
vec3 operator /(vec3 v, float k);
vec3 operator /(vec3 v1, vec3 v2);
vec3 operator +(vec3 v1, vec3 v2);

vec2 operator *(vec2 v, float k);
vec2 operator *(float k, vec2 v);
vec2 operator *(vec2 v1, vec2 v2);
vec2 operator /(vec2 v, float k);
vec2 operator /(vec2 v1, vec2 v2);
vec2 operator +(vec2 v1, vec2 v2);

std::ostream& operator<<(std::ostream &out, const vec3 &toString);
std::ostream& operator<<(std::ostream &out, const vec2 &toString);

float clamp(float x, float minVal, float maxVal);
vec2 clamp(vec2 v, vec2 minVal, vec2 maxVal);
vec3 clamp(vec3 v, vec3 minVal, vec3 maxVal);

float mix(float x, float y, float a);
vec2 mix(vec2 v1, vec2 v2, vec2 a);
vec3 mix(vec3 v1, vec3 v2, vec3 a);

typedef vec3 rgb;

#endif