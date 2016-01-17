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
    float t;     // param t in r = o + td for a ray r
    vec3 normal; // normal to the surface defined by the intersection
    vec3 pointOnSurface;
    rgb colour;  // colour of the point
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
    vec3 p0; // left point
    vec3 p1; // top point
    vec3 p2; // right point
    rgb colour;

    triangle(const vec3 &p0_, const vec3 &p1_, const vec3& p2_, const rgb &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

/* sphere: defined by a centre and a radius */
struct sphere : shape
{
    vec3 centre; // centre of sphere
    float radius; // radius of sphere
    rgb colour;  // colour of sphere

    sphere(const vec3 &centre_, float radius_, const rgb &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

/* plane: defined by a point + a normal */
struct plane : shape
{
    vec3 pt; // point on the plane
    vec3 n;  // vector normal to the plane
    rgb colour; // colour of plane

    plane(const vec3 &pt_, const vec3 &n_, const vec3 &colour_);
    bool hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const;
    bool shadowHit(const ray &r, float tmin, float tmax, float time) const;
};

std::ostream &operator <<(std::ostream &out, const triangle &toString);
std::ostream &operator <<(std::ostream &out, const sphere &toString);
std::ostream &operator <<(std::ostream &out, const plane &toString);

#endif