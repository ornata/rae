#include <iostream>
#include "vec.hpp"

int main(void)
{
    rgb r1(2,2,2);
    rgb r2(3,3,3);

    std::cout << r1 + r2 << "\n";
    std::cout << r1 * r2 << "\n";
    std::cout << r2 / r1 << "\n";
    std::cout << r1 * 2 << "\n";

    r2 = mix(r2, r1, r1*2);

    std::cout << r2 << "\n";
}