#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 p, double r, shared_ptr<material> m) 
            : center(p), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
    vec3 oc = r.origin() - center;
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
    vec3 outward_normal = (record.p - center) / radius;
    record.set_face_normal(r, outward_normal);
    record.mat_ptr = mat_ptr;
    
    return true;
}

#endif