#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
    public:
        constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
            : boundary(b), phase_function(make_shared<isotropic>(a)), neg_inv_density(-1/d) {}

        constant_medium(shared_ptr<hittable> b, double d, color c)
            : boundary(b), phase_function(make_shared<isotropic>(c)), neg_inv_density(-1/d) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            return boundary->bounding_box(time0, time1, output_box);
        }

    public:
        shared_ptr<hittable> boundary;
        shared_ptr<material> phase_function;
        double neg_inv_density;
};

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    hit_record rec1, rec2;
    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t+0.0001, infinity, rec2))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << "\nt_max=" << rec2.t << '\n';

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t <= 0)
        rec1.t = 0; //ray origin inside the volume

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());//A distance where the scattering occurs.

    if (hit_distance > distance_inside_boundary)
        return false;

    record.t = rec1.t + hit_distance / ray_length;
    record.p = r.at(record.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "record.t = " << record.t << '\n'
                  << "record.p = " << record.p << '\n';
    }

    record.normal = vec3(1, 0, 0); // arbitrary
    record.front_face = true; // aslo arbitrary
    record.mat_ptr = phase_function;

    return true;
}

#endif