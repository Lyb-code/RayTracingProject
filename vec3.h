#ifndef VEC3_H
#define VEC3_H

#include<cmath>
#include<iostream>

using std::sqrt;

class vec3 {
    public:
        vec3() : e{0, 0, 0} {}
        vec3(double e1, double e2, double e3) : e{e1, e2, e3} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const { return sqrt(length_squared()); }

        double length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }


    public:
        double e[3];

};

// Aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// vec3 Utility functions
inline std::ostream& operator<<(std::ostream &out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(const double t, const vec3& v) {
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(const vec3& v, const double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, const double t) {
    return (1/t) * v;
}

inline double dot(const vec3& v1, const vec3& v2) {
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
                v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2],
                v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

#endif