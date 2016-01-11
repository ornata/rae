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

#define WIDTH 500
#define HEIGHT 500

int main(void)
{
    img image(WIDTH, HEIGHT);
    
    bool isHit;
    float tmax;
    vec3 dir(0,0,-1); // direction of viewing rays
    hitRecord rec;
    std::vector<shape*> shapes; // TODO: make this a faster data structure

    shapes.push_back(new triangle (vec3(300,600,-100), vec3(0,100,-100), vec3(450,20,-100), rgb(0.8,0.0,0.0)));
    shapes.push_back(new sphere (vec3(250,250,0), 150.0f, rgb(0.8,0.0,0.8)));

    // iterate over every pixel, firing off rays at objects
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            tmax = 100000.0f;
            isHit = false;
            ray r(vec3(i, j, 0), dir);

            for (auto s : shapes) {
                if (s->hit(r, 0.0001f, tmax, 0, rec)) {
                    tmax = rec.t;
                    isHit = true;
                }

                if (isHit == true) {
                    image.set(i, j, rec.colour);
                }

                else {
                    image.set(i, j, rgb(0.0, 0.0, 0.0));
                }
            }
        }
    }

    image.writePPM(std::cout);
}