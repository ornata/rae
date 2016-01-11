#include "img.hpp"
#include <iostream>

img::img(int w, int h)
{
    nx = w;
    ny = h;
    raster = new rgb*[nx];

    for (int i = 0; i < nx; i++) {
        raster[i] = new rgb[ny];
    }
}

img::img(int w, int h, rgb bg)
{
    nx = w;
    ny = h;

    for (int i = 0; i < nx; i++) {
        raster[i] = new rgb[ny];
        for (int j = 0; j < ny; j++) {
            raster[i][j] = bg;
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

    raster[x][y] = colour;
    return true;
}

void img::gammaCorrect(float g)
{
    rgb tmp;
    float power = 1.0 / g;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            tmp = raster[i][j];
            raster[i][j] = (rgb(pow(tmp.r, power)),
                            rgb(pow(tmp.g, power)),
                            rgb(pow(tmp.b, power)));
        }
    }
}

void img::writePPM(std::ostream &out)
{
    // header
    out << "P6\n";
    out << nx << " " << ny << "\n";
    out << "255\n";

    int i, j;
    uint32_t ired, igreen, iblue;
    uint8_t red, green, blue;
    
    // output clamped to [0, 255]
    for (i = ny-1; i >= 0; i--) {
        for (j = 0; j < nx; j++) {
            ired = (uint32_t) (256 * raster[i][j].r);
            igreen = (uint32_t) (256 * raster[i][j].g);
            iblue = (uint32_t) (256 * raster[i][j].b);

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

void img::readPPM(std::string fname)
{
    std::ifstream in;
    in.open(fname.c_str());

    if (!in.is_open()) {
        std::cerr << "ERROR: Could not open '" << fname << "'.\n";
        exit(-1);
    }

    char ch, type;
    char red, green, blue;

    int i, j, cols, rows;
    int num;

    // read header
    in.get(ch);
    in.get(type);

    in >> cols >> rows >> num;

    nx = cols;
    ny = rows;

    // allocate raster
    raster = new rgb*[nx];

    for (i = 0; i < nx; i++) {
        raster[i] =  new rgb[ny];
    }

    // clean up the newline
    in.get(ch);

    // store the ppm pixel values in the raster
    for (i = ny-1; i >= 0; i--) {
        for (j = 0; j < nx; j++) {
            in.get(red);
            in.get(green);
            in.get(blue);
            raster[j][i] = rgb((float) ((uint8_t)red)/255.0,
                               (float) ((uint8_t)green)/255.0,
                               (float) ((uint8_t)blue)/255.0);
        }
    }
}
