#include "Int.hpp"

// For debugging.
std::ostream& operator<<(std::ostream& o, const std::deque<int32_t>& deque) {
    std::deque<int32_t>::const_iterator i;
    o << "Deque[";
    for (i = deque.begin(); i < deque.end(); ++i) {
        o << *i;
        if (i < --deque.end()) {
            o << ", ";
        }
    }
    return o << "]";
}

std::ostream& operator<<(std::ostream& o, const Int& x) {
    return x.print(o);
}

std::istream& operator>>(std::istream& i, Int& x) {
    return x.read(i);
}

std::ostream& Int::print(std::ostream& out) const {
    if (negative)
        out << '-';
    size_t i = 0;
    int32_t x;
    while (i < bins.size() && (x = get_bin_from_back(i)) == 0)
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
    if (s_num.fail() || !s_num.eof())
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
        set_value(0);
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
    if (x.negative && y.negative)        // (-x) + (-y) --> -(x + y)
        return -(-x + (-y));
    else if (x.negative && !y.negative)  // (-x) + y --> -(x - y)
        return -(-x - y);
    else if (!x.negative && y.negative)  // x + (-y) --> x - y
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
    if (x.negative && y.negative)        // -x - (-y) --> -(x - y)
        return -(-x - (-y));
    else if (x.negative && !y.negative)  // -x - y --> -(x + y)
        return -(-x + y);
    else if (!x.negative && y.negative)  // x - (-y) --> x + y
        return x + (-y);

    if (x > y) {
        Int r(x);
        r.cleanBins();
        r.subtract(y);
        return r;
    } else {
        Int r(y);
        r.cleanBins();
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

void Int::operator+=(const Int& other) {
    if (negative && other.negative) {         // (-x) + (-y) --> -(x + y)
        Int tmp = -other;   // needed if x and y same instance
        negate();
        (*this) += tmp;
        negate();
    } else if (negative && !other.negative) { // (-x) + y --> -(x - y)
        negate();
        (*this) -= other;
        negate();
    } else if (!negative && other.negative) { // x + (-y) --> x - y
        (*this) -= (-other);
    } else {
        add(Int(other));  // copy in case same instance
    }
}

void Int::operator-=(const Int& other) {
    if (negative && other.negative) {         // -x - (-y) --> -(x - y)
        Int tmp = -other;
        negate();
        (*this) -= tmp;
        negate();
    } else if (negative && !other.negative) { // -x - y --> -(x + y)
        negate();
        (*this) += other;
        negate();
    } else if (!negative && other.negative) { // x - (-y) --> x + y
        (*this) += (-other);
    } else {
        if (*this > other) {
            subtract(other);
        } else {
            Int r(other);
            r.subtract(*this);
            r.negate();
            return std::swap(*this, r);
        }
    }
}

void Int::operator*=(const Int& other) {
    Int r;
    multiply(*this, other, r);
    r.negative = (negative != other.negative);
    std::swap(*this, r);
}

void Int::operator/=(const Int& other) {
    divide(Int(other));  // copy y in case x and y are the same instance
}

void Int::operator%=(const Int& other) {
    Int r;
    modulo(*this, other, r);
    std::swap(*this, r);
}

void Int::operator^=(const Int& other) {
    Int r;
    exponentiate(*this, other, r);
    std::swap(*this, r);
}

Int operator-(const Int& x) {
    Int y(x);
    y.negate();
    return y;
}

/* Default constructor - zero is the default value
 * bins(1, 0) says size = 1 and value = 0.
 */
Int::Int() : bins(1, 0), negative(false) {
}

/* Construct from a 64-bit int and apply a shift.
 *   This puts shift zero bins in *this, followed by the value of x.
 *   That is, after construction, *this will have the value x * (BIN_LIMIT ^ shift).
 *   (This is used in multiplication and division)
 */
Int::Int(int64_t x, size_t shift) : bins(shift, 0), negative(false) {
    if (x < 0) {
        negative = true;
        x = -x;
    } else if (x == 0) {
        set_value(0);
        return;
    }
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
Int::Int(const Int& x, size_t shift) : bins(shift, 0), negative(x.negative) {
    for (size_t i = 0; i < x.bins.size(); ++i)
		bins.push_back(x.bins.at(i));
}

/* Construct an Int from a std::string.
 *   The string can be of the format: [-][0-9]+
 *
 * This throws an invalid_argument exception when unable to convert any part
 *   of x to an int (see parse_int)
 */
Int::Int(const std::string& x) : negative(false) {
    std::stringstream ss(x);
    read(ss);
    if (ss.fail() || !ss.eof()) {
        set_value(0);
        throw std::invalid_argument("Failed to parse Int from '" + x + "'");
    }
}

/* Int::is_int - return true if the value of *this is equal to x */
bool Int::equals_int32(int32_t x) const {
    int32_t val = abs(x);
    size_t i = 0;
    for (; i < bins.size() - 1; ++i)
        if (bins.at(i))
            return false;
	if (bins.at(i) == val && negative == (x < 0))
        return true;
	else if (bins.at(i) == val && val == 0)
        return true;
    return false;
}

/* Int::set_int - set the value of *this to val with correct sign */
void Int::set_value(int32_t val) {
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
    if (offset < 0) {
        return -x.cmp_bins(*this);
    } else if (offset > 0) {  // x has more bins
        for (int32_t i = 0; i < offset; ++i)
            if (x.get_bin_from_back(i) != 0)
                return -1;
    }
    for (size_t i = 0; i < bins.size(); ++i)
        if ((tmp = cmp_ints(get_bin_from_back(i), x.get_bin_from_back(offset + i))))
            return tmp;
    return 0;
}

void Int::cleanBins() {
    if (DEBUG_INT_DIVIDE) std::cout << "Cleaning bins:" << std::endl << "  before: " << bins << std::endl;
    while (bins.size() > 1 && bins.back() == 0) {
        bins.pop_back();
    }
    if (DEBUG_INT_DIVIDE) std::cout << "  after:  " << bins << std::endl;
}

/* cmp - compare *this with x
 * returns -1 if *this < x,
 *          1 if *this > x,
 *          0 if equal
 */
int32_t Int::cmp(const Int& x) const {
    int32_t bin_cmp = cmp_bins(x);
    if (bin_cmp == 0 && equals_int32(0)) {
        return 0;
    } else {
        if (negative && x.negative)
            return -bin_cmp;
        else if (!negative && !x.negative)
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
		sum = bins.at(i) + x.bins.at(i) + carry;
        val = sum % BIN_LIMIT;
        carry = max(0, sum / BIN_LIMIT);
		bins.at(i) = val;
    }
    while (carry != 0 && i < bins.size()) {
		sum = bins.at(i) + carry;
        val = sum % BIN_LIMIT;
        carry = max(0, sum / BIN_LIMIT);
		bins.at(i) = val;
        ++i;
    }
    if (carry != 0)
        bins.push_back(carry);
}

/* Int::subtract - subtract off x from *this
 *   This ignores signs and assumes that *this is larger than or equal to x in absolute value.
 */
void Int::subtract(const Int& x) {
    int32_t diff;
    int32_t borrow = 0;
    size_t i = 0;
    if (DEBUG_INT_SUBTRACT) std::cout << "Subtracting: " << std::endl
              << "    " << *this << std::endl
              << "  - " << x << std::endl
              << "  bins = " << bins << std::endl
              << "  x.bins = " << x.bins << std::endl
              << "  cmp_bins(x) = " << cmp_bins(x) << std::endl
              << "  bins.size() = " << bins.size() << std::endl
              << "  x.bins.size() = " << x.bins.size() << std::endl;
    if (bins.size() > 1) assert(bins.back() != 0);
    if (x.bins.size() > 1) assert(x.bins.back() != 0);
    assert(abs(*this) >= abs(x));
    for (; i < x.bins.size(); ++i) {
        if (DEBUG_INT_SUBTRACT) std::cout << " i = " << i << "(subtract)" << std::endl;
        if (DEBUG_INT_SUBTRACT) std::cout << "  *this = " << *this << std::endl;
		diff = bins.at(i) - borrow - x.bins.at(i);
        if (DEBUG_INT_SUBTRACT) std::cout << "  borrow = " << borrow << std::endl;
        if (DEBUG_INT_SUBTRACT) std::cout << "  diff = " << diff << std::endl;
        if (diff < 0) {
            borrow = 1;
            diff += BIN_LIMIT;
        } else {
            borrow = 0;
        }
        if (DEBUG_INT_SUBTRACT) std::cout << "  borrow = " << borrow << std::endl;
        if (DEBUG_INT_SUBTRACT) std::cout << "  diff = " << diff << std::endl;
        bins.at(i) = diff;
    }
    while (i < bins.size() && borrow != 0) {
        diff = bins.at(i) - borrow;
        if (diff < 0) {
            borrow = 1;
            diff += BIN_LIMIT;
        } else {
            borrow = 0;
        }
        bins.at(i) = diff;
        ++i;
    }
    cleanBins();
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
        while (pow >= BIN_WIDTH && !bins.empty()) {
            bins.pop_front();
            pow -= BIN_WIDTH;
        }
        if (bins.empty()) {
            set_value(0);
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
    result.set_value(0);
    for (size_t i = 0; i < x.bins.size(); ++i) {
        prod = ((int64_t) y) * ((int64_t) x.bins.at(i));
        result += Int(prod, i);
    }
}

/* multiply - multiply two Ints together
 *   This ignores signs and assumes x has at least as many bins as y
 */
void multiply(const Int& x, const Int& y, Int& result) {
    Int b;
    result.set_value(0);
    for (size_t i = 0; i < y.bins.size(); ++i) {
        multiply_by_int(x, y.bins.at(i), b);
        result += Int(b, i);
    }
}

/* divide_by_int - divide and Int by an int
 *   This ignores signs. Pass in abs(y) and compute the sign aftwerwards.
 */
void divide_by_int(const Int& x, int32_t y, Int& result) {
    result.bins.clear();
    if (y == 0)
        throw divide_by_zero_error();
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
void iter_quotient(const Int& y, const Int& x, int32_t& q, Int& r, int32_t step) {
    int32_t b = q;
    Int prod;
    if (DEBUG_INT_DIVIDE) {
        std::cout << "iter_quotient(y=" << y << ", x=" << x << ", q=" << q << ", r=" << r << ", step=" << step << ")" << std::endl;
        std::cout << "  y.bins = " << y.bins << std::endl;
        std::cout << "  x.bins = " << x.bins << std::endl;
    }
    do {
        q = b;
        b += step;
        if (DEBUG_INT_DIVIDE) std::cout << "  b = " << b << std::endl;
        multiply_by_int(x, b, prod);
        if (DEBUG_INT_DIVIDE) std::cout << "  prod = " << prod << std::endl;
        if (DEBUG_INT_DIVIDE) std::cout << "  prod.bins = " << prod.bins << std::endl;
        r = y - prod;
        if (DEBUG_INT_DIVIDE) std::cout << "  r = " << r << std::endl;
    } while (!r.negative && x.cmp_bins(r) <= 0);
}

/* quotient_and_remainder - find q and r so that q * x + r = y.
 *   This assumes we'll have y and x so that x <= y < BIN_LIMIT * x,
 *   so we know after returning that q >= 1.
 *
 * -- Right now, iter_quotient (with step /= 2 before each call) checks
 *  (q + step) and (q + 2*step) which is unnecessary:
 *  We check:
 *      q = 50 000 000 and possibly q = 1 000 000 000
 *      q = 25 000 000 and possibly q = 50 000 000
 *      q = 12 500 000 and possibly q = 25 000 000
 *      ...
 * We're binary searching here over the values of q in [1, 1 000 000 000).
 * We stop when r = y - q*x is in [0, x).
 *      1. Check the midpoint q' = 50 000 000
 *          -- If the remainder r = y - q'*x >= 0, then q' <= q (branch right)
 *              We may have the correct value of q, or we may need to increase q to
 *              reduce r to be less than x.
 *          -- Otherwise, the remainder r = y - q'*x < 0, the q' > q (branch left)
 *              r is negative, so q is too big. We need to decrease q to get r >= 0.
 */
void quotient_and_remainder(const Int& y, const Int& x, int32_t& q, Int& r) {
    if (DEBUG_INT_DIVIDE) std::cout << "quotient_and_remainder(" << y << ", " << x << ")" << std::endl;
    if (DEBUG_INT_DIVIDE) std::cout << "  y.bins = " << y.bins << std::endl;
    if (DEBUG_INT_DIVIDE) std::cout << "  x.bins = " << x.bins << std::endl;
    assert(!x.is_negative());
    assert(!y.is_negative());
    assert(x <= y && y < Int::BIN_LIMIT * x);

    int32_t step = Int::BIN_LIMIT;
    q = 0;
    while (step > 1) {
        step /= 2;
        iter_quotient(y, x, q, r, step);
    }
    q += 1; // what is this for?
}

/* Int::divide - do an in-place division of two Ints
 *   This handles the sign, and throws a divide_by_zero_error when x is zero.
 */
void Int::divide(const Int& x) {
    if (x.equals_int32(0)) {
        throw divide_by_zero_error();
    } else if (cmp_bins(x) < 0) {  // division by a larger number truncates
        set_value(0);
        return;
    }


//    bool result_is_neg = (negative != x.negative);
    const Int& x_norm = (x.negative) ? -x : x;


    Int r, current;
    int32_t q;
    for (int32_t i = ((int32_t) bins.size()) - 1; i >= 0; --i) {
        if (DEBUG_INT_DIVIDE) std::cout << "i = " << i << "(divide)" << std::endl
                  << "  *this = " << *this << std::endl
                  << "  bins.size() = " << bins.size() << std::endl
                  << "  bins = " << bins << std::endl
                  << "  x_norm = " << x_norm << std::endl;
        assert(current.bins.size() > 0);
        assert(0 <= i && ((size_t) i) < bins.size());
        current.bins.at(0) = bins.at(i);
        if (DEBUG_INT_DIVIDE) std::cout << "  current = " << current << std::endl;
        if (x_norm.cmp_bins(current) <= 0) {    // if x_norm divides current (with some remainder)
            quotient_and_remainder(current, x_norm, q, r);
            if (DEBUG_INT_DIVIDE) std::cout << "  setting index " << i << " = " << q << std::endl;
            assert(q != 0);
            bins.at(i) = q;
            current = r;
        } else {
            if (DEBUG_INT_DIVIDE) std::cout << "  setting index " << i << " = ZERO" << std::endl;
            bins.at(i) = 0;
            cleanBins();
        }
        current.shift(1);
        current.cleanBins();
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

    if (DEBUG_INT_MODULO) {
        std::cout << "Int::modulo(x, y)" << std::endl
                  << "  a = " << a << std::endl
                  << "  b = " << b << std::endl;
    }
    Int q = a / b;
    if (DEBUG_INT_MODULO) std::cout << "  q = " << q << std::endl;
    result = a - q * b;
    if (DEBUG_INT_MODULO) std::cout << "  result = " << result << std::endl;
    if (result.equals_int32(0))
        return;
    if (x.negative && y.negative)
        result.negate();
    else if (!x.negative && y.negative)
        result += y;
    else if (x.negative && !y.negative)
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
    if (y.equals_int32(0)) {
        result.set_value(1);
    } else if (y.equals_int32(1)) {
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
