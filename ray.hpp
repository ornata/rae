/*
* ray.hpp
* Contains the struct and functions for the ray type.
* That is, a halfline from [0,infinity].
* Equation of a ray: r = o + td, where t is a parameter
* basically saying where to "stop".
*/

#ifndef RAY_H
#define RAY_H

#include "vec.hpp"
#include <iostream>

struct ray
{
    vec3 origin;
    vec3 direction;

    ray()
    {
        origin = vec3(0,0,0);
        direction = vec3(0,0,0);
    }

    ray(const vec3 &v1, const vec3 &v2)
    {
        origin = v1;
        direction = v2;
    }
};

inline std::ostream &operator <<(std::ostream &out, const ray &toString)
{
    out << "(" << toString.origin << ") + t(" << toString.direction << ")";
    return out;
}

#endif