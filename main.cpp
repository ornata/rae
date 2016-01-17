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
#define WHITE rgb(1.0, 1.0, 1.0)
#define FAR 1000000.0
#define AMBIENT rgb(0.3, 0.3, 0.3)
#define MAX_BOUNCE 2

void drawLight(pointLight l, img image)
{
    image.set(l.location.x, l.location.y, WHITE);
    image.set(l.location.x+1, l.location.y, WHITE);
    image.set(l.location.x+2, l.location.y, WHITE);
    image.set(l.location.x-1, l.location.y, WHITE);
    image.set(l.location.x-2, l.location.y, WHITE);

    image.set(l.location.x, l.location.y+1, WHITE);
    image.set(l.location.x, l.location.y+2, WHITE);
    image.set(l.location.x+2, l.location.y-1, WHITE);
    image.set(l.location.x-1, l.location.y-2, WHITE);
}

rgb lighting(hitRecord rec, std::vector<shape*> shapes, std::vector<pointLight*> pointLights)
{
    float tmax;
    vec3 p = rec.pointOnSurface;
    vec3 q;
    rgb lightContribution(0.0, 0.0, 0.0);
    int numLights = pointLights.size();
    bool blocked;

    // Now check if there is any shape in the way of the path from p to q, for all q = a pointLight position
    for (auto l : pointLights) {
        q = l->location;
        tmax = length(p - q);
        vec3 ptoqDir = makeUnitVector(q - p);
        ray r(p, ptoqDir);

        if (dot(rec.normal, ptoqDir) < 0) {
            continue;
        }

        blocked = false;
        for (auto s : shapes) {
            // the shape is shadowed by another shape
            if (s->shadowHit(r, SMALL_VAL+0.01, tmax, 0)) {
                blocked = true;
                break;
            }
        }

        if (blocked == false) {
            lightContribution += l->colour * l->strength;
        }
    }

    lightContribution += AMBIENT;

    lightContribution *= rec.colour;

    return lightContribution;
}

/* Trace a ray to a point and return the colour of that point */
rgb trace(ray r, std::vector<shape*> shapes, std::vector<pointLight*> pointLights)
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

    // At this point, tmax contains the distance from the eye to the closest shape.
    // Now we want to determine whether or not the point at distance tmax should be coloured.
    if (hitShape == true) {
        rec.pointOnSurface = r.origin + rec.t * r.direction;
        return lighting(rec, shapes, pointLights);
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
    std::vector<pointLight*> pointLights;  // List of pointLights in the scene
    rgb background(0.0,0.0,0.0); // Background colour

    // Popultate shapes to build the scene
    shapes.push_back(new sphere (vec3(250,250,-200), 50.0f, rgb(0.7,0.1,0.7)));
    shapes.push_back(new sphere (vec3(400,250,-200), 100.0f, rgb(0.9,0.1,0.7)));
    pointLights.push_back(new pointLight(vec3(200,300,-200), 1.0f));
    shapes.push_back(new plane (vec3(0,0.5,-100), vec3(0,0.5,0.5), vec3(1,1,1)));

    // Iterate over every pixel, firing off rays at objects
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {

            rgb avgColour = background;

            // Generate randomly sampled rays
            for (int k = 0; k < NUM_SAMPLES; k++) {
                ray r(vec3(drand48() + i - 0.5f, drand48() + j - 0.5f, 0), dir);
                avgColour += trace(r, shapes, pointLights);
            }

            image.set(i, j, avgColour/NUM_SAMPLES);
        }        
    }   

    for (auto l : pointLights) {
        drawLight(*l, image);
    }

    // Output the image to a ppm file
    image.writePPM(std::cout);
}