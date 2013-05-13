#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <iostream>
#include <sstream>
#include <cstdint>

const double EPSILON = .00001;

template <typename T>
inline T abs(const T& a) {
    return (a < 0) ? -a : a;
}

inline int float_equals(double x, double y) {
    return abs(x - y) < EPSILON; 
}

inline bool isdigit(char c) {
    return '0' <= c && c <= '9';
}

inline int cmp_ints(int32_t x, int32_t y) {
    return (x < y) ? -1 : ((x > y) ? 1 : 0); 
}

inline int max(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}

int float_cmp(double x, double y) {
    double d = y - x;
    if (d < -EPSILON) 
        return -1;
    else if (d > EPSILON) 
        return 1;
    return 0;
}

template <typename T>
bool testOutput(const T& y, const std::string& expected) {
    std::stringstream ss;
    ss << y;
    bool result = (ss.str().compare(expected) == 0);
    if (!result)
        std::cout << "FAILED: " << ss.str() << " : " << expected << std::endl;
    return result;
}

#endif
