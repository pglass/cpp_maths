#include "common.hpp"



int float_cmp(double x, double y) {
    double d = y - x;
    if (d < -EPSILON)
        return -1;
    else if (d > EPSILON)
        return 1;
    return 0;
}
