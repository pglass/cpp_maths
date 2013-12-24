#include "common.hpp"

bool isFinite(double d) {
   return (d == d
           && d != std::numeric_limits<double>::infinity()
           && d != -std::numeric_limits<double>::infinity());
}

int float_cmp(double x, double y) {
    double d = y - x;
    if (d < -EPSILON)
        return -1;
    else if (d > EPSILON)
        return 1;
    return 0;
}
