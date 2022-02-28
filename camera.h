#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(
            point3 lookfrom, 
            point3 lookat,
            vec3   vup,
            double vfov, // vfov: vertical field-of-view in degrees
            double aspect_ratio
        ) { 
            auto theta = degrees_to_radians(vfov);
            auto viewport_height = 2.0 * tan(theta/2);
            auto viewport_width = viewport_height * aspect_ratio;
            
            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            low_left_corner = origin - w - horizontal/2 - vertical/2; //focal_length = (-w).length() = 1
        }

        ray get_ray(double s, double t) const {
            return ray(origin, low_left_corner + s*horizontal + t*vertical - origin);
        }

    public:
        point3 origin;
        vec3 horizontal;
        vec3 vertical;
        point3 low_left_corner;
};

#endif