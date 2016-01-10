#include <iostream>
#include "vec.hpp"
#include "onb.hpp"

void onb::setBases(const vec3 &v1, const vec3 &v2, const vec3 &v3)
{
    u = v1;
    v = v2;
    w = v3;
}

void onb::initFromU(const vec3 &u_)
{
    vec3 n(1, 0, 0);
    vec3 m(0, 1, 0);

    u = makeUnitVector(u_);
    v = cross(u, n);

    if (lengthSquared(v) < ONB_EPSILON) {
        v = cross(u, m);
    }

    w = cross(u, v);
}

void onb::initFromV(const vec3 &v_)
{
    vec3 n(1, 0, 0);
    vec3 m(0, 1, 0);

    v = makeUnitVector(v_);
    u = cross(v, n);

    if (lengthSquared(u) < ONB_EPSILON) {
        u = cross(v, m);
    }

    w = cross(u, v);
}

void onb::initFromW(const vec3 &w_)
{
    vec3 n(1, 0, 0);
    vec3 m(0, 1, 0);

    w = makeUnitVector(w_);
    u = cross(v, n);

    if (lengthSquared(u) < ONB_EPSILON) {
        u = cross(v, m);
    }

    v = cross(w, u);
}

void onb::initFromUV(const vec3 &u_, const vec3 &v_)
{
    u = makeUnitVector(u_);
    w = makeUnitVector(cross(u_, v_));
    v = cross(w, u);
}

void onb::initFromUW(const vec3 &u_, const vec3 &w_)
{
    u = makeUnitVector(u_);
    v = makeUnitVector(cross(u_, w_));
    w = cross(u, v);
}

void onb::initFromWU(const vec3 &w_, const vec3 &u_)
{
    w = makeUnitVector(w_);
    v = makeUnitVector(cross(w_, u_));
    u = cross(v, w);
}

void onb::initFromVW(const vec3 &v_, const vec3 &w_)
{
    v = makeUnitVector(v_);
    u = makeUnitVector(cross(v_, w_));
    w = cross(u, v);
}

void onb::initFromWV(const vec3 &w_, const vec3 &v_)
{
    w = makeUnitVector(w_);
    u = makeUnitVector(cross(v_, w_));
    v = cross(w, u);
}

std::istream& operator >>(std::istream &in, onb &fromString)
{
    vec3 in_u, in_v, in_w;
    in >> in_u >> in_v >> in_w;
    fromString.initFromUV(in_u, in_v);

    return in;
}

std::ostream& operator <<(std::ostream &out, onb &toString)
{
    out << toString.u << "\n" << toString.v << "\n" << toString.w;
    return out; 
}

bool operator ==(const onb &o1, const onb &o2)
{
    return (o1.u == o2.u) && (o1.v == o2.v) && (o1.w == o2.w);
}
