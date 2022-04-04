#include "rtweekend.h"
#include "color.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include <iostream>

color ray_color(const ray& r, const hittable& world, const color& background, int depth) {
    hit_record rec;
    // If the ray bounce limit is exceeded, no more light is collected.
    if (depth <= 0) 
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        double pdf;
        color albedo;

        if (rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf)) {
            auto on_light = point3(random_double(213, 343), 554, random_double(227, 332));
            auto to_light = on_light - rec.p;
            auto distance_squared = to_light.length_squared();
            to_light = unit_vector(to_light);

            if (dot(to_light, rec.normal) < 0)
                return emitted;
            
            double light_area = (343-213) * (332-227);
            auto light_cosine = to_light.y();//The normal of light plane is (0, -1, 0);
            if (light_cosine < 0.000001)
                return emitted;

            pdf = distance_squared / (light_cosine*light_area);
            scattered = ray(rec.p, to_light, r.time());

            return emitted 
                + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                         * ray_color(scattered, world, background, depth-1) / pdf;
        } else {
            return emitted;
        }
    } else {
        //If the ray hits nothing, return background color.
        return background;
    }
}


hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(162, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    objects.add(box2);

    return objects;
}

int main() {
    // Image
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    // world
    hittable_list world = cornell_box();
    color background(0, 0, 0);

    // camera
    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0,1,0);
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto dist_to_focus = 10.0;
    auto time0 = 0.0;
    auto time1 = 1.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int k = 0; k < samples_per_pixel; k++) {
                auto u = (i+random_double()) / (image_width-1);
                auto v = (j+random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, background, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }

    }
    std :: cerr << "\nDone.\n";

}