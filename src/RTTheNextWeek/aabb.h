#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
    public:
        aabb() {}
        aabb(const point3& min, const point3& max) : minimum(min), maximum(max) {}

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const {
            for (int i = 0; i < 3; i++) {
                auto invD = 1.0f / r.direction()[i];
                auto t0 = (min()[i] - r.origin()[i]) * invD;
                auto t1 = (max()[i] - r.origin()[i]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min; //take the bigger value
                t_max = t1 < t_max ? t1 : t_max;
                if (t_min >= t_max)
                    return false;
            }
            return true;
        }

    public:
        point3 minimum;
        point3 maximum;
};

aabb surrounding_box(const aabb& box0, const aabb& box1) {
    // computes the bounding box of two boxes
    point3 minP(fmin(box0.min().x(), box1.min().x()), 
                fmin(box0.min().y(), box1.min().y()), 
                fmin(box0.min().z(), box1.min().z()));

    point3 maxP(fmax(box0.max().x(), box1.max().x()), 
                fmax(box0.max().y(), box1.max().y()), 
                fmax(box0.max().z(), box1.max().z()));
    
    return aabb(minP, maxP);
}

#endif