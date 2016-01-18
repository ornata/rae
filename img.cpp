/*
* img.cpp
* Contains implementations of functions from img.hpp.
* Allows the user to read, output, and alter .ppm images.
*/

#include "img.hpp"

img::img(int w, int h)
{
    nx = w;
    ny = h;
    raster = new rgb*[ny];

    for (int j = 0; j < ny; j++) {
        raster[j] = new rgb[nx];
    }
}

img::img(int w, int h, rgb bg)
{
    nx = w;
    ny = h;

    for (int j = 0; j < ny; j++) {
        raster[j] = new rgb[nx];
        for (int i = 0; i < nx; i++) {
            raster[j][i] = bg;
        }
    }
}

bool img::set(int x, int y, const rgb &colour)
{
    // are we in bounds?
    if (x < 0 || x > nx) {
        return false;
    }

    if (y < 0 || y > ny) {
        return false;
    }

    raster[y][x] = colour;
    return true;
}

void img::gammaCorrect(float g)
{
    rgb tmp;
    float power = 1.0 / g;

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            tmp = raster[j][i];
            raster[j][i] = (rgb(pow(tmp.r, power)),
                            rgb(pow(tmp.g, power)),
                            rgb(pow(tmp.b, power)));
        }
    }
}

void img::writePPM(std::ostream &out)
{
    int i, j;
    uint32_t ired, igreen, iblue;
    uint8_t red, green, blue;

    // write header to output
    out << "P6\n";
    out << nx << " " << ny << "\n";
    out << "255\n";
    
    // output clamped to [0, 255]
    for (j = 0; j < ny; j++) {
        for (i = 0; i < nx; i++) {
            ired = (uint32_t) (256 * raster[j][i].r);
            igreen = (uint32_t) (256 * raster[j][i].g);
            iblue = (uint32_t) (256 * raster[j][i].b);

            if (ired > 255) ired = 255;
            if (igreen > 255) igreen = 255;
            if (iblue > 255) iblue = 255;

            red = (uint8_t) ired;
            green = (uint8_t) igreen;
            blue = (uint8_t) iblue;

            out.put(red);
            out.put(green);
            out.put(blue);
        }
    }
}
