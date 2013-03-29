#ifndef _INT_HPP_
#define _INT_HPP_
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>
#include <deque>
#include "./numbers.hpp"
#include "./common.hpp"

/* parse_int - parse an int from a string
 *   This throws an invalid_argument exception if x contains anything other 
 *   than an int. Use while constructing an Int from a string.
 */
unsigned int parse_int(const std::string& x) {
    unsigned int val;
    std::stringstream ss(x);
    ss >> val;
    if (ss.fail() or !ss.eof())
        throw std::invalid_argument("Cannot parse int from '" + x + "'");
    return val; 
}

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

std::ostream& Int::print(std::ostream& out) const {
    if (negative)
        out << '-';
    size_t i = 0;
    unsigned int x;
    while (i < bins.size() and (x = get_bin_from_back(i)) == 0)
        ++i;
    if (i == bins.size()) {
        out << (int) 0;
    } else {
        out << get_bin_from_back(i);    // don't pad the most significant bin
        ++i;
        for (; i < bins.size(); ++i)
            out << std::setw(BIN_WIDTH) << std::setfill('0') << get_bin_from_back(i);
    }
	return out;
}

Int operator+(const Int& x, const Int& y) {
    if (x.negative and y.negative)        // (-x) + (-y) --> -(x + y)
        return -(-x + (-y));
    else if (x.negative and !y.negative)  // (-x) + y --> -(x - y)
        return -(-x - y);
    else if (!x.negative and y.negative)  // x + (-y) --> x - y
        return x - (-y);

    if (x.bins.size() > y.bins.size()) {
        Int r(x);
        r.add(y);
        return r;
    } else {
        Int r(y);
        r.add(x);
        return r;
    }
}

Int operator-(const Int& x, const Int& y) {
    if (x.negative and y.negative)        // -x - (-y) --> -(x - y)
        return -(-x - (-y));
    else if (x.negative and !y.negative)  // -x - y --> -(x + y)
        return -(-x + y);
    else if (!x.negative and y.negative)  // x - (-y) --> x + y
        return x + (-y);

    if (x > y) {
        Int r(x);
        r.subtract(y);
        return r;
    } else {
        Int r(y);
        r.subtract(x);
        return -r;
    }
}

Int operator*(const Int& x, const Int& y) {
    Int r;
    if (x.bins.size() > y.bins.size())
        multiply(x, y, r);
    else
        multiply(y, x, r);

    if (x.negative != y.negative)
        r.negate();
    return r;
}

Int operator/(const Int& x, const Int& y) {
    Int r(x);
    r.divide(y);
    return r;
}

Int operator%(const Int& x, const Int& y) {
    Int r;
    modulo(x, y, r);
    return r;
}

Int operator^(const Int& x, const Int& y) {
    Int r;
    exponentiate(x, y, r);
    return r;
}

void operator+=(Int& x, const Int& y) {
    if (x.negative and y.negative) {         // (-x) + (-y) --> -(x + y)
        x.negate();
        x += (-y);
        x.negate();
    } else if (x.negative and !y.negative) { // (-x) + y --> -(x - y)
        x.negate();
        x -= y;
        x.negate();
    } else if (!x.negative and y.negative) { // x + (-y) --> x - y
        x -= (-y);
    } else {
        x.add(y);
    }
}

void operator-=(Int& x, const Int& y) {
    if (x.negative and y.negative) {         // -x - (-y) --> -(x - y)
        x.negate();
        x -= (-y);
        x.negate();
    } else if (x.negative and !y.negative) { // -x - y --> -(x + y)
        x.negate();
        x += y;
        x.negate();
    } else if (!x.negative and y.negative) { // x - (-y) --> x + y
        x += (-y);
    } else {
        if (x > y) {
            x.subtract(y);
        } else {
            Int r(y);
            r.subtract(x);
            r.negate();
            return std::swap(x, r);
        }
    }
}

void operator*=(Int& x, const Int& y) {
    Int r;
    multiply(x, y, r);
    r.negative = (x.negative != y.negative);
    std::swap(x, r);
}

void operator/=(Int& x, const Int& y) {
    x.divide(y);
}

void operator%=(Int& x, const Int& y) {
    Int r;
    modulo(x, y, r);
    std::swap(x, r);
}

void operator^=(Int& x, const Int& y) {
    Int r;
    exponentiate(x, y, r);
    std::swap(x, r);
}

Int operator-(const Int& x) {
    Int y(x);
    y.negate();
    return y;
}

/* Default constructor - zero is the default value */
Int::Int() {
    bins.push_back(0);
    negative = false;
}

/* Construct from a long and apply a shift.
 *   This puts shift zero bins in *this, followed by the value of x.
 *   That is, after construction, *this will have the value x * (BIN_LIMIT ^ shift).
 *   (This is used in multiplication and division)
 */
Int::Int(long x, size_t shift) {
    if (x < 0) {
        negative = true;
        x = -x;
    } else if (x == 0) {
        negative = false;
        bins.push_back(0);
        return;
    } else {
        negative = false;
    }
    for (size_t i = 0; i < shift; ++i)
        bins.push_back(0);
    while (x >= BIN_LIMIT) {
        bins.push_back(x % BIN_LIMIT);
        x /= BIN_LIMIT;
    }
    if (x > 0)
        bins.push_back((int) x);
}

/* Construct from an Int and apply a shift.
 *   This puts shift zero bins in *this, followed by the value of x.
 *   That is, after construction, *this will have the value x * (BIN_LIMIT ^ shift).
 *   (This is used in multiplication and division)
 */
Int::Int(const Int& x, size_t shift) {
    negative = x.negative;
    for (size_t i = 0; i < shift; ++i)
        bins.push_back(0);
    for (size_t i = 0; i < x.bins.size(); ++i)
        bins.push_back(x.bins[i]);
}

/* Construct an Int from a std::string.
 *   The string can be of the format: [+-][0-9]+
 *
 * This throws an invalid_argument exception when unable to convert any part 
 *   of x to an int (see parse_int)
 */
Int::Int(const std::string& x) {
    if (x.length() == 0) {
        bins.push_back(0);
        negative = false;
        return;
    }    

    size_t i = 0;
    negative = (x[i] == '-');
    if (negative or x[i] == '+')
        ++i;

    // skip leading zeroes
    size_t k = i;
    while (k < x.size() and x[k] == '0')
        ++k;
    if (k > i and k < x.size())
        i = k;
    else if (k > i) {
        bins.push_back(0);
        negative = false;
        return;
    }
  
    // now i is the offset after skipping '-' and leading zeroes.
    if (x.size() - i <= BIN_WIDTH) {
        bins.push_back(parse_int(x.substr(i, x.length() - i)));
    } else {
        // j is the index in x of the digit starting the first full bin.
        int j = x.size() - ((x.length() - i) / BIN_WIDTH) * BIN_WIDTH;
        for (int k = x.size() - BIN_WIDTH; k >= j; k -= BIN_WIDTH)
            bins.push_back(parse_int(x.substr(k, BIN_WIDTH)));
        if (j > (int) i)
            bins.push_back(parse_int(x.substr(i, j - i)));
    } 

    if (is_int(0))
        negative = false;
}

/* Int::is_int - return true if the value of *this is equal to x */
bool Int::is_int(int x) const {
    unsigned int val = abs(x);
    size_t i = 0;
    for (; i < bins.size() - 1; ++i)
        if (bins[i])
            return false;
    if (bins[i] == val and negative == (x < 0))
        return true;
    else if (bins[i] == val and val == 0)
        return true;
    return false;
}

/* Int::set_int - set the value of *this to val with correct sign */
void Int::set_int(int val) {
    bins.clear();
    bins.push_back(abs(val));
    negative = (val < 0);
}

/* cmp_bins - compare to x, ignoring the signs
 * returns -1 if *this < x, 
 *          1 if *this > x,
 *          0 if equal
 */
int Int::cmp_bins(const Int& x) const {
    int tmp;
    int offset = x.bins.size() - bins.size();
    if (offset > 0) {  // x has more bins
        for (int i = 0; i < offset; ++i)
            if (x.get_bin_from_back(i) != 0)
                return -1;
    } else if (offset < 0) {
        return -x.cmp_bins(*this);
    }
    if (offset < 0)
        offset = -offset;
    for (size_t i = 0; i < bins.size(); ++i)
        if ((tmp = cmp_ints(get_bin_from_back(i), x.get_bin_from_back(offset + i))))
            return tmp;
    return 0;
}

/* cmp - compare *this with x
 * returns -1 if *this < x, 
 *          1 if *this > x,
 *          0 if equal
 */
int Int::cmp(const Int& x) const {
    int bin_cmp = cmp_bins(x);
    if (bin_cmp == 0 and is_int(0)) {
        return 0;
    } else {
        if (negative and x.negative)
            return -bin_cmp;
        else if (!negative and !x.negative)
            return bin_cmp;
        else if (negative)
            return -1;
        else
            return 1;
    }
}

/* Int::add - add x to *this, ignoring signs */
void Int::add(const Int& x) {
    if (x.bins.size() > bins.size())
        bins.resize(x.bins.size(), 0);

    int carry = 0;
    unsigned int sum, val;
    size_t i = 0;
    for (; i < x.bins.size(); ++i) {
        sum = bins[i] + x.bins[i] + carry;
        val = sum % BIN_LIMIT;
        carry = max(0, sum / BIN_LIMIT);
        bins[i] = val;
    }

    while (carry != 0 and i < bins.size()) {
        sum = bins[i] + carry;
        val = sum % BIN_LIMIT;
        carry = max(0, sum / BIN_LIMIT);
        bins[i] = val;
        ++i;
    }

    if (carry != 0)
        bins.push_back(carry);
}

/* Int::subtract - subtract off x from *this
 *   This ignores signs and assumes that *this is larger than x in absolute value.
 */
void Int::subtract(const Int& x) {
    int diff;
    int borrow = 0;
    size_t i = 0;
    for (; i < x.bins.size(); ++i) {
        diff = bins[i] - borrow - x.bins[i];
        if (diff < 0) {
            borrow = 1;
            diff += BIN_LIMIT;
        } else {
            borrow = 0;
        }
        bins[i] = diff;
    }

    while (i < bins.size() and borrow != 0) {
        diff = bins[i] - borrow;
        if (diff < 0) {
            borrow = 1;
            diff += BIN_LIMIT;
        } else {
            borrow = 0;
        }
        bins[i] = diff;
        ++i;
    }
}

/* multiply_by_int - multiply an Int by an int
 *   This ignores signs. Pass in abs(y) and compute the sign afterwards.
 */
void multiply_by_int(const Int& x, int y, Int& result) {
    long prod;
    result = Int(0);
    for (size_t i = 0; i < x.bins.size(); ++i) {
        prod = ((long) y) * ((long) x.bins[i]);
        result += Int(prod, i);
    }
}

/* multiply - multiply two Ints together
 *   This ignores signs and assumes x has at least as many bins as y 
 */
void multiply(const Int& x, const Int& y, Int& result) {
    Int b;
    result = Int(0);
    for (size_t i = 0; i < y.bins.size(); ++i) {
        multiply_by_int(x, y.bins[i], b);
        result += Int(b, i);
    }
}

/* divide_by_int - divide and Int by an int
 *   This ignores signs. Pass in abs(y) and compute the sign aftwerwards.
 */
void divide_by_int(const Int& x, int y, Int& result) {
    result.bins.clear();
    long q, d;
    long r = 0;
    for (size_t i = 0; i < x.bins.size(); ++i) {
        d =  (r * Int::BIN_LIMIT) + x.get_bin_from_back(i);
        q = d / y;
        r = d % y;
        result.bins.push_front(q);
    }
    if (result.bins.size() == 0)
        result.bins.push_back(0);
}

/* iter_quotient - helper function for quotient_and_remainder
 *   This adds step to q and stop at the largest q such that x * q <= y.
 *   This assumes we'll have x and y so that 0 <= q < Int::BIN_LIMIT always.
 *
 *   We know y >= x, and we want to find the quotient q and remainder r 
 *   so that q * x + r = y. One way of doing so is to start with q = 0, and
 *   compute r = y - q*x for all q >= 0 until we find the an r less than x. 
 *   Obviously, this isn't efficient. This function allows us to iterate q by 
 *   various step sizes repeatedly, so we can do essentially a binary search 
 *   through possible values of q instead of a linear search.
 */
inline void iter_quotient(const Int& y, const Int& x, int& q, Int& r, int step) {
    int b = q;
    Int prod;
    do {
        q = b;
        b += step;
        multiply_by_int(x, b, prod);
        r = y - prod;
    } while (!r.negative and x.cmp_bins(r) <= 0);   
}

/* quotient_and_remainder - find q and r so that q * x + r = y.
 *   This assumes we'll have y and x so that x <= y < BIN_LIMIT * x,
 *   so we know after returning that q >= 1.
 */
void quotient_and_remainder(const Int& y, const Int& x, int& q, Int& r) {
    int step = Int::BIN_LIMIT;
    q = 0;
    while (step > 1) {
        step /= 2;
        iter_quotient(y, x, q, r, step); 
    }
    q += 1;
}

/* Int::divide - do a full, correct, in-place division of two Ints
 *   This handles the sign, and will throw a divide_by_zero_error
 *   when x is zero.
 */
void Int::divide(const Int& x) {
    if (x.is_int(0)) {
        throw divide_by_zero_error();
    } else if (cmp_bins(x) < 0) {  // division by a larger number truncates
        set_int(0);
        return;
    }

    Int r, current;
    int q;
    size_t i = 0;
    for (; i < bins.size(); ++i) {
        current.bins[0] = get_bin_from_back(i);

        if (x.cmp_bins(current) <= 0) {
            quotient_and_remainder(current, x, q, r);
            set_bin_from_back(i, q);
            current = Int(r);
        } 
        else {
            set_bin_from_back(i, 0);
        }
        current.shift(1);
    }

    negative = (negative != x.negative);
}

/* modulo - compute the modulus of two Ints according to the "floored division" 
 *   (see: http://en.wikipedia.org/wiki/Modulo_operation)
 */
void modulo(const Int& x, const Int& y, Int& result) {
    Int a(x);
    Int b(y);
    if (a.negative)
        a.negate();
    if (b.negative)
        b.negate();

    Int q = a / b;
    result = a - q * b;

    if (result.is_int(0))
        return;
    if (x.negative and y.negative)
        result.negate();
    else if (!x.negative and y.negative)
        result += y;
    else if (x.negative and !y.negative)
        result = y - result;
}

/* exponentiate - compute x raise to the y power
 *   This is recursive. To compute x^y:
 *      -- If y is odd, return x * x^(y-1)
 *      -- If y is even, return x^y = x^(y/2) * x^(y/2)
 *      -- If y is 0 or 1, return 1 or x respectively.
 *   The recursion therefore terminates after about log_2(y) calls.
 * TODO: Do something when y < 0. Either throw an exception on negative exponents, or 
 *   return 1/x^(abs(y)) (which is zero for any x > 1 and abs(y)) > 1)
 */
void exponentiate(const Int& x, const Int& y, Int& result) { 
    if (y.is_int(0)) {
        result.set_int(1);
    } else if (y.is_int(1)) {
        result = x;
    } else if (y.is_odd()) {
        Int a;
        exponentiate(x, y - Int(1), a);
        result = x * a;
    } else {
        Int half_y;
        divide_by_int(y, 2, half_y);
        exponentiate(x, half_y, result);
        result = result * result;
    }
}

#endif
