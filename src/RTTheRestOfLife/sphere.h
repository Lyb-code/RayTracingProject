#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "pdf.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 p, double r, shared_ptr<material> m) 
            : center(p), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
        virtual double pdf_value(const point3& origin, const vec3& v) const override;
        virtual vec3 random(const point3& origin) const override;

    private:
        static void get_sphere_uv(const point3& p, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }

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
    get_sphere_uv(outward_normal, record.u, record.v);
    
    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

double sphere::pdf_value(const point3& origin, const vec3& v) const {
    hit_record rec;
    if (!this->hit(ray(origin, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius*radius/(center-origin).length_squared());
    auto solid_angle = 2*pi*(1-cos_theta_max);
    return 1/solid_angle;
}

vec3 sphere::random(const point3& origin) const {
    vec3 direction = center - origin;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

#endif