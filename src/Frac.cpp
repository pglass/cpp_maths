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

/* This reads in a fraction of the form '(INT/INT)' or 'INT/INT'.
 * This expects no whitespace.
 */
istream& Frac::read(istream& in) {
    // check whether the stream is in a good state
    std::istream::sentry s(in, true);
    if (!s)
        return in;

    //
    bool parens = false;
    bool success = true;
    if (in.peek() == '(') {
        parens = true;
        in.get();
    }
    if (in >> tt) {
        if (in.peek() == '/') {
            in.get();
            if (in >> bb) {
            } else {
                success = false;
            }
        } else {
            bb.set_value(1);
        }
    } else {
        success = false;
        if (parens)
            in.unget();
    }
    // only match a close parens if we matched an opening parens
    if (success && parens && in.peek() == ')') {
        in.get();
    }

    if (success) {
        // in.get() sets the failbit on eof but we have not failed, so unset the failbit
        // (It will be re-set on later calls to get() on the stream)
        in.clear(in.rdstate() & ~std::ios::failbit);  // clear(flags) sets all the flags as given
    }

    normalize();
    return in;
}


/* Parse a Frac from a double.
 *   precision is the number of decimal digits to look at.
 *      By default, this is set to 15.
 *      Setting this too high may capture some floating point
 *      inaccuracy, giving an undesirable fraction. Setting this
 *      too low will not capture enough digits and give a fraction
 *      that does not correctly represent the original double.
 * WARNING: This is experimental.
 */
Frac Frac::from_double(double x, int precision) {
    /* convert to a string and parse from there */
    if (DEBUG_FRAC) cout << "from_double(x=" << x << ", " << precision << ")" << endl;
    stringstream ss;
    ss.precision(precision);
    ss << fixed << x;
    string sx = ss.str();
    if (!isFinite(x))
        throw invalid_argument("cannot construct Frac from double '" + sx + "'");
    size_t k = sx.find('.');
    size_t num_frac_digits = sx.length() - k;
    if (DEBUG_FRAC) {
        cout << "  sx = " << sx << endl;
        cout << "  k = " << k << endl;
        cout << "  num_frac_digits = " << num_frac_digits << endl;
    }
    sx.erase(sx.begin() + k);
    if (DEBUG_FRAC) cout << "  sx = " << sx << endl;
    Int tt(sx);
    if (DEBUG_FRAC) cout << "  tt = " << tt << endl;
    Int bb(1);
    bb.times_power_ten(num_frac_digits - 1);
    if (DEBUG_FRAC) cout << "  bb = " << bb << endl;
    Frac result(tt, bb);
    if (DEBUG_FRAC) cout << "  result = " << result << endl;
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
    if (DEBUG_FRAC) {
        cout << "nochecks_gcd(x=" << x << ", y=" << y << ")" << endl;
    }
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

/* Parse a fraction of the basic form INT/INT
 * This throws an invalid_invalid argument on bad inputs.
 */
Frac::Frac(const string& x) : tt(0), bb(1) {
    if (x.length() == 0) {
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
    if (DEBUG_FRAC) cout << "Frac::normalize()" << endl;
    if (bb.is_negative()) {
        bb.negate();
        tt.negate();
    }
    Int c = Frac::GCD(tt, bb);
    if (DEBUG_FRAC) cout << "GCD = " << c << endl;
    tt /= c;
    bb /= c;
}

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
