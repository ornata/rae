#include <iostream>
#include <ctime>
#include "vec.hpp"
#include "onb.hpp"
#include "dynarray.hpp"
#include "rng.hpp"
#include "shape.hpp"

int main(void)
{
    vec3 v0(0.132, 1, 4);
    vec3 v1(2, 3, 5);
    vec3 v2(10, 4, 4);
    rgb c(255,0,0);
    float r = 1;
    triangle t(v0,v1,v2,c);
    sphere s(v1, r, c);
    std::cout << t << "\n";
    std::cout << s << "\n";
}