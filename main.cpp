/*
* main.cpp
* Contains the actual raytracing algorithm.
*/

#include <iostream>
#include <ctime>
#include <vector>
#include "vec.hpp"
#include "onb.hpp"
#include "shape.hpp"
#include "img.hpp"
#include "sample.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "matrix.hpp"

#define WIDTH 600
#define HEIGHT 600
#define SMALL_VAL 0.000001f
#define NUM_SAMPLES 15
#define BACKGROUND rgb(0.0, 0.0, 0.0)
#define WHITE rgb(1.0, 1.0, 1.0)
#define RED rgb(1.0, 0.0, 0.0)
#define FAR 1000000.0
#define AMBIENT rgb(0.3, 0.3, 0.3)
#define MAX_BOUNCE 10
#define PI 3.14159265359

/* Compute the lighting of the scene using the rendering equation. */
rgb lighting(hitRecord rec, const std::vector<shape*> &shapes, const std::vector<pointLight*> &pointLights)
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
        vec3 w_i = makeUnitVector(q - p);
        ray r(p, w_i);

        if (dot(rec.normal, w_i) < 0) {
            continue;
        }

        blocked = false;
        for (auto s : shapes) {
            // the shape is shadowed by another shape
            if (s->shadowHit(r, SMALL_VAL + 0.01, tmax, 0)) {
                blocked = true;
                break;
            }
        }

        if (blocked == false) {
            vec3 Li = l->colour * l->strength;
            float rho_d = 0.7f; //todo
            float fr = rho_d / PI;
            lightContribution += fr * Li * dot(w_i, rec.normal); // rendering equation
        }
    }

    lightContribution += AMBIENT;
    lightContribution *= rec.colour;

    return lightContribution;
}

/* Trace a ray to a point and return the colour of that point */
rgb trace(ray r, const std::vector<shape*> &shapes, const std::vector<pointLight*> &pointLights)
{   
    hitRecord rec;           // Contains information concerning what we hit
    shape* hitShape;
    float tmax;        // The maximum t value of r (maximum distance, really)
    int i;
    rgb colour;

    for (i = 0; i <= MAX_BOUNCE; i++) {
        tmax = FAR;
        hitShape = nullptr;

        for (auto s : shapes) {
            if (s->hit(r, SMALL_VAL, tmax, 0, rec)) {
                tmax = rec.t;
                hitShape = s;
            }
        }

        // At this point, tmax contains the distance from the eye to the closest shape.
        // Now we want to determine whether or not the point at distance tmax should be coloured.
        if (hitShape) {
            rec.pointOnSurface = r.origin + rec.t * r.direction;

            if (hitShape->mirror) {
                vec3 normal = makeUnitVector(rec.normal);
                r.direction = r.direction - 2.0f * dot(normal, r.direction) * normal; // reflect
                r.origin = rec.pointOnSurface + r.direction * 0.001f;
                continue; // bounce
            }

            colour = lighting(rec, shapes, pointLights) * (1.0f - (float)i / (MAX_BOUNCE + 1));
            break;
        }

        // We didn't hit anything, so we should return the background colour
        else {
            colour = BACKGROUND;
            break;
        }
    }

    return colour;
}

void initLights(std::vector<pointLight*> &pointLights)
{
    pointLights.push_back(new pointLight());
    pointLights.push_back( new pointLight( vec3(0, 200, -400), 4.0f, rgb(0.7, 0.7, 0.0f)));
    pointLights.push_back( new pointLight( vec3(0, 200, 100), 5.0f, rgb(0.5, 0.0, 0.9f)));
    pointLights.push_back( new pointLight( vec3(350, 300, 300), 5.0f, rgb(0.5, 0.7, 0.9f)));
}

/* Initialize the shapes to be rendered in the scene */
void initShapes(std::vector<shape*> &shapes, int id)
{
    shapes.push_back(new plane (vec3(0,60, 0), vec3(0,1,0), rgb(1,1,1)));

    if (id == 1) {
        shapes.push_back(new sphere (vec3(180,250,-200), 50.0f, rgb(0.1,0.7,0.7)));
        shapes.push_back(new sphere (vec3(400,250,-200), 100.0f, rgb(0.9,0.9,0.0)));
        shapes.push_back(new sphere (vec3(300,100,-200), 60.0f, rgb(0.9,0.1,0.7)));
        shapes.back()->mirror = true;
    }

    else if (id == 2) {
        for (int i = 0; i < 10; i++) {
            tmat moveSphere = translate(i*200, 300, -200);
            sphere* s = new sphere(vec3(0,0,0), 100.0f, rgb(0.1,0.7,0.7));
            shapes.push_back(new instance(moveSphere, s));
            if (i % 2 == 0) {
                shapes.back()->mirror = true;
            }
        }
    }

    else if (id == 3) {
        tmat move = translate(200, 200, -50);
        tmat scaleMesh = scale(100, 100, 100);
        triangleMesh* m = new triangleMesh("cube.mesh", RED);
        shapes.push_back(new instance(move * scaleMesh, m));
    }

    else if (id == 4) {
        tmat move = translate(200, 200, -50);
        tmat scaleSphere = scale(100, 100, 100);

        sphere* s = new sphere(vec3(0,0,0), 1.0f, rgb(0.1,0.7,0.7));
        shapes.push_back(new instance(move * scaleSphere, s));
    }
}

inline vec2 sample(const int &i, const int &j)
{  
    return vec2(drand48() + i - 0.5f, drand48() + j - 0.5f);
}

/* Return a ray to fire off at the scene */
inline ray getRayWithPerspective(const int &i, const int &j, const vec3 &origin)
{
    // get a ray in orthogonal projection
    vec2 p = sample(i, j);
    vec3 originToPixel(makeUnitVector(vec3(p.x, p.y, 0) - origin));
    return ray(origin, originToPixel);
}

inline ray getRayOrthogonal(const int &i, const int &j, const vec3 &dir)
{
    vec2 p = sample(i, j);
    return ray(vec3(p.x, p.y, 0), dir);
}

int main(void)
{
    img image(WIDTH, HEIGHT);    // PPM image
    bool isHit;                  // Records whether or not the given ray hit anything
    float tmax;                  // Farthest distance from camera we'll bother looking at

    vec3 eye(WIDTH/2, HEIGHT/2, 400); // centre of camera
    vec3 dir(0,0,-1);                 // Direction of viewing rays

    hitRecord rec;               // Contains information concerning what we hit
    std::vector<shape*> shapes;  // List of shapes in the scene...TODO: make this a faster data structure
    std::vector<pointLight*> pointLights;  // List of pointLights in the scene
    rgb background(0.0,0.0,0.0); // Background colour

    initLights(pointLights);
    initShapes(shapes, 3);

    // Iterate over every pixel, firing off rays at objects 
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            rgb avgColour = background;

            // Generate randomly sampled rays
            for (int k = 0; k < NUM_SAMPLES; k++) {
                //ray r = getRayOrthogonal(i, j, eye);
                ray r = getRayWithPerspective(i, j, eye);
                avgColour += trace(r, shapes, pointLights);
            }

            image.set(i, HEIGHT - j - 1, avgColour/NUM_SAMPLES);
        }        
    }   

    // Output the image to a ppm file
    image.writePPM("out.ppm");
    
}