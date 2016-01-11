/*
* img.hpp
* Contains the img class, which allows the user to
* alter/output/update .ppm files.
*/

#ifndef IMG_H
#define IMG_H

#include <cmath>
#include <string>
#include <fstream>
#include <cstdint>
#include "vec.hpp"

class img
{
public:
    img(int w, int h); // ppm with width w, height h
    img(int w, int h, rgb bg); // ppm with background bg

    bool set(int x, int y, const rgb &colour);
    void gammaCorrect(float g);
    void writePPM(std::ostream &out);
    void readPPM(std::string fname);

private:
    rgb** raster;
    int nx;
    int ny;
};

#endif