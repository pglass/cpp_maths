#ifndef _INT_HPP_
#define _INT_HPP_
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>
#include <deque>
#include "./common.hpp"

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
 *  Concretely, each bin is a (positive) int with value at most 10^BIN_WIDTH.
 *  A 32-bit int can store at most 9 decimal digits, so BIN_WIDTH = 9.
 *  Then addition, for example, works by summing the first bins from each Int,
 *  computing the carry and adding that to the sum of the second bins from each Int,
 *  and so on, which takes the most advantage of integer arithmetic compared to
 *  having any fewer digits per bin.
 *
 *  Furthermore, the least significant bin is stored at the lowest index.
 *  
 *  common.hpp tries to define int32_t and int64_t as 32-bit and 64-bit signed integers.
 *  This uses 64-bit ints to temporarily store the product of two 32-bit ints. If there
 *  is at most a 32-bit int available, then the code would need to be changed to use 
 *  16 bits per bin (4 decimal digits) so that the product of two bins does not 
 *  overflow a 32-bit int.
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

    bool equals_int32(int32_t x) const;
    int32_t cmp(const Int& y) const;
    void times_power_ten(int32_t power);

    inline bool is_odd() const { return bins[0] % 2 == 1; }
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

    void set_value(int32_t x);
    int32_t cmp_bins(const Int& x) const;

    inline void shift(size_t amount) { bins.push_front(0); }
    inline void set_bin_from_back(int32_t i, int32_t val) { bins[bins.size() - 1 - i] = val; }
    inline int32_t get_bin_from_back(int32_t i) const { return bins[bins.size() - 1 - i]; }

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
