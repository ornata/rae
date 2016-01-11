#include <iostream>
#include "vec.hpp"
#include "onb.hpp"
#include "dynarray.hpp"

int main(void)
{
    dynArray<int> d(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.append(5);
    d.truncate();
    std::cout << d << "\n";
}