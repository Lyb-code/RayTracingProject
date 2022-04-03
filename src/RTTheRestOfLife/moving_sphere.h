#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
            :center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) {} 

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& record) const override;
        virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;

        point3 center(double time) const;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
        
};

point3 moving_sphere::center(double time) const {
    return center0 + (center1 - center0) * ((time - time0) / (time1 - time0));
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared(); 
    auto half_b = dot(r.direction(), oc); 
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return false;
    } 
    auto sqrtd = sqrt(discriminant);
    // Find the nearst root in [t_min, t_max]
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }
    record.t = root;
    record.p = r.at(root);
    vec3 outward_normal = (record.p - center(r.time())) / radius;
    record.set_face_normal(r, outward_normal);
    record.mat_ptr = mat_ptr;
    
    return true;
}

bool moving_sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    aabb box0(
        center(time0) - vec3(radius, radius, radius),
        center(time0) + vec3(radius, radius, radius));

    aabb box1(
        center(time1) - vec3(radius, radius, radius),
        center(time1) + vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);
    return true;
}
#endif