#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Replace NaN components with zero. NaN does not equal itself;
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0; 

    // divide the color by the number of samples and gama-correct for gama = 2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r*scale);
    g = sqrt(g*scale);
    b = sqrt(b*scale);

    //write the translated [0, 255] value of each color component(rgb).
    out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' ' 
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << ' ' 
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
}

#endif