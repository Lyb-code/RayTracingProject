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
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
        ) { 
            auto theta = degrees_to_radians(vfov);
            auto viewport_height = 2.0 * tan(theta/2);
            auto viewport_width = viewport_height * aspect_ratio;
            
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            low_left_corner = origin - focus_dist*w - horizontal/2 - vertical/2;

            lens_radius = aperture / 2;
            time0 = _time0;
            time1 = _time1;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            
            return ray(
                origin + offset, 
                low_left_corner + s*horizontal + t*vertical - origin - offset, 
                random_double(time0, time1)
            );
        }

    public:
        point3 origin;
        vec3 horizontal;
        vec3 vertical;
        point3 low_left_corner;
        vec3 u, v, w;
        double lens_radius;
        double time0, time1; // shutter open/close times
};

#endif