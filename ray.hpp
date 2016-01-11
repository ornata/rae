#ifndef RAY_H
#define RAY_H

#include "vec.hpp"
#include <iostream>

struct ray
{
    vec3 origin;
    vec3 direction;

    ray(const vec3 &v1, const vec3 &v2)
    {
        origin = v1;
        direction = v2;
    }

    vec3 pointAtParameter(float t)
    {
        return origin + t * direction;
    }
};

inline std::ostream &operator <<(std::ostream &out, const ray &toString)
{
    out << "(" << r.origin << ") + t(" << r.direction << ")";
}

#endif