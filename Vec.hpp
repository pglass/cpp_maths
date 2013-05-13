#ifndef _VEC_HPP_
#define _VEC_HPP_
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include "./common.hpp"

/* Vec - a templated mathematical vector type
 *   This class is mostly straightforward. The only concern is if you want to
 *      compute norm() or unit_vector() accurately, you may need to provide a template
 *      parameter to these functions. In particular, if you have a Vec<int> x, then 
 *      x.norm() will return an int in any case which will probably give the wrong answer, 
 *      but x.norm<double>() will compute the correct magnitude.
 *
 *   For future reference,
 *     In C++11, this syntax using 'auto' and a trailing return type works: 
 *          template<class Lhs, class Rhs>
 *            auto adding_func(const Lhs &lhs, const Rhs &rhs) -> decltype(lhs+rhs) {return lhs + rhs;}
 *     Then we might be able to do something like:
 *          template <typename T>
 *            auto norm() -> decltype(sqrt(T(1))) { ... }
 *     However, for the time being (2013) I prefer not to rely on C++11 features.
 */
template <typename T>
class Vec {
  public:
    Vec(size_t size)
        { data.resize(size, 0); }
    template <typename S> Vec(const Vec<S>& x)                  /* copy constructor */
        { for (size_t i = 0; i < x.size(); ++i) data.push_back(T(x[i])); }
    template <typename S> Vec(const S* const vals, size_t size) /* construct from array and size */
        { for (size_t i = 0; i < size; ++i) data.push_back(T(vals[i])); }
    template <typename S> Vec(const std::vector<S>& vals)       /* construct from vector */
        { for (size_t i = 0; i < vals.size(); ++i) data.push_back(T(vals[i])); }
    
    template <typename S> void setEntries(const S& val)
        { data.assign(size(), T(val)); }
    template <typename S> void setEntries(const S* const vals, size_t _size) {
        if (_size < size())
            throw std::invalid_argument("not enough entries in array to fill Vec");
        for (size_t i = 0; i < size(); ++i)
            data[i] = T(vals[i]);
    }

    inline size_t size() const { return data.size(); }  /* number of elements */
    T&  operator[](size_t i) { return data[i]; }
    T   operator[](size_t i) const { return data[i]; }

    template <typename S> friend std::ostream& operator<<(std::ostream& out, const Vec<S>& x);

    /* scalar-vector operations */
    template <typename S> friend Vec<S> operator * (const S& a, const Vec<S>& y); 
    template <typename S> friend Vec<S> operator * (const Vec<S>& y, const S& a);
    template <typename S> friend Vec<S> operator / (const Vec<S>& y, const S& a); 
    template <typename S> friend void   operator += (Vec<S>& x, const Vec<S>& y);
    template <typename S> friend void   operator -= (Vec<S>& x, const Vec<S>& y);
    /* vector-vector operations */
    template <typename S> friend Vec<S> operator +  (const Vec<S>& x, const Vec<S>& y);
    template <typename S> friend Vec<S> operator -  (const Vec<S>& x, const Vec<S>& y);
    template <typename S> friend void   operator *= (Vec<S>& y, const S& a);
    template <typename S> friend void   operator /= (Vec<S>& y, const S& a); 
    template <typename S> friend bool   operator == (const Vec<S>& x, const Vec<S>& y);
    template <typename S> friend bool   operator != (const Vec<S>& x, const Vec<S>& y); 

    static Vec<T> constantVec(size_t size, const T& val) {  /* create a Vec of size entries set to val */
        Vec<T> r(size);
        r.setEntries(val);
        return r;
    }
    static T scalar_triple_product(const Vec<T>& x, const Vec<T>& y, const Vec<T>& z) 
        { return x.dot(y.cross(z)); }
    static Vec<T> vector_triple_product(const Vec<T>& x, const Vec<T>& y, const Vec<T>& z) 
        { return x.cross(y.cross(z)); }
    
    Vec<T> concatenate(const Vec<T>& x) const;  /* concatenate two vectors */

    T dot(const Vec<T>& x) const;
    Vec<T> cross(const Vec<T>& x) const;
    T norm();
    template <typename S> S norm();
    Vec<T> unit_vector();
    template <typename S> Vec<S> unit_vector();
  private:
    std::vector<T> data;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vec<T>& x) {
    out << "(";
    size_t i = 0;
    for (; i + 1 < x.size(); ++i)
        out << x[i] << ", ";
    if (i < x.size())
        out << x[i];
    out << ")";
    return out;
}

template <typename T>
Vec<T> operator+(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size())
        throw std::invalid_argument("cannot add two Vecs with unequal numbers of elements");
    Vec<T> r(x);
    for (size_t i = 0; i < y.size(); ++i)
        r[i] += y[i];
    return r;
}

template <typename T>
Vec<T> operator-(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size())
        throw std::invalid_argument("cannot subtract two Vecs with unequal numbers of elements");
    Vec<T> r(x);
    for (size_t i = 0; i < y.size(); ++i)
        r[i] -= y[i];
    return r;
}

template <typename T>
Vec<T> operator*(const T& a, const Vec<T>& y) {
    Vec<T> r(y);
    for (size_t i = 0; i < r.size(); ++i)
        r[i] *= a;
    return r;
}

template <typename T> 
Vec<T> operator*(const Vec<T>& y, const T& a) { 
    return a * y; 
}

template <typename T>
Vec<T> operator/(const Vec<T>& x, const T& a) {
    Vec<T> r(x);
    for (size_t i = 0; i < r.size(); ++i)
        r[i] /= a;
    return r;
}

template <typename T>
void operator+=(Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size())
        throw std::invalid_argument("cannot add two Vecs with unequal numbers of elements");
    for (size_t i = 0; i < x.size(); ++i)
        x[i] += y[i];
}

template <typename T>
void operator-=(Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size())
        throw std::invalid_argument("cannot subtract two Vecs with unequal numbers of elements");
    for (size_t i = 0; i < x.size(); ++i)
        x[i] -= y[i];
}

template <typename T>
void operator*=(Vec<T>& x, const T& y) {
    for (size_t i = 0; i < x.size(); ++i)
        x[i] *= y;
}

template <typename T>
void operator/=(Vec<T>& x, const T& y) {
    for (size_t i = 0; i < x.size(); ++i)
        x[i] /= y;
}

template <typename T>
bool operator==(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size())
        return false;
    for (size_t i = 0; i < x.size(); ++i)
        if (x[i] != y[i])
            return false;
    return true;
}

template <typename S> 
bool operator!=(const Vec<S>& x, const Vec<S>& y) { return !(x == y); }

template <typename T> 
T Vec<T>::norm() { return sqrt(dot(*this)); }

template <typename T> template <typename S> 
S Vec<T>::norm() { return sqrt(dot(*this)); }

template <typename T>
Vec<T> Vec<T>::unit_vector() {
    T len = norm();
    return (*this) / len;
}

template <typename T> template <typename S>
Vec<S> Vec<T>::unit_vector() {
    S len = norm<S>();
    return Vec<S>(*this) / len;
}

template <typename T>
T Vec<T>::dot(const Vec<T>& x) const {
    if (size() != x.size())
        throw std::invalid_argument("cannot dot two Vecs of unequal numbers of elements");
    T r(0);
    for (size_t i = 0; i < size(); ++i)
        r += data[i] * x[i];
    return r;
}

template <typename T>
Vec<T> Vec<T>::cross(const Vec<T>& x) const {
    if (size() != 3 || x.size() != 3)
        throw std::invalid_argument("cross product is undefined for Vec.size() != 3");
    T vals[3];
    vals[0] = data[1] * x[2] - data[2] * x[1];
    vals[1] = data[2] * x[0] - data[0] * x[2];
    vals[2] = data[0] * x[1] - data[1] * x[0];
    return Vec(vals, 3);
}

template <typename T>
Vec<T> Vec<T>::concatenate(const Vec<T>& x) const {
    Vec r(size() + x.size());
    for (size_t i = 0; i < size(); ++i)
        r[i] = data[i];
    for (size_t k = 0; k < x.size(); ++k)
        r[size() + k] = x[k];
    return r;
}

#endif
