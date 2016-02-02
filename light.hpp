#ifndef LIGHT_H
#define LIGHT_H

#include "vec.hpp"
#include "matrix.hpp"

struct pointLight {
    vec3 location;  // location of light
    float strength; // strength of light
    rgb colour;     // colour of the light
    tmat transformation = identityMatrix();

    pointLight() : location(vec3(0,0,0)), strength(1.0f), colour(rgb(1,1,1)) {}
    pointLight(vec3 location_) : location(location_), strength(1.0f), colour(rgb(1,1,1)) {}
    pointLight(vec3 location_, float strength_) : location(location_), strength(strength_), colour(rgb(1,1,1)) {}
    pointLight(vec3 location_, float strength_, rgb colour_) : location(location_), strength(strength_), colour(colour_) {}
};

std::ostream& operator <<(std::ostream &out, pointLight &toString)
{
    out << "pointLight: " << "(" << toString.location << ") .. Strength: " << toString.strength;
    return out; 
}

#endif