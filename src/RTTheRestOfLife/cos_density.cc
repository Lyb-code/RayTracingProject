#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#include <math.h>

inline vec3 random_cosine_direction() {
    //pdf(directions) = cos(theta)/pi 
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);//Because the range of random_double is [0,1), take 1-r2 instead of r2. Thus the value of z will not be 0.
    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);
    return vec3(x, y, z);
}

int main() {
    int N = 1000000;

    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        auto v = random_cosine_direction();
        sum += v.z()*v.z()*v.z() / (v.z()/pi);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Pi/2     = " << pi/2 << '\n';
    std::cout << "Estimate = " << sum/N << '\n';
}