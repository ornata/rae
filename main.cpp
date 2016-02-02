/*
* main.cpp
* Contains the actual raytracing algorithm.
*/

#include <iostream>
#include <ctime>
#include <vector>
#include "vec.hpp"
#include "shape.hpp"
#include "img.hpp"
#include "light.hpp"
#include "matrix.hpp"

const int WIDTH  = 500;
const int HEIGHT = 500;
const int MAX_BOUNCE = 7;
const float SMALL_VAL = 0.000001f;
const float FAR = 1000000.0f;
const int NUM_SAMPLES = 15;
const rgb background(0.0f, 0.0f, 0.0f);
const rgb white(1.0, 1.0, 1.0);
const rgb red(1.0, 0.0, 0.0);
const rgb ambient(0.3, 0.3, 0.3);
const float pi = 3.14159265359;
img image(WIDTH, HEIGHT);

/* Compute the lighting of the scene using the rendering equation. */
inline rgb lighting(hitRecord rec, const std::vector<shape*> &shapes, const std::vector<pointLight*> &pointLights)
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
            float fr = rho_d / pi;
            lightContribution += fr * Li * dot(w_i, rec.normal); // rendering equation
        }
    }

    lightContribution += ambient; // make sure everything in the scene isn't just black!
    lightContribution *= rec.colour;

    return lightContribution;
}

/* Trace a ray to a point and return the colour of that point */
inline rgb trace(ray r, const std::vector<shape*> &shapes, const std::vector<pointLight*> &pointLights)
{   
    hitRecord rec;    // Contains information concerning what we hit
    shape* hitShape;  // The shape we actually hit
    float tmax;
    int i;
    rgb colour;

    for (i = 0; i <= MAX_BOUNCE; i++) {
        tmax = FAR;
        hitShape = nullptr;

        // Find the closest shape hit by the ray, if one exists
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
            colour = background;
            break;
        }
    }

    return colour;
}

/* Initialize the lights to be put in the scene */
inline void initLights(std::vector<pointLight*> &pointLights)
{
    pointLights.push_back(new pointLight());
    pointLights.push_back( new pointLight( vec3(800, 400, -400), 2.0f, rgb(0.7, 0.7, 0.0f)));
    pointLights.push_back( new pointLight( vec3(0, 200, 100), 2.0f, rgb(0.5, 0.0, 0.9f)));
    pointLights.push_back( new pointLight( vec3(350, 300, 300), 2.0f, rgb(0.5, 0.7, 0.9f)));
    pointLights.push_back( new pointLight( vec3(600, 600, -600), 2.0f, rgb(0.7, 0.7, 0.7f)));
}

/* Load the fractal scene made of cubes */
void initFractalCubes(std::vector<shape*> &shapes, int depth, vec3 centre, float size, shape* s) {
    if (depth == 3) return; // max depth of fractal

    tmat c = translate(centre.x, centre.y, centre.z);
    tmat sc = scale(size, size, size);
    tmat rot = rotateZ(50) * rotateY(50);
    shapes.push_back(new instance(c * sc * rot, s));

    if (depth % 2 != 0)
        shapes.back()->mirror = true;

    initFractalCubes(shapes, depth + 1, centre + vec3(size*1.5, 0, 0), size * 0.5, s);
    initFractalCubes(shapes, depth + 1, centre + vec3(-size*1.5, 0, 0), size * 0.5, s);
    initFractalCubes(shapes, depth + 1, centre + vec3(0, size*1.5, 0), size * 0.5, s);
    initFractalCubes(shapes, depth + 1, centre + vec3(0, -size*1.5, 0), size * 0.5, s);
    initFractalCubes(shapes, depth + 1, centre + vec3(0, 0, size*1.5), size * 0.5, s);
    initFractalCubes(shapes, depth + 1, centre + vec3(0, 0, -size*1.5), size * 0.5, s);
}

/* Load the fractal scene made of spheres */
void initFractalSpheres(std::vector<shape*> &shapes, int depth, vec3 centre, float size, shape* s) {
    if (depth == 3) return; // max depth of fractal

    tmat c = translate(centre.x, centre.y, centre.z);
    tmat sc = scale(size, size, size);
    shapes.push_back(new instance(c * sc, s));

    if (depth % 2 != 0)
        shapes.back()->mirror = true;

    initFractalSpheres(shapes, depth + 1, centre + vec3(size*2, 0, 0), size * 0.5, s);
    initFractalSpheres(shapes, depth + 1, centre + vec3(-size*2, 0, 0), size * 0.5, s);
    initFractalSpheres(shapes, depth + 1, centre + vec3(0, size*2, 0), size * 0.5, s);
    initFractalSpheres(shapes, depth + 1, centre + vec3(0, -size*2, 0), size * 0.5, s);
    initFractalSpheres(shapes, depth + 1, centre + vec3(0, 0, size*2), size * 0.5, s);
    initFractalSpheres(shapes, depth + 1, centre + vec3(0, 0, -size*2), size * 0.5, s);
}

/* Initialize the shapes to be rendered in the scene */
inline void initShapes(std::vector<shape*> &shapes, const int &id)
{
    shapes.push_back(new plane (vec3(0,20, 0), vec3(0,1,0), rgb(1,1,1)));

    if (id == 1) { // default boring dummy scene
        shapes.push_back(new sphere (vec3(180,250,-200), 50.0f, rgb(0.1,0.7,0.7)));
        shapes.push_back(new sphere (vec3(400,250,-200), 100.0f, rgb(0.9,0.9,0.0)));
        shapes.push_back(new sphere (vec3(300,100,-200), 60.0f, rgb(0.9,0.1,0.7)));
        shapes.back()->mirror = true;
    }

    else if (id == 2) { // every second sphere is a mirror, shows instancing
        for (int i = 0; i < 10; i++) {
            tmat moveSphere = translate(i*200, 300, -200);
            sphere* s = new sphere(vec3(0,0,0), 100.0f, rgb(0.1,0.7,0.7));
            shapes.push_back(new instance(moveSphere, s));
            if (i % 2 == 0) {
                shapes.back()->mirror = true;
            }
        }
    }

    else if (id == 3) { // scene that shows we can load a triangle mesh
        tmat move = translate(200, 200, -50);
        tmat scaleMesh = scale(100, 100, 100);
        triangleMesh* m = new triangleMesh("cube.mesh", red);
        shapes.push_back(new instance(move * scaleMesh, m));
    }

    else if (id == 4) { // scene showing that we can move stuff around with matrices
        tmat move = translate(200, 200, -50);
        tmat scaleSphere = scale(100, 100, 100);
        sphere* s = new sphere(vec3(0,0,0), 1.0f, rgb(0.1,0.7,0.7));
        shapes.push_back(new instance(move * scaleSphere, s));
    }

    else if (id == 5) { // scene containing lots of cubes!
        triangleMesh* m1 = new triangleMesh("cube.mesh", rgb(0.1,0.7,0.7));
        triangleMesh* m2 = new triangleMesh("cube.mesh", rgb(0.9,0.9,0.1));
        sphere* s = new sphere(vec3(0,0,0), 1.0f, rgb(0.3,0.7,0.0));

        shapes.push_back(new instance(translate(50, 260, -400) * scale(200, 200, 200) * rotateZ(70), m1));
        shapes.push_back(new instance(translate(100, 500, -50) * scale(90, 90, 90) * rotateY(56), m2));
        shapes.push_back(new instance(translate(500, 400, -150) * scale(200, 200, 200) * rotateZ(50), m1));
        shapes.push_back(new instance(translate(600, 210, -300) * scale(90, 90, 90) * rotateX(90), m2));
        shapes.push_back(new instance(translate(700, 600, -200) * scale(90, 90, 90) * rotateX(90), m1));
    }

    else if (id == 6) { // cubes with rotations in fractal
        triangleMesh* m = new triangleMesh("cube.mesh", rgb(0.1,0.7,0.7));
        initFractalCubes(shapes, 0, vec3(WIDTH/2, HEIGHT/2, -200), 100.0f, m);
    }

    else if (id == 7) { // spheres as fractal
        sphere* s = new sphere(vec3(0,0,0), 1.0f, rgb(0.1, 0.7, 0.7));
        initFractalSpheres(shapes, 0, vec3(WIDTH/2, HEIGHT/2, -200), 100.0f, s);
    }
}

/* Randomly pick a place to sample in the pixel coordinate system */
inline vec2 sample(const int &i, const int &j)
{  
    return vec2(drand48() + i - 0.5f, drand48() + j - 0.5f);
}

/* Return a ray to fire off at the scene, from a given point to give us perspective */
inline ray getRayWithPerspective(const int &i, const int &j, const vec3 &origin)
{
    vec2 p = sample(i, j);
    vec3 originToPixel(makeUnitVector(vec3(p.x, p.y, 0) - origin));
    return ray(origin, originToPixel);
}

/* Return a ray to fire off at the scene. Note that the origin of each ray changes here
* as opposed to in getRayWithPerspective.
*/
inline ray getRayOrthogonal(const int &i, const int &j, const vec3 &dir)
{
    vec2 p = sample(i, j);
    return ray(vec3(p.x, p.y, 0), dir);
}


int main(void)
{
    vec3 eye(WIDTH/2, HEIGHT/2, 400);      // Where to shoot rays from
    hitRecord rec;                         // Contains information concerning what we hit
    std::vector<shape*> shapes;            // List of shapes in the scene 
    std::vector<pointLight*> pointLights;  // List of pointLights in the scene

    initLights(pointLights);
    initShapes(shapes, 6);   // init shapes for the scene, number is id of scene

    std::cout << "Ray tracing... (number of shapes = " << shapes.size() << ")\n";

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

    std::cout << "Done ray tracing.\nWriting out.ppm.\n";

    // Output the image to a ppm file
    image.writePPM("out.ppm");
    std::cout << "Done writing out.ppm.\n";
    
}