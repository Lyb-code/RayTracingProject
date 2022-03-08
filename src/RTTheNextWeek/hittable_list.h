#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include <memory>
#include <vector>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void add(shared_ptr<hittable> object) { objects.push_back(object); }
        void clear() { objects.clear(); }
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
    hit_record temp_rec;
    double closet_t_max = t_max;
    bool hit_anything = false;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closet_t_max, temp_rec)) {
            record = temp_rec;
            closet_t_max = temp_rec.t;
            hit_anything = true;
        }
    }

    return hit_anything;
}

#endif