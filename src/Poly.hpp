#ifndef POLY_HPP
#define POLY_HPP
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include "common.hpp"
#include "Frac.hpp"
#include "Int.hpp"

using namespace std;

/* Factor represents x^n for an integer n */
struct Factor {
    Factor(const string& _id, const Int& _power) : id(_id), power(_power) {}
    string id;
    Int power;

    inline string getId() const { return id; }

    friend bool operator <  (const Factor& x, const Factor& y);
    friend bool operator == (const Factor& x, const Factor& y);
    friend bool operator != (const Factor& x, const Factor& y) { return !(x == y); }
};

ostream& operator<<(ostream& out, const Factor& x) {
    if (x.power.equals_int32(1))
        return out << x.id;
    else
        return out << x.id << "^" << x.power;
}

bool operator<(const Factor& x, const Factor& y) {
    if (x.id < y.id)
        return true;
    else if (x.id == y.id and x.power < y.power)
        return true;
    return false;
}

bool operator==(const Factor& x, const Factor& y) {
    return (x.id == y.id and x.power == y.power);
}

static const Frac FRAC_ZERO = Frac(0);
static const Frac FRAC_ONE = Frac(1);

/* Term represents c * x1^n1 * x2^n2 * x3^n3 * ... */
struct Term {
    typedef map<string, Factor>::iterator       map_iter_t;
    typedef map<string, Factor>::const_iterator map_citer_t;

    explicit Term(const Factor& x) : coeff(1) { appendFactor(x); }
    Term(const Frac& c, const Factor& x) : coeff(c) { appendFactor(x); }
    Term(const Frac& c, const vector<Factor>& _factors);

    inline void negate() { coeff *= -1; }
    void appendFactor(const Factor& x);

    friend Term operator * (const Factor& x, const Factor& y);
    friend Term operator * (const Term& x,   const Term& y);
    friend Term operator * (const Term& x,   const Factor& y);
    friend Term operator * (const Factor& x, const Term& y) { return y * x; }

    friend void operator *= (Term& x, const Factor& y);
    friend void operator *= (Term& x, const Term& y);

    friend bool operator <  (const Term& x, const Term& y);
    friend bool operator == (const Term& x, const Term& y);

    Frac coeff;
    map<string, Factor> factors;
};

Term::Term(const Frac& c, const vector<Factor>& _factors) : coeff(c) {
    vector<Factor>::const_iterator it;
    for (it = _factors.begin(); it != _factors.end(); ++it)
        appendFactor(*it);
}

/* Multiply this Term by the Factor x */
void Term::appendFactor(const Factor& x) {
    Term::map_iter_t it = factors.find(x.getId());
    if (it == factors.end()) {
        // I don't use operator[] because it requires a default constructor on Factor
        // (which I don't provide. A Factor does not have a logical default value)
        factors.insert(map<string, Factor>::value_type(x.getId(), x));
    } else {
        it->second.power += x.power;
    }
}

ostream& operator<<(ostream& out, const Term& x) {
    Term::map_citer_t it;
    Term::map_citer_t end_it;
    if (x.coeff == FRAC_ZERO) {
        out << "0";
    } else if (x.factors.size() == 0) {
        if (x.coeff == FRAC_ONE)
            out << "1";
        else
            out << x.coeff;
    } else if (x.coeff == FRAC_ONE) {
        it = x.factors.begin();
        out << it->second;
        for (it++; it != x.factors.end(); ++it)
            out << " * " << it->second;
    } else {
        out << x.coeff;
        for (it = x.factors.begin(); it != x.factors.end(); ++it)
            out << " * " << it->second;
    }
    return out;
}

/* Compare terms based on the Factors only. This ignores the coefficient.
 * This will sort like:
 *      x < x^2 < x * y < x^2 * y < x * y^2
 */
bool operator<(const Term& x, const Term& y) {
    Term::map_citer_t xi;
    Term::map_citer_t yi;

    if (x.factors.size() < y.factors.size()) {
        return true;
    } else {
        xi = x.factors.begin();
        yi = y.factors.begin();
        for (; xi != x.factors.end() and yi != y.factors.end(); ++xi, ++yi) {
            if (xi->second < yi->second)
                return true;
        }
    }
    return false;
}

/* Comparison is done only on the Factors. This does not compare the coefficients */
bool operator==(const Term& x, const Term& y) {
    Term::map_citer_t xi, yi;
    if (x.factors.size() != y.factors.size())
        return false;
    xi = x.factors.begin();
    yi = y.factors.begin();
    for (; xi != x.factors.end() and yi != y.factors.end(); ++xi, ++yi)
        if (xi->second != yi->second)
            return false;
    return true;
}

Term operator*(const Factor& x, const Factor& y) {
    Term term(x);
    term.appendFactor(y);
    return term;
}

Term operator*(const Term& x, const Factor& y) {
    Term term(x);
    term.appendFactor(y);
    return term;
}

Term operator*(const Term& x, const Term& y) {
    Term term(x);
    Term::map_citer_t it;
    for (it = y.factors.begin(); it != y.factors.end(); ++it)
        term.appendFactor(it->second);
    term.coeff *= y.coeff;
    return term;
}

void operator*=(Term& x, const Factor& y) {
    x.appendFactor(y);
}

void operator*=(Term& x, const Term& y) {
    Term::map_citer_t it;
    for (it = y.factors.begin(); it != y.factors.end(); ++it)
        x.appendFactor(it->second);
    x.coeff *= y.coeff;
}

/* Poly represents a polynomial with integer exponents */
struct Poly {
    typedef list<Term>::iterator       list_iter_t;
    typedef list<Term>::const_iterator list_citer_t;

    explicit Poly(const Factor& x) { terms.push_back(Term(x)); }
    explicit Poly(const Term& x) { terms.push_back(x); }
    explicit Poly(const vector<Factor>& _factors);
    explicit Poly(const vector<Term>& _terms);

    void appendTerm(const Term& x);
    void appendTerms(const list<Term>& x);
    inline size_t numTerms() { return terms.size(); }

    friend Poly operator +  (const Factor& x, const Factor& y);
    friend Poly operator -  (const Factor& x, const Factor& y);
    friend Poly operator +  (const Term& x, const Term& y);
    friend Poly operator -  (const Term& x, const Term& y);
    friend void operator += (Poly& p, const Factor& y);
    friend void operator -= (Poly& p, const Factor& y);

    list<Term> terms;
};


Poly::Poly(const vector<Factor>& _factors) {
    for (size_t i = 0; i < _factors.size(); ++i)
        appendTerm(Term(_factors[i]));
}

Poly::Poly(const vector<Term>& _terms) {
    for (size_t i = 0; i < _terms.size(); ++i)
        appendTerm(_terms[i]);
}

ostream& operator<<(ostream& out, const Poly& p) {
    Poly::list_citer_t it;
    if (p.terms.size() > 0) {
        it = p.terms.begin();
        out << *it;
        ++it;
        for (; it != p.terms.end(); ++it)
            out << " + " << *it;
    }
    return out;
}

void Poly::appendTerm(const Term& x) {
    Poly::list_iter_t it;
    for (it = terms.begin(); it != terms.end() and *it < x; ++it)
        ;  // iterate until *it >= x
    if (*it == x)
        it->coeff += x.coeff;
    else
        terms.insert(it, x);
}

void Poly::appendTerms(const list<Term>& x) {
//    terms.merge(list<Term>(x));  // merge two sorted lists
    list<Term> copy(x);
    terms.merge(copy);
}

Poly operator+(const Factor& x, const Factor& y) {
    Term tx(x);
    Poly p(tx);
    p.appendTerm(Term(y));
    return p;
}

Poly operator+(const Term& x, const Term& y) {
    Poly p(x);
    p.appendTerm(y);
    return p;
}

void operator+=(Poly& p, const Factor& y) {
    p.appendTerm(Term(y));
}

Poly operator-(const Factor& x, const Factor& y) {
    Poly p(x);
    p.appendTerm(Term(Frac(-1), y));
    return p;
}

Poly operator-(const Term& x, const Term& y) {
    Poly p(x);
    Term ty(y);
    ty.negate();
    p.appendTerm(ty);
    return p;
}

void operator-=(Poly& p, const Factor& y) {
    p.appendTerm(Term(Frac(-1), y));
}

#endif // POLY_HPP
