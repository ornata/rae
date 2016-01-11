/*
* rng.hpp
* Defines a simple random number generator.
*/

#ifndef _RNG_H_
#define _RNG_H_

#include <cstdint>
#include <cfloat>

struct rng
{
    uint64_t seed, mult;

    rng(uint64_t seed_ = 7564231)
    {
        seed = seed_;
        mult = 62089911;

    }

    float operator()()
    {
        seed *= mult;
        return float(seed % UINT64_MAX) / FLT_MAX;
    }
};

#endif