#ifndef ONB_H
#define ONB_H

#include <iostream>
#include "vec.hpp"

#define ONB_EPSILON 0.001f

struct onb
{
    vec3 u, v, w;

    onb(const vec3 &v1, const vec3& v2, const vec3 &v3)
    {
        u = v1;
        v = v2;
        w = v3;
    }

    void setBases(const vec3 &v1, const vec3 &v2, const vec3 &v3);
    void initFromU(const vec3 &u_);
    void initFromV(const vec3 &v_);
    void initFromW(const vec3 &w_);
    void initFromUV(const vec3 &u_, const vec3 &v_);
    void initFromUW(const vec3 &u_, const vec3 &w_);
    void initFromWU(const vec3 &w_, const vec3 &u_);
    void initFromVW(const vec3 &v_, const vec3 &w_);
    void initFromWV(const vec3 &w_, const vec3 &v_);

};

std::istream &operator >>(std::istream &in, onb &fromString);
std::ostream &operator <<(std::ostream &out, onb &toString);

bool operator ==(const onb &o1, const onb &o2);

#endif
