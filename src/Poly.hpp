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
class Factor {
  public:
    Factor(const string& _id, const Int& _power) : id(_id), power(_power) {}
    inline string& getId() { return id; }
    inline const std::string& getId() const { return id; }
    inline Int& getPower() { return power; }
    inline const Int& getPower() const { return power; }
    friend bool operator <  (const Factor& x, const Factor& y);
    friend bool operator == (const Factor& x, const Factor& y);
    friend bool operator != (const Factor& x, const Factor& y) { return !(x == y); }
    friend std::ostream& operator<<(std::ostream& o, const Factor& factor);
  private:
    string id;
    Int power;
};

ostream& operator<<(ostream& o, const Factor& factor) {
    if (factor.power.equals_int32(1))
        return o << factor.id;
    else
        return o << factor.id << "^" << factor.power;
}

bool operator<(const Factor& x, const Factor& y) {
    if (x.id < y.id)
        return true;
    else if (x.id == y.id && x.power < y.power)
        return true;
    return false;
}

bool operator==(const Factor& x, const Factor& y) {
    return (x.id == y.id && x.power == y.power);
}

static const Frac FRAC_ZERO = Frac(0);
static const Frac FRAC_ONE = Frac(1);

/* Term represents c * x1^n1 * x2^n2 * x3^n3 * ... */
class Term {
  public:
    typedef map<string, Factor>::iterator       map_iter_t;
    typedef map<string, Factor>::const_iterator map_citer_t;

    explicit Term(const Factor& x) : coeff(1) { appendFactor(x); }
    Term(const Frac& c, const Factor& x) : coeff(c) { appendFactor(x); }
    Term(const Frac& c, const vector<Factor>& _factors);

    inline const Frac& coefficient() const { return coeff; }
    inline Frac& coefficient() { return coeff; }
    inline void negate() { coeff *= -1; }
    void appendFactor(const Factor& x);

    friend Term operator * (const Factor& x, const Factor& y);
    friend Term operator * (const Term& x,   const Term& y);
    friend Term operator * (const Term& x,   const Factor& y);
    inline friend Term operator * (const Factor& x, const Term& y) { return y * x; }

    friend void operator *= (Term& x, const Factor& y);
    friend void operator *= (Term& x, const Term& y);

    friend bool operator <  (const Term& x, const Term& y);
    friend bool operator == (const Term& x, const Term& y);

    friend std::ostream& operator<<(std::ostream& o, const Term& term);
  private:
    Frac coeff;
    map<string, Factor> factors;
};

Term::Term(const Frac& c, const vector<Factor>& _factors) : coeff(c) {
    vector<Factor>::const_iterator it;
    for (it = _factors.begin(); it != _factors.end(); ++it)
        appendFactor(*it);
}

/* appending a factor to a Term is a multiplication by the Factor */
void Term::appendFactor(const Factor& x) {
    Term::map_iter_t it = factors.find(x.getId());
    if (it == factors.end()) {
        // Don't use operator[] -- it requires a default constructor on Factor
        // which doesn't exist. A Factor does not have a logical default value.
        factors.insert(map<string, Factor>::value_type(x.getId(), x));
    } else {
        it->second.getPower() += x.getPower();
    }
}

ostream& operator<<(ostream& o, const Term& term) {
    Term::map_citer_t it;
    if (term.coeff == FRAC_ZERO) {
        o << "0";
    } else if (term.factors.size() == 0) {
        if (term.coeff == FRAC_ONE)
            o << "1";
        else
            o << term.coeff;
    } else if (term.coeff == FRAC_ONE) {
        it = term.factors.begin();
        o << it->second;
        for (it++; it != term.factors.end(); ++it)
            o << " * " << it->second;
    } else {
        o << term.coeff;
        for (it = term.factors.begin(); it != term.factors.end(); ++it)
            o << " * " << it->second;
    }
    return o;
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
        for (; xi != x.factors.end() && yi != y.factors.end(); ++xi, ++yi) {
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
    for (; xi != x.factors.end() && yi != y.factors.end(); ++xi, ++yi)
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
    term.coefficient() *= y.coeff;
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
class Poly {
  public:
    typedef list<Term>::iterator       list_iter_t;
    typedef list<Term>::const_iterator list_citer_t;

    explicit Poly(const Factor& x) { terms.push_back(Term(x)); }
    explicit Poly(const Term& x) { terms.push_back(x); }
    explicit Poly(const vector<Factor>& _factors);
    explicit Poly(const vector<Term>& _terms);

    void appendTerm(const Term& x);
    void appendTerms(const list<Term>& x);
    inline size_t numTerms() { return terms.size(); }

    friend Poly operator + (const Factor& x, const Factor& y);
    friend Poly operator - (const Factor& x, const Factor& y);
    friend Poly operator + (const Term& x, const Term& y);
    friend Poly operator - (const Term& x, const Term& y);
    void operator += (const Factor& y);
    void operator -= (const Factor& y);

    friend std::ostream& operator<<(std::ostream& o, const Poly& poly);
  private:
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

std::ostream& operator<<(std::ostream& o, const Poly& poly) {
    Poly::list_citer_t it;
    if (poly.terms.size() > 0) {
        it = poly.terms.begin();
        o << *it;
        ++it;
        for (; it != poly.terms.end(); ++it)
            o << " + " << *it;
    }
    return o;
}

void Poly::appendTerm(const Term& x) {
    Poly::list_iter_t it;
    for (it = terms.begin(); it != terms.end() && *it < x; ++it)
        ;  // iterate until *it >= x
    if (*it == x)
        it->coefficient() += x.coefficient();
    else
        terms.insert(it, x);
}

void Poly::appendTerms(const list<Term>& x) {
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

void Poly::operator+=(const Factor& y) {
    this->appendTerm(Term(y));
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

void Poly::operator-=(const Factor& y) {
    this->appendTerm(Term(Frac(-1), y));
}

#endif // POLY_HPP
