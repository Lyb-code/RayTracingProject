#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

// usings
using std::sqrt;
using std::shared_ptr;
using std::make_shared;

// constants
const double inifinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// untility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// common headers
#include "ray.h"
#include "vec3.h"

#endif