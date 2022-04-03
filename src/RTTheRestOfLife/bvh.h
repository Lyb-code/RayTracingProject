#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

class bvh_node : public hittable {
    public:
        bvh_node() {}
        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}

        bvh_node(
            const std::vector<shared_ptr<hittable>>& src_objects, 
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& record) const override;
        virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;
        

    public:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb box;
};

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, record);
    bool hit_right = right->hit(r, t_min, hit_left ? record.t : t_max, record);

    return hit_left || hit_right;
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);
inline bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
inline bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
inline bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& src_objects, 
    size_t start, size_t end, double time0, double time1
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects
    size_t span = end - start;
    int axis = random_int(0, 2);
    bool (*comparator)(const shared_ptr<hittable>, const shared_ptr<hittable>) = (axis == 0) ? box_x_compare :
        (axis == 1) ?  box_y_compare : box_z_compare; //function pointer

    if (span == 1) {
        left = objects[start];
        right = objects[start];
    } else if (span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + span/2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}

//The box comparision functions
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;
    //Get the bounding box at time 0!
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in box_compare.\n";
    
    return box_a.min()[axis] < box_b.min()[axis];
}

inline bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

inline bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

inline bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

#endif