#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <climits>
#if INT_MAX == 2147483647
    typedef int int32_t;
#elif LONG_MAX == 2147483647
    typedef long int32_t;
#elif SHRT_MAX == 2147483647
    typedef short int32_t;
#else
#error "Cannot find 32 bit integer"
#endif


#if LONG_MAX == 9223372036854775807
    typedef long int64_t;
#elif LLONG_MAX == 9223372036854775807
    typedef long long int64_t;
#else
#error "Cannot find 64 bit integer"
#endif

const double EPSILON = 0.00001;

struct divide_by_zero_error : public std::domain_error {
    divide_by_zero_error() : std::domain_error("cannot divide by zero") { }
};

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

int float_cmp(double x, double y);

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
