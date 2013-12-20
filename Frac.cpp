#include "Frac.hpp"

std::ostream& operator<<(std::ostream& o, const Frac& x) {
    return x.print(o);
}

std::istream& operator>>(std::istream& i, Frac& x) {
    return x.read(i);
}

ostream& Frac::print(ostream& out) const {
    out << "(" << tt << "/" << bb << ")";
    return out;
}

// TODO
istream& Frac::read(istream& in) {
    return in;
}


/* Parse a Frac from a double.
 *   precision is the number of decimal digits to look at.
 *      By default, this is set to 15.
 *      Setting this too high may capture some floating point
 *      inaccuracy, giving an undesirable fraction. Setting this
 *      too low will not capture enough digits and give a fraction
 *      that does not correctly represent the original double.
 */
Frac Frac::from_double(double x, int precision) {
    /* convert to a string and parse from there */
    stringstream ss;
    ss.precision(precision);
    ss << fixed << x;
    string sx = ss.str();
    if (DEBUG_FRAC) cout << x << " : " << sx << endl;
    if (!isfinite(x))
        throw invalid_argument("cannot construct Frac from double '" + sx + "'");
    size_t k = sx.find('.');
    size_t num_frac_digits = sx.length() - k;
    sx.erase(sx.begin() + k);
    Int tt(sx);
    Int bb(1);
    bb.times_power_ten(num_frac_digits - 1);
    Frac result(tt, bb);
    if (DEBUG_FRAC) cout << "   --> " << result << endl;
    return result;
}

/* This always returns positive factors */
Int Frac::GCD(const Int& x, const Int& y) {
    Int absx = abs(x);
    Int absy = abs(y);
    if (absx < absy)
        return nochecks_gcd(absy, absx);
    else
        return nochecks_gcd(absx, absy);
}

/* Assumes a and b are nonnegative, and that a > b  */
Int Frac::nochecks_gcd(const Int& x, const Int& y) {
    Int a = x;
    Int b = y;
    Int r;
    while (!b.equals_int32(0)) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Parse a fraction of the basic form <integer>/<integer>
 * This throws an invalid_invalid argument on bad inputs.
 */
Frac::Frac(const string& x) {
    if (x.length() == 0) {
        tt = Int(0);
        bb = Int(1);
        return;
    }
    int j = x.find('/');
    if (j < 0) {
        tt = Int(x);
        bb = Int(1);
    } else {
        tt = Int(x.substr(0, j));
        bb = Int(x.substr(j + 1));
    }
    if (bb.equals_int32(0))
        throw invalid_argument("poorly formatted input string: " + x);
    normalize();
}

void Frac::normalize() {
    /* tt will store the sign, bb is always positive */
    if (bb.is_negative()) {
        bb.negate();
        tt.negate();
    }
    Int c = Frac::GCD(tt, bb);
    tt /= c;
    bb /= c;
}

bool operator==(const Frac& x, const Frac& y) { return x.tt == y.tt and x.bb == y.bb; }
bool operator!=(const Frac& x, const Frac& y) { return !(x == y); }
bool operator< (const Frac& x, const Frac& y) { return x.tt * y.bb < y.tt * x.bb; }
bool operator> (const Frac& x, const Frac& y) { return y < x; }
bool operator<=(const Frac& x, const Frac& y) { return !(y < x); }
bool operator>=(const Frac& x, const Frac& y) { return !(x < y); }

void operator+=(Frac& x, const Frac& y) {
    x.tt = x.tt * y.bb + y.tt * x.bb;
    x.bb *= y.bb;
    x.normalize();
}

void operator-=(Frac& x, const Frac& y) {
    x.tt = x.tt * y.bb - y.tt * x.bb;
    x.bb *= y.bb;
    x.normalize();
}

void operator*=(Frac& x, const Frac& y) {
    x.tt *= y.tt;
    x.bb *= y.bb;
    x.normalize();
}

void operator/=(Frac& x, const Frac& y) {
    Int tmp = y.tt; /* in case x and y are the same instance */
    x.tt *= y.bb;
    x.bb *= tmp;
    x.normalize();
}

Frac operator^(const Frac& x, const Int& a) {
    if (a.is_negative()) {
        Int neg_a = -a;
        return Frac(x.bb ^ neg_a, x.tt ^ neg_a);
    } else {
        return Frac(x.tt ^ a, x.bb ^ a);
    }
}

void operator+=(Frac& x, const Int& a) {
    x.tt += x.bb * a;
    x.normalize();
}

void operator-=(Frac& x, const Int& a) {
    x.tt -= x.bb * a;
    x.normalize();
}

void operator*=(Frac& x, const Int& a) {
    x.tt *= a;
    x.normalize();
}

void operator/=(Frac& x, const Int& a) {
    x.bb *= a;
    x.normalize();
}

void operator^=(Frac& x, const Int& a) {
    if (a.is_negative()) {
        Int neg_a = -a;
        x.bb ^= neg_a;
        x.tt ^= neg_a;
    } else {
        x.bb ^= a;
        x.tt ^= a;
    }
    x.normalize();
}
