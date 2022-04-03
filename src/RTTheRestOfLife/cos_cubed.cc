#include "rtweekend.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

int main() {
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        auto r1 = random_double();
        auto r2 = random_double();
        auto x = cos(2*pi*r1) * sqrt(r2*(2-r2));
        auto y = sin(2*pi*r1) * sqrt(r2*(2-r2));
        auto z = 1 - r2;
        sum += z*z*z / (0.5/pi);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Pi/2     = " << pi/2 << '\n';
    std::cout << "Estimate = " << sum/N << '\n';
}