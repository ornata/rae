#include <iostream>
#include "vec.hpp"
#include "onb.hpp"
#include "dynarray.hpp"

int main(void)
{
    rgb r1(1,3,-4);
    rgb r2(1,1,1);

    std::cout << indexOfMaxComponentAbs(r1) << "\n";
}