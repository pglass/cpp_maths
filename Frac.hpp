#ifndef _FRAC_HPP_
#define _FRAC_HPP_
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>
#include "Int.hpp"

using namespace std;

const bool DEBUG_FRAC = false;

/* Frac - represents a fraction tt/bb
 *   The fraction should always be fully reduced.
 *   The numerator tt stores the sign (tt.is_negative()).
 *   The denominator bb is always positive.
 */
class Frac {
  public:
    Frac() : tt(0), bb(1) { }
    explicit Frac(int x) : tt(x), bb(1) { }
    explicit Frac(const Int& x) : tt(x), bb(1) { }
    explicit Frac(const string& x);
    Frac(const Int& top, const Int& bottom) : tt(top), bb(bottom) { 
        if (bb.equals_int32(0))
            throw divide_by_zero_error();
        normalize();
    }

    static Int GCD(const Int& a, const Int& b); /* move this somewhere else? */
    static Frac from_double(double x, int precision = 15); 

    Frac reciprocal() { return Frac(bb, tt); }

    friend bool operator == (const Frac& x, const Frac& y);
    friend bool operator != (const Frac& x, const Frac& y);
    friend bool operator <  (const Frac& x, const Frac& y);
    friend bool operator <= (const Frac& x, const Frac& y);
    friend bool operator >  (const Frac& x, const Frac& y);
    friend bool operator >= (const Frac& x, const Frac& y);
    
    friend Frac operator + (const Frac& x, const Frac& y) { return Frac(x.tt * y.bb + y.tt * x.bb, x.bb * y.bb); }
    friend Frac operator - (const Frac& x, const Frac& y) { return Frac(x.tt * y.bb - y.tt * x.bb, x.bb * y.bb); }
    friend Frac operator * (const Frac& x, const Frac& y) { return Frac(x.tt * y.tt, x.bb * y.bb); }
    friend Frac operator / (const Frac& x, const Frac& y) { return Frac(x.tt * y.bb, x.bb * y.tt); }
    
    friend Frac operator + (const Int& a, const Frac& x) { return Frac(a * x.bb + x.tt, x.bb); }
    friend Frac operator - (const Int& a, const Frac& x) { return Frac(a * x.bb - x.tt, x.bb); }
    friend Frac operator * (const Int& a, const Frac& x) { return Frac(a * x.tt, x.bb); }
    friend Frac operator / (const Int& a, const Frac& x) { return Frac(a * x.bb, x.tt); }

    friend Frac operator + (const Frac& x, const Int& a) { return Frac(x.tt + a * x.bb, x.bb); }
    friend Frac operator - (const Frac& x, const Int& a) { return Frac(x.tt - a * x.bb, x.bb); }
    friend Frac operator * (const Frac& x, const Int& a) { return Frac(a * x.tt, x.bb); }
    friend Frac operator / (const Frac& x, const Int& a) { return Frac(x.tt, x.bb * a); }
    friend Frac operator ^ (const Frac& x, const Int& a); 

    friend void operator += (Frac& x, const Frac& y);
    friend void operator *= (Frac& x, const Frac& y);
    friend void operator -= (Frac& x, const Frac& y);
    friend void operator /= (Frac& x, const Frac& y);

    friend void operator += (Frac& x, const Int& a); 
    friend void operator -= (Frac& x, const Int& a); 
    friend void operator *= (Frac& x, const Int& a); 
    friend void operator /= (Frac& x, const Int& a); 
    friend void operator ^= (Frac& x, const Int& a); 

    friend std::ostream& operator<<(std::ostream& o, const Frac& x);
    friend std::istream& operator>>(std::istream& i, Frac& x);
    std::ostream& print(std::ostream& out) const;
    std::istream& read(std::istream& in);
  private:
    Int tt, bb; /* top, bottom */
    void normalize(); 

    static Int nochecks_gcd(const Int& a, const Int& b);
};

#endif
