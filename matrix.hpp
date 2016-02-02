/* Implements transformation matrices, to allow us to perform transformations on points in 3D space */
#ifndef MATRIX_H
#define MATRIX_H

#include "vec.hpp"
#include "ray.hpp"

/* Transforms vectors into other vectors */
struct tmat {
    float m[4][4];

    tmat operator +=(const tmat &other);
    tmat operator -=(const tmat &other);
    tmat operator *=(const tmat &other);
    tmat operator *=(float s);

    float det() const;
    void setToTranspose();
    tmat transpose() const;
};

tmat operator +(tmat l, const tmat &r);
tmat operator -(tmat l, const tmat &r);
tmat operator *(tmat l, const tmat &r);
vec3 operator *(const tmat &l, vec3 r);
ray operator *(const tmat &l, ray r);
tmat operator *(const tmat &l, float r);

vec3 transformPt(const tmat &mat, vec3 v);
vec3 transformVec(const tmat &mat, vec3 v);
tmat identityMatrix();
tmat zeroMatrix();
tmat transpose(const tmat &mat);
tmat inverse(const tmat &mat);
tmat translate(float x, float y, float z);
tmat scale(float x, float y, float z);
tmat rotateX(float angle);
tmat rotateY(float angle);
tmat rotateZ(float angle);
tmat rotate(const vec3 &axis, float angle);
tmat viewMat(const vec3 &eye, const vec3 &gaze, const vec3 &up);

std::ostream &operator <<(std::ostream &out, const tmat &mat);

// helper for 4*4 det
inline float det3(float a, float b, float c,
           float d, float e, float f,
           float g, float h, float i) {

    return(a*e*i + d*h*c + g*b*f - g*e*c - d*b*i - a*h*f);

}

#endif