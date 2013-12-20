#include "Int.hpp"

std::ostream& Int::print(std::ostream& out) const {
    if (negative)
        out << '-';
    size_t i = 0;
    int32_t x;
    while (i < bins.size() and (x = get_bin_from_back(i)) == 0)
        ++i;
    if (i == bins.size()) {
        out << (int32_t) 0;
    } else {
        out << get_bin_from_back(i);    // don't pad the most significant bin
        ++i;
        for (; i < bins.size(); ++i)
            out << std::setw(BIN_WIDTH) << std::setfill('0') << get_bin_from_back(i);
    }
    return out;
}

/* parse_int - read an positive int from the stringstream
 *   Made specifically for use with Int::read()
 *   This consumes no more than max_digits characters from the stream
 *      and only consumes digit characters [0-9].
 *
 *   This throws an invalid_argument exception if x contains anything other
 *   than an int. Use while constructing an Int from a string.
 */
int32_t parse_int(std::stringstream& ss, int max_digits) {
    std::stringstream s_num;
    int i = 0;
    while (i < max_digits && isdigit(ss.peek())) {
        s_num << (char) ss.get();
        ++i;
    }
    int32_t val;
    s_num >> val;
    if (s_num.fail() or !s_num.eof())
        throw std::invalid_argument("Failed to parse int from '" + s_num.str() + "'");
    return val;
}

/* Read an Int from the given stream into this object
 *   The Int is of the form: [-][0-9]+
 *   This destroys the contents of this Int instance used
 *
 * Note:
 *   This should not throw an exception. The call to parse_int is
 *   setup in a way that should not trigger the exception.
 */
std::istream& Int::read(std::istream& in) {
    // check whether the stream is in a good state
    std::istream::sentry s(in, true);
    if (!s)
        return in;

    if (in.peek() == '-') {
        in.get();
        if (isdigit(in.peek())) {
            negative = true;
        } else {
            in.unget();
            in.setstate(std::ios::failbit);
            return in;
        }
    } else {
        if (isdigit(in.peek())) {
            negative = false;
        } else {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    bins.clear();

    while (in.peek() == '0') {  // skip leading zeroes
        in.get();
    }

    // read all digits; we need to know the number of digits
    std::stringstream digit_ss;
    while (isdigit(in.peek()))
        digit_ss << (char) in.get();

    int num_digits = digit_ss.str().size();
    int count = 0;
    int k, n;
    if (num_digits == 0) {  // number is zero
        // in.get() sets the failbit on eof but we have not failed, so unset the failbit
        // (It will be re-set on later calls to get() on the stream)
        in.clear(in.rdstate() & ~std::ios::failbit);  // clear(flags) sets all the flags as given
        set_int(0);
        return in;
    } else if ((k = num_digits % BIN_WIDTH) > 0) {  // last bin size <= BIN_WIDTH
        n = parse_int(digit_ss, k);
        bins.push_front(n);
        count += k;
    }
    while (count < num_digits) {        // all other bin sizes == BIN_WIDTH
        n = parse_int(digit_ss, BIN_WIDTH);
        bins.push_front(n);
        count += BIN_WIDTH;
    }

    return in;
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
        Int tmp = -y;   // needed if x and y same instance
        x.negate();
        x += tmp;
        x.negate();
    } else if (x.negative and !y.negative) { // (-x) + y --> -(x - y)
        x.negate();
        x -= y;
        x.negate();
    } else if (!x.negative and y.negative) { // x + (-y) --> x - y
        x -= (-y);
    } else {
        x.add(Int(y));
    }
}

void operator-=(Int& x, const Int& y) {
    if (x.negative and y.negative) {         // -x - (-y) --> -(x - y)
        Int tmp = -y;
        x.negate();
        x -= tmp;
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
    x.divide(Int(y));  // copy y in case x and y are the same instance
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

/* Construct from a 64-bit int and apply a shift.
 *   This puts shift zero bins in *this, followed by the value of x.
 *   That is, after construction, *this will have the value x * (BIN_LIMIT ^ shift).
 *   (This is used in multiplication and division)
 */
Int::Int(int64_t x, size_t shift) {
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
        bins.push_back((int32_t) x);
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
 *   The string can be of the format: [-][0-9]+
 *
 * This throws an invalid_argument exception when unable to convert any part
 *   of x to an int (see parse_int)
 */
Int::Int(const std::string& x) : negative(false) {
    std::stringstream ss(x);
    this->read(ss);
    if (ss.fail() or !ss.eof()) {
        set_int(0);
        throw std::invalid_argument("Failed to parse Int from '" + x + "'");
    }
}

/* Int::is_int - return true if the value of *this is equal to x */
bool Int::is_int(int32_t x) const {
    int32_t val = abs(x);
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
void Int::set_int(int32_t val) {
    bins.clear();
    bins.push_back(abs(val));
    negative = (val < 0);
}

/* cmp_bins - compare to x, ignoring the signs
 * returns -1 if *this < x,
 *          1 if *this > x,
 *          0 if equal
 */
int32_t Int::cmp_bins(const Int& x) const {
    int32_t tmp;
    int32_t offset = x.bins.size() - bins.size();
    if (offset > 0) {  // x has more bins
        for (int32_t i = 0; i < offset; ++i)
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
int32_t Int::cmp(const Int& x) const {
    int32_t bin_cmp = cmp_bins(x);
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
    int32_t carry = 0;
    int32_t sum, val;
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
    int32_t diff;
    int32_t borrow = 0;
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

void Int::times_power_ten(int32_t power) {
    int32_t pow = power;
    if (pow > 0) {
        for (int32_t i = 0; i < pow % BIN_WIDTH; ++i)
            (*this) *= 10;
        while (pow >= BIN_WIDTH) {
            bins.push_front(0);
            pow -= BIN_WIDTH;
        }
    } else if (pow < 0) {
        pow = -pow;
        while (pow >= BIN_WIDTH and !bins.empty()) {
            bins.pop_front();
            pow -= BIN_WIDTH;
        }
        if (bins.empty()) {
            set_int(0);
        } else {
            for (int32_t i = 0; i < pow % BIN_WIDTH; ++i)
                (*this) /= 10;
        }
    }
}

/* multiply_by_int - multiply an Int by an int
 *   This ignores signs. Pass in abs(y) and compute the sign afterwards.
 */
void multiply_by_int(const Int& x, int32_t y, Int& result) {
    int64_t prod;
    result = Int(0);
    for (size_t i = 0; i < x.bins.size(); ++i) {
        prod = ((int64_t) y) * ((int64_t) x.bins[i]);
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
void divide_by_int(const Int& x, int32_t y, Int& result) {
    result.bins.clear();
    int64_t q, d;
    int64_t r = 0;
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
 *   This adds step to q and stops at the largest q such that x * q <= y.
 *   This assumes we'll have x and y so that 0 <= q < Int::BIN_LIMIT.
 *
 *   We know y >= x, and we want to find the quotient q and remainder r
 *   so that q * x + r = y. One way of doing so is to start with q = 0, and
 *   compute r = y - q*x for all q >= 0 until we find the an r less than x.
 *   This function allows us to iterate q by various step sizes repeatedly,
 *   so we can do essentially a binary search through possible values of q
 *   instead of a linear search.
 */
inline void iter_quotient(const Int& y, const Int& x, int32_t& q, Int& r, int32_t step) {
    int32_t b = q;
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
void quotient_and_remainder(const Int& y, const Int& x, int32_t& q, Int& r) {
    int32_t step = Int::BIN_LIMIT;
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
    int32_t q;
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
