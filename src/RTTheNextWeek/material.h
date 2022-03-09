#ifndef MATERIAL_H
#define MATERILA_H

#include "rtweekend.h"
#include "texture.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a): albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}
        
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction, r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};

class metal : public material {
    public:
        metal(const color& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {}
        
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected_dir = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected_dir + fuzz * random_in_unit_sphere(), r_in.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1, 1, 1); // the glass surface absorbs nothing
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1 - cos_theta*cos_theta);
            bool cannot_refract = refraction_ratio*sin_theta > 1.0;

            vec3 scatter_direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                scatter_direction = reflect(unit_direction, rec.normal);
            } else {
                scatter_direction = refract(unit_direction, rec.normal, refraction_ratio);
            }
            scattered = ray(rec.p, scatter_direction, r_in.time());
            return true;
        }

    public:
        double ir; // Index of refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for relectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif