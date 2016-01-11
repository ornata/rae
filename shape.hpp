/*
* shape.hpp
* Contains structs for shapes that can be ray traced, and the hitRecord struct.
* That includes things like triangles, spheres, etc.
*/

#ifndef SHAPE_H
#define SHAPE_H

#include <cmath>
#include <iostream>
#include "ray.hpp"
#include "vec.hpp"

/* hitRecord: stores information to do with ray-object intersections */
struct hitRecord
{
    float t;
    vec3 normal;
    rgb colour;
};

/* shape: all shapes will be iterated over and checked for hit and shadowhit */
struct shape
{
    virtual bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const=0;
    virtual bool shadowHit(const ray &r, float tmin, float tmax, float time) const=0;
};

/* triangle: defined by three points */
struct triangle : public shape
{
    vec3 p0, p1, p2;
    rgb colour;

    triangle(const vec3 &p0_, const vec3 &p1_, const vec3& p2_, const rgb &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

/* sphere: defined by a centre and a radius */
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