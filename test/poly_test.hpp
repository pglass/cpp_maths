#ifndef POLY_TEST_HPP
#define POLY_TEST_HPP
#include <iostream>
#include <sstream>
#include <UnitTest++.h>
#include <vector>
#include "../src/Poly.hpp"
#include "../src/common.hpp"

namespace testVars {
    const Frac frac_a(2);
    const Frac frac_b(3);
    const Frac frac_c(4);
    const Factor factor_x("x", 3);
    const Factor factor_y("y", 2);
    const Factor factor_z("z", 1);
    const Term term_ax(frac_a, factor_x);
    const Term term_by(frac_b, factor_y);
    const Term term_cz(frac_c, factor_z);
    const Poly poly_x(factor_x);
    const Poly poly_y(factor_y);
    const Poly poly_z(factor_z);
    const Poly poly_ax(term_ax);
    const Poly poly_by(term_by);
    const Poly poly_cz(term_cz);
//    const vector<Factor> fxyz{x, y, z};
//    const vector<Factor> fyzx{y, z, x};
//    const vector<Factor> fzxy{z, x, y};
//    const vector<Term>   txyz{ax, by, cz};
//    const vector<Term>   tyzx{by, cz, ax};
//    const vector<Term>   tzxy{cz, ax, by};
}


namespace PolyTest {

/* Return the string representation of x (as given by operator<<)
 *
 * The coefficient of a Term is a Frac. Use frac_str to
 * ensure these tests do not need to change if the implementation
 * of Frac::print changes. */
string frac_str(const Frac& x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

TEST(FactorOutput) {
    CHECK(testOutput(Factor("x", 1), "x"));
    CHECK(testOutput(Factor("x", 2), "x^2"));
}

//TEST(TermOutput) {
//    CHECK(testOutput(Term(Frac(0), testVars::fxyz), "0"));
//    CHECK(testOutput(Term(Frac(0), testVars::fyzx), "0"));
//    CHECK(testOutput(Term(Frac(0), testVars::fzxy), "0"));

//    CHECK(testOutput(Term(Frac(1), testVars::fxyz), "x^3 * y^2 * z"));
//    CHECK(testOutput(Term(Frac(1), testVars::fyzx), "x^3 * y^2 * z"));
//    CHECK(testOutput(Term(Frac(1), testVars::fzxy), "x^3 * y^2 * z"));

//    CHECK(testOutput(Term(Frac(2), testVars::fxyz), frac_str(Frac(2)) + " * x^3 * y^2 * z"));
//    CHECK(testOutput(Term(Frac(2), testVars::fyzx), frac_str(Frac(2)) + " * x^3 * y^2 * z"));
//    CHECK(testOutput(Term(Frac(2), testVars::fzxy), frac_str(Frac(2)) + " * x^3 * y^2 * z"));
//}

TEST(PolyOutput) {
    vector<Term> terms;
    terms.push_back(Term(Frac(2), testVars::factor_x));
    terms.push_back(Term(Frac(3), testVars::factor_y));
    terms.push_back(Term(Frac(4), testVars::factor_z));
    CHECK(testOutput(Poly(terms),
        frac_str(Frac(2)) + " * x^3 + " +
        frac_str(Frac(3)) + " * y^2 + " +
        frac_str(Frac(4)) + " * z"));
}

TEST(TermMultiplication) {
    // Factor * Factor
    CHECK(testOutput(testVars::factor_x * testVars::factor_y, "x^3 * y^2"));
    CHECK(testOutput(testVars::factor_y * testVars::factor_x, "x^3 * y^2"));
    CHECK(testOutput(testVars::factor_x * testVars::factor_x, "x^6"));
    CHECK(testOutput(testVars::factor_y * testVars::factor_y, "y^4"));

    // Term * Factor, Factor * Term
    CHECK(testOutput(testVars::term_ax * testVars::factor_x, frac_str(Frac(2)) + " * x^6"));
    CHECK(testOutput(testVars::factor_x * testVars::term_ax, frac_str(Frac(2)) + " * x^6"));
    CHECK(testOutput(testVars::term_ax * testVars::factor_y, frac_str(Frac(2)) + " * x^3 * y^2"));
    CHECK(testOutput(testVars::factor_y * testVars::term_ax, frac_str(Frac(2)) + " * x^3 * y^2"));
    CHECK(testOutput(testVars::term_by * testVars::factor_x, frac_str(Frac(3)) + " * x^3 * y^2"));
    CHECK(testOutput(testVars::factor_x * testVars::term_by, frac_str(Frac(3)) + " * x^3 * y^2"));

    // Term * Term
    CHECK(testOutput(testVars::term_ax * testVars::term_by, frac_str(Frac(6)) + " * x^3 * y^2"));
    CHECK(testOutput(testVars::term_by * testVars::term_ax, frac_str(Frac(6)) + " * x^3 * y^2"));
    CHECK(testOutput(testVars::term_ax * testVars::term_ax, frac_str(Frac(4)) + " * x^6"));
    CHECK(testOutput(testVars::term_by * testVars::term_by, frac_str(Frac(9)) + " * y^4"));
    CHECK(testOutput(
        testVars::term_by * testVars::term_ax * testVars::term_cz,
        frac_str(Frac(24)) + " * x^3 * y^2 * z"));
    CHECK(testOutput(
        testVars::term_cz * testVars::term_by * testVars::term_ax,
        frac_str(Frac(24)) + " * x^3 * y^2 * z"));
    CHECK(testOutput(
        testVars::term_ax * testVars::term_cz * testVars::term_by,
        frac_str(Frac(24)) + " * x^3 * y^2 * z"));
}

TEST(TermCompare) {
    CHECK(testVars::term_ax < testVars::term_by);
    CHECK(testVars::term_by < testVars::term_cz);
    CHECK(testVars::term_ax < testVars::term_cz);
    CHECK(testVars::term_ax < (testVars::term_ax * testVars::term_ax));
    CHECK(testVars::term_by < (testVars::term_by * testVars::term_by));
}

TEST(PolyAddition) {
    CHECK(testOutput(testVars::poly_x, "x^3"));
    CHECK(testOutput(testVars::poly_y, "y^2"));
    CHECK(testOutput(testVars::poly_z, "z"));
    CHECK(testOutput(testVars::poly_ax, frac_str(testVars::frac_a) + " * x^3"));
    CHECK(testOutput(testVars::poly_by, frac_str(testVars::frac_b) + " * y^2"));
    CHECK(testOutput(testVars::poly_cz, frac_str(testVars::frac_c) + " * z"));

//    Poly pfxyz(testVars::fxyz);
//    Poly pfyzx(testVars::fyzx);
//    Poly pfzxy(testVars::fzxy);
//    Poly ptxyz(testVars::txyz);
//    Poly ptyzx(testVars::tyzx);
//    Poly ptzxy(testVars::tzxy);
//    CHECK(testOutput(pfxyz, "x^3 + y^2 + z"));
//    CHECK(testOutput(pfyzx, "x^3 + y^2 + z"));
//    CHECK(testOutput(pfzxy, "x^3 + y^2 + z"));

//    CHECK(testOutput(ptxyz,
//        frac_str(testVars::a) + " * x^3 + " +
//        frac_str(testVars::b) + " * y^2 + " +
//        frac_str(testVars::c) + " * z"));
//    CHECK(testOutput(ptyzx,
//        frac_str(testVars::a) + " * x^3 + " +
//        frac_str(testVars::b) + " * y^2 + " +
//        frac_str(testVars::c) + " * z"));
//    CHECK(testOutput(ptzxy,
//        frac_str(testVars::a) + " * x^3 + " +
//        frac_str(testVars::b) + " * y^2 + " +
//        frac_str(testVars::c) + " * z"));
}

} // namespace PolyTest
#endif // POLY_TEST_HPP
