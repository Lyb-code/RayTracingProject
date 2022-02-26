#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * aspect_ratio;
            auto focal_length = 1.0;

            origin = point3(0, 0, 0);
            horizontal = vec3(viewport_width, 0, 0);
            vertical = vec3(0, viewport_height, 0);
            low_left_corner = origin - vec3(0, 0, focal_length) - horizontal/2 - vertical/2;
        }

        ray get_ray(double u, double v) {
            return ray(origin, low_left_corner + u*horizontal + v*vertical - origin);
        }

    public:
        point3 origin;
        vec3 horizontal;
        vec3 vertical;
        point3 low_left_corner;
};

#endif