#ifndef _INT_HPP_
#define _INT_HPP_
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>
#include <deque>
#include <cassert>
#include "common.hpp"

/* Int - a (virtually) arbitrary precision integer
 *
 * All the arithmetic operators are overloaded. Division truncates the fractional part.
 * The modulo operator works according to the "floored division" method
 * (http://en.wikipedia.org/wiki/Modulo_operation) which may be different than 
 * modulo on ints in C++ for negative numbers.
 *
 * === Implementation notes ===
 *  Conceptually, this is implemented as a list of bins where each bin contains 
 *  some number of decimal digits (Int::BIN_WIDTH), and arithmetic is done bin-by-bin.
 *  Concretely, each bin is a (positive) int with value at most 10^BIN_WIDTH (== Int::BIN_LIMIT).
 *  A 32-bit int can store at most 9 decimal digits, so BIN_WIDTH is fixed to 9.
 *  Bins are ordered from least-significant first (index 0) to most-significant.
 *  
 *  This uses 64-bit ints to temporarily store the product of two 32-bit ints. If there
 *  is at most a 32-bit int available, then the code would need to be changed to use 
 *  16 bits per bin (4 decimal digits) so that the product of two bins does not 
 *  overflow a 32-bit int.
 *  common.hpp tries to define int32_t and int64_t as 32-bit and 64-bit signed integers.
 */
class Int {
  public:
    static const int32_t BIN_WIDTH = 9;
    static const int32_t BIN_LIMIT = 1000000000;

    Int();
    Int(const Int& x, size_t shift = 0);    /* shift is the number of bins */
    Int(int64_t x, size_t shift = 0);
    explicit Int(const std::string& x);

    friend std::ostream& operator<<(std::ostream& o, const Int& x);
    friend std::istream& operator>>(std::istream& i, Int& x);
    std::ostream& print(std::ostream& out) const;
    std::istream& read(std::istream& in);
    friend std::ostream& operator<<(std::ostream& o, const std::deque<int32_t>& deque);

    void set_value(int32_t x);
    bool equals_int32(int32_t x) const;
    int32_t cmp(const Int& y) const;
    void times_power_ten(int32_t power);

    // inline bool is_odd() const { return bins[0] % 2 == 1; }
    inline bool is_odd() const { return bins.at(0) % 2 == 1; }
    inline bool is_negative() const { return negative; }
    inline void negate() { negative = (equals_int32(0) ? false : !negative); }

    friend Int operator - (const Int& x);   /* negation */
    friend Int operator + (const Int& x, const Int& y);
    friend Int operator - (const Int& x, const Int& y);
    friend Int operator * (const Int& x, const Int& y);
    friend Int operator / (const Int& x, const Int& y);
    friend Int operator % (const Int& x, const Int& y);
    friend Int operator ^ (const Int& x, const Int& y);

    void operator += (const Int& other);
    void operator -= (const Int& other);
    void operator *= (const Int& other);
    void operator /= (const Int& other);
    void operator %= (const Int& other);
    void operator ^= (const Int& other);

    inline bool operator < (const Int& other) const { return cmp(other) < 0; }
    inline bool operator > (const Int& other) const { return cmp(other) > 0; }
    inline bool operator <= (const Int& other) const { return cmp(other) <= 0; }
    inline bool operator >= (const Int& other) const { return cmp(other) >= 0; }
    inline bool operator != (const Int& other) const { return cmp(other) != 0; }
    inline bool operator == (const Int& other) const { return cmp(other) == 0; }
  private:
    std::deque<int32_t> bins;
    bool negative;

    int32_t cmp_bins(const Int& x) const;

    inline void shift(size_t amount) {
        for (size_t i = 0; i < amount; ++i)
            bins.push_front(0);
    }
//    inline void set_bin_from_back(int32_t i, int32_t val) { bins[bins.size() - 1 - i] = val; }
    inline void set_bin_from_back(int32_t i, int32_t val) {
//        std::cout << "set_bin_from_back" << std::endl;
//        std::cout << "  bins.size() = " << bins.size() << std::endl;
//        std::cout << "  1 + i = " << (1 + i) << std::endl;
//        std::cout << "  bins.size() - (1+i) = " << (bins.size() - 1 - i) << std::endl;
//        bins.at(bins.size() - 1 - i) = val;
        bins[bins.size() - 1 - i] = val;
    }
//    inline int32_t get_bin_from_back(int32_t i) const { return bins[bins.size() - 1 - i]; }
    inline int32_t get_bin_from_back(int32_t i) const { return bins.at(bins.size() - 1 - i); }

    /*
     * A number is stored in "reverse" order.
     *      1111 222333444 555666777
     * is stored as
     *      [555666777, 222333444, 1111]
     * but may also be stored correctly as
     *      [555666777, 222333444, 1111, 0, 0, ..., 0]
     * This method removes all trailing zeroes from the number.
     *
     * Note: This is part of a (kind of) hack to correct some algorithms. Ideally, we shouldn't
     * need to call this. The problem is `y.bins.size() > x.bins.size()` doesn't necessarily mean
     * `abs(y) > abs(x)`, which is an easy mistake to make. It would be more desireable to gurantee
     * this property.
     */
    void cleanBins();

    /* All the following functions are helpers to the operator overloads.
     * Some aren't member functions because they have awkward in-place versions.
     * They make various assumptions about inputs (add/subtract require positives)
     * Check the comments at the functions' definition for more.
     */
    void add(const Int& x);
    void subtract(const Int& x);
    void divide(const Int& x);
    friend void multiply(const Int& x, const Int& y, Int& result);
    friend void multiply_by_int(const Int& x, int32_t y, Int& result);
    friend void divide_by_int(const Int& x, int32_t y, Int& result);
    friend void quotient_and_remainder(const Int& y, const Int& x, int32_t& q, Int& r);
    friend void iter_quotient(const Int& y, const Int& x, int32_t& q, Int& r, int32_t step);
    friend void modulo(const Int& x, const Int& y, Int& result);
    friend void exponentiate(const Int& x, const Int& exp, Int& result);
};




#endif
