#ifndef _NUMBERS_H_
#define _NUMBERS_H_
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>
#include <deque>

unsigned int parse_int(const std::string& x);

template <typename T> inline T abs(T x) { return x < 0 ? -x : x; }
inline int max(int a, int b) { return (a > b) ? a : b; }
inline int cmp_ints(int x, int y) { return (x < y) ? -1 : ((x > y) ? 1 : 0); }

struct divide_by_zero_error : public std::domain_error { 
    divide_by_zero_error() : std::domain_error("cannot divide by zero") { } 
};

/* Abstract Number base class.
 * Inherit from Number and implement print() instead of overriding operator<<
 */
struct Number { virtual std::ostream& print(std::ostream& out) const = 0; };
std::ostream& operator<<(std::ostream& out, const Number& x);

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
 *  A 32-bit int can store at most 9 decimal digits, so set BIN_WIDTH = 9.
 *  Then addition, for example, works by summing the first bins from each Int,
 *  computing the carry and adding that to the sum of the second bins from each Int,
 *  and so on, which takes the most advantage of integer arithmetic compared to
 *  having any fewer digits per bin.
 *  
 *  This assumes sizeof(int) = 4 (32 bits) and sizeof(long) = 8 (64 bits). 
 *  This use a 64-bit long to temporarily store the product of two ints. If there 
 *  is at most a 32-bit int available, then the code would need to be changed to use 
 *  16 bits per bin (4 decimal digits) so that the product of two bins does not 
 *  overflow a 32-bit int. (This can be done for portability, but that's a large task).
 */
class Int : public Number {
  public:
    static const size_t BIN_WIDTH = 9;
    static const unsigned int BIN_LIMIT = 1000000000;

    Int();
    Int(const Int& x, size_t shift);
    Int(long x, size_t shift = 0);
    Int(const std::string& x);

    friend Int operator - (const Int& x);
    friend Int operator + (const Int& x, const Int& y);
    friend Int operator - (const Int& x, const Int& y);
    friend Int operator * (const Int& x, const Int& y);
    friend Int operator / (const Int& x, const Int& y);
    friend Int operator % (const Int& x, const Int& y);
    friend Int operator ^ (const Int& x, const Int& y);

    friend void operator += (Int& x, const Int& y);
    friend void operator -= (Int& x, const Int& y);
    friend void operator *= (Int& x, const Int& y);
    friend void operator /= (Int& x, const Int& y);
    friend void operator %= (Int& x, const Int& y);
    friend void operator ^= (Int& x, const Int& y);

    friend bool operator < (const Int& x, const Int& y) { return x.cmp(y) < 0; }
    friend bool operator > (const Int& x, const Int& y) { return x.cmp(y) > 0; }
    friend bool operator >= (const Int& x, const Int& y) { return x.cmp(y) >= 0; }
    friend bool operator <= (const Int& x, const Int& y) { return x.cmp(y) <= 0; }
    friend bool operator != (const Int& x, const Int& y) { return x.cmp(y) != 0; }
    friend bool operator == (const Int& x, const Int& y) { return x.cmp(y) == 0; }

    virtual std::ostream& print(std::ostream& out) const;
  private:
    std::deque<unsigned int> bins;
    bool negative;

    /* All the following functions are helpers to the operator overloads.
     * Some aren't member functions because they have awkward in-place versions.
     * They make various assumptions about inputs (add/subtract require positives)
     * Check the comments at the functions' definition for more.
     */
    void add(const Int& x);
    void subtract(const Int& x);
    void divide(const Int& x);
    friend void multiply(const Int& x, const Int& y, Int& result);
    friend void multiply_by_int(const Int& x, int y, Int& result);
    friend void divide_by_int(const Int& x, int y, Int& result);
    friend void quotient_and_remainder(const Int& y, const Int& x, int& q, Int& r);
    friend void iter_quotient(const Int& y, const Int& x, int& q, Int& r, int step);
    friend void modulo(const Int& x, const Int& y, Int& result);
    friend void exponentiate(const Int& x, const Int& exp, Int& result);

    bool is_int(int x) const;
    void set_int(int x);
    int cmp(const Int& y) const;
    int cmp_bins(const Int& x) const;

    inline bool is_odd() const { return bins[0] % 2 == 1; }
    inline void negate() { negative = is_int(0) ? false : !negative; }
    inline void shift(size_t amount) { bins.push_front(0); }
    inline void set_bin_from_back(int i, int val) { bins[bins.size() - 1 - i] = val; }
    inline unsigned int get_bin_from_back(int i) const { return bins[bins.size() - 1 - i]; }
};

#endif
