#include <sstream>
#include <stdexcept>
#include <cmath>
#include <UnitTest++.h>
#include "../src/Vec.hpp"
#include "../src/common.hpp"

using namespace std;

namespace VecTest {
    
const int IVALS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const double DVALS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

TEST(constructionAndOutput) {
    CHECK(testOutput(Vec<int>(0), "()"));
    CHECK(testOutput(Vec<int>(5), "(0, 0, 0, 0, 0)"));
    /* second arg is the length of the array */
    CHECK(testOutput(Vec<int>(IVALS, 0), "()"));
    CHECK(testOutput(Vec<int>(IVALS, 1), "(1)"));
    CHECK(testOutput(Vec<int>(IVALS, 2), "(1, 2)"));
    CHECK(testOutput(Vec<int>(IVALS, 3), "(1, 2, 3)"));
    CHECK(testOutput(Vec<int>(IVALS, 9), "(1, 2, 3, 4, 5, 6, 7, 8, 9)"));

    CHECK(testOutput(Vec<int>::constantVec(4, 3), "(3, 3, 3, 3)"));
    CHECK(testOutput(Vec<int>::constantVec(4, 3.5), "(3, 3, 3, 3)"));

    /* ensure we can construct from an array of an implicitly convertible type */
    CHECK(testOutput(Vec<double>(IVALS, 0), "()"));
    CHECK(testOutput(Vec<double>(IVALS, 1), "(1)"));
    CHECK(testOutput(Vec<double>(IVALS, 2), "(1, 2)"));
    CHECK(testOutput(Vec<double>(IVALS, 3), "(1, 2, 3)"));
    CHECK(testOutput(Vec<double>(IVALS, 9), "(1, 2, 3, 4, 5, 6, 7, 8, 9)"));

    /* check construction from a vector */
    vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    CHECK(testOutput(Vec<int>(x), "(1, 2, 3, 4)"));
    CHECK(testOutput(Vec<double>(x), "(1, 2, 3, 4)"));

    /* check setEntries and copy construction */
    Vec<double> w(5);
    w.setEntries(DVALS, 5);
    Vec<double> z(w);
    CHECK(testOutput(w, "(1, 2, 3, 4, 5)"));
    CHECK(testOutput(z, "(1, 2, 3, 4, 5)"));
    w.setEntries(3.5);
    CHECK(testOutput(w, "(3.5, 3.5, 3.5, 3.5, 3.5)"));
    Vec<int> u(w);
    Vec<double> v(u);
    CHECK(testOutput(u, "(3, 3, 3, 3, 3)"));
    CHECK(testOutput(v, "(3, 3, 3, 3, 3)"));

    CHECK_THROW(w.setEntries(DVALS, 1), invalid_argument);
}

TEST(relationalOperators) {
    Vec<int> v(IVALS, 5);
    Vec<int> w(IVALS, 5);
    Vec<int> x(IVALS, 6);

    CHECK(Vec<int>(0) == Vec<int>(0));
    CHECK(v == w);
    CHECK(w == v);
    CHECK(v != x);
    CHECK(x != v);

    w[w.size()-1] = -v[v.size() - 1];
    CHECK(v != w);
}

TEST(scaling) {
    int v_vals[] = {1,2,3,4,5};
    int w_vals[] = {2,4,6,8,10};
    Vec<int> v(v_vals, 5);
    Vec<int> w(w_vals, 5);

    CHECK(2 * v == w);
    CHECK(v * 2 == w);
    CHECK(w / 2 == v);

    Vec<int> r(v);
    r *= 2;
    CHECK(r == w);
    r /= 2;
    CHECK(r == v);
}

TEST(vecAddition) {
    int v_vals[] = {1,2,3,4,5};
    int w_vals[] = {2,4,6,8,10};
    int x_vals[] = {3,6,9,12,15};
    Vec<int> v(v_vals, 5);
    Vec<int> w(w_vals, 5);
    Vec<int> x(x_vals, 5);
    Vec<int> zero(5);
    Vec<int> z(x_vals, 6);

    CHECK(v - v == zero);
    CHECK(v + v == w);
    CHECK(v + w == x);
    CHECK(x - w == v);
    
    CHECK_THROW(v + z, invalid_argument);

    Vec<int> r(v);
    r += w;
    CHECK(r == x);
    r -= w;
    CHECK(r == v);
}

TEST(dotProduct) {
    Vec<int> v(IVALS, 5);
    Vec<int> w = 2 * v;
    Vec<int> zero(5);
    Vec<int> z(IVALS, 6);

    CHECK(v.dot(v) == 55);
    CHECK(v.dot(w) == 110);
    CHECK(w.dot(v) == 110);
    CHECK(v.dot(zero) == 0);
    CHECK_THROW(z.dot(v), invalid_argument);
    CHECK_THROW(w.dot(z), invalid_argument);
}

TEST(crossProduct) {
    Vec<int> v(IVALS, 3);                           // (1, 2, 3)
    Vec<int> w = v + Vec<int>::constantVec(3, 1);   // (2, 3, 4)
    int x_vals[] = {-1, 2, -1};
    Vec<int> x(x_vals, 3);
    Vec<int> z = Vec<int>::constantVec(4, 4);

    CHECK(v.cross(w) == x);
    CHECK(w.cross(v) == -1 * x);

    CHECK_THROW(w.cross(z), invalid_argument);
}

TEST(norm) {
    Vec<int> v(IVALS, 4);  // 1 2 3 4
    CHECK(v.norm<double>() == sqrt(30));
    CHECK(v.norm() == int(sqrt(30)));

    double w_vals[] = {1.0, 2.0, 3.0, 4.0};
    Vec<double> w(w_vals, 4);
}

TEST(unit_vector) {
    double w_vals[] = {1.0, 2.0, 3.0, 4.0};
    double x_vals[] = { 1.0/sqrt(30), 2.0/sqrt(30), 3.0/sqrt(30), 4.0/sqrt(30) };
    Vec<double> w(w_vals, 4);
    Vec<double> x(x_vals, 4);
    CHECK(w.unit_vector() == x);
    CHECK(float_equals(1.0, w.unit_vector().norm()));
    
    Vec<int> y(IVALS, 4);
    Vec<int> zero(4);
    CHECK(y.unit_vector() == zero);         // integer division truncates to zero
    CHECK(y.unit_vector<double>() == x);    // floating ops give correct value
    CHECK(float_equals(1.0, y.unit_vector<double>().norm()));
}

TEST(tripleProducts) {
   int x_vals[] = {1, -3 ,2};
   int y_vals[] = {3, 7, -5};
   int z_vals[] = {4, 1, 6};
   int vtp_vals[] = {151, 119, 103};
   Vec<int> x(x_vals, 3);
   Vec<int> y(y_vals, 3);
   Vec<int> z(z_vals, 3);
   Vec<int> vtp(vtp_vals, 3);

   CHECK(Vec<int>::scalar_triple_product(x, y, z) == 111);
   CHECK(Vec<int>::vector_triple_product(x, y, z) == vtp);
}

} // namespace VecTest
