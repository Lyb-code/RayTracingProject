#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    //write the translated [0, 255] value of each color component(rgb).
    out << static_cast<int>(pixel_color.x() * 255.999) << ' ' 
        << static_cast<int>(pixel_color.y() * 255.999) << ' ' 
        << static_cast<int>(pixel_color.z() * 255.999) << '\n';
}

#endif