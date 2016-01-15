/*
* main.cpp
* Contains the actual raytracing algorithm.
*/

#include <iostream>
#include <ctime>
#include <vector>
#include "vec.hpp"
#include "onb.hpp"
#include "dynarray.hpp"
#include "rng.hpp"
#include "shape.hpp"
#include "img.hpp"
#include "sample.hpp"
#include "camera.hpp"
#include "light.hpp"

#define WIDTH 500
#define HEIGHT 500
#define SMALL_VAL 0.000001f
#define NUM_SAMPLES 10
#define BACKGROUND rgb(0.0, 0.0, 0.0)
#define FAR 1000000.0

rgb lighting(hitRecord rec, std::vector<shape*> shapes, std::vector<light*> lights)
{
    hitRecord lightRecord;

    // Fire a ray from the point stored in rec to every light
    // If the object sees a light, then we illuminate it.
    for (auto l : lights) {
    }

    return rec.colour;
}

/* Trace a ray to a point and return the colour of that point */
rgb trace(ray r, std::vector<shape*> shapes, std::vector<light*> lights)
{   
    hitRecord rec;           // Contains information concerning what we hit
    float tmax = FAR;        // The maximum t value of r (maximum distance, really)
    bool hitShape = false;   // Tells us whether or not we've hit an object

    for (auto s : shapes) {
        if (s->hit(r, SMALL_VAL, tmax, 0, rec)) {
            tmax = rec.t;
            hitShape = true;
        }
    }

    // We've hit an object. rec thus contains how far away it is, and a normal
    if (hitShape == true) {
        return lighting(rec, shapes, lights);
    }

    // We didn't hit anything, so we should return the background colour
    else {
        return BACKGROUND;
    }
}

int main(void)
{
    img image(WIDTH, HEIGHT);    // PPM image
    bool isHit;                  // Records whether or not the given ray hit anything
    float tmax;                  // Farthest distance from camera we'll bother looking at
    vec3 dir(0,0,-1);            // Direction of viewing rays
    hitRecord rec;               // Contains information concerning what we hit
    std::vector<shape*> shapes;  // List of shapes in the scene...TODO: make this a faster data structure
    std::vector<light*> lights;  // List of lights in the scene
    rgb background(0.0,0.0,0.0); // Background colour

    // Popultate shapes to build the scene
    shapes.push_back(new sphere (vec3(250,250,-1000), 150.0f, rgb(0.7,0.1,0.7)));
    //shapes.push_back(new plane (vec3(0,0,0), vec3(0,0,1), vec3(1,1,1)));

    // Iterate over every pixel, firing off rays at objects
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {

            rgb avgColour = background;

            // Generate randomly sampled rays
            for (int k = 0; k < NUM_SAMPLES; k++) {
                ray r(vec3(drand48() + i - 0.5f, drand48() + j - 0.5f, 0), dir);
                avgColour += trace(r, shapes);
            }

            image.set(i, j, avgColour/NUM_SAMPLES);
        }        
    }   

    // Output the image to a ppm file
    image.writePPM(std::cout);
}