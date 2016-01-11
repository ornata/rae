#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include "ray.hpp"
#include "vec.hpp"

struct hitRecord
{
    float t;
    vec3 normal;
    rgb colour;
};

struct shape
{
    virtual bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const=0;
    virtual bool shadowHit(const ray &r, float tmin, float tmax, float time) const=0;
};

struct triangle : public shape
{
    vec3 p0, p1, p2;
    rgb colour;

    triangle(const vec3 &p0_, const vec3 &p1_, const vec3& p2_, const rgb &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

struct sphere : shape
{
    vec3 centre;
    float radius;
    rgb colour; 

    sphere(const vec3 &centre_, float radius_, const rgb &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

std::ostream &operator <<(std::ostream &out, const triangle &toString);
std::ostream &operator <<(std::ostream &out, const sphere &toString);

#endif