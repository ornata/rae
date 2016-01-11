#include <iostream>
#include <ctime>
#include "vec.hpp"
#include "onb.hpp"
#include "dynarray.hpp"
#include "rng.hpp"

int main(void)
{
    rng r;
    std::cout << r() << "\n";
    std::cout << r() << "\n";
    std::cout << r() << "\n";
}