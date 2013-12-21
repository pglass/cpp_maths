#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <UnitTest++.h>
#include "../Mat.hpp"
#include "../common.hpp"

namespace MatTest {

template <typename T>
bool matrixFloatEquals(const Mat<T>& x, const Mat<T>& y) {
    if (x.numRows() != y.numRows() || x.numCols() != y.numCols())
        return false;
    
    for (size_t i = 0; i < x.numRows(); ++i)
        for (size_t j = 0; j < x.numCols(); ++j)
            if (!float_equals(x(i, j), y(i, j)))
                return false;
    return true;
}

TEST(matrixEquality) {
    Mat<double> a(10, 10);
    Mat<double> b(10, 10);
    Mat<double> c(10, 9);
    Mat<double> d(9, 10);
    
    a.setEntries(1);
    b.setEntries(1);
    CHECK(a == b);
    CHECK(b == a);
    CHECK(a == a);

    a.setEntries(2);
    b.setEntries(3);
    CHECK(a != b);
    CHECK(b != a);
    a.setEntries(4);
    b.setEntries(4);
    b(b.numRows()-1, b.numCols()-1) = 5;
    CHECK(a != b);

    a.setEntries(5);
    c.setEntries(5);
    d.setEntries(5);
    CHECK(a != c);
    CHECK(a != d);
    CHECK(c != d);
}

TEST(matrixNegate) {
    Mat<double> a(10, 10, 5);
    Mat<double> b(10, 10, -5);
    CHECK(matrixFloatEquals(-a, b));
    CHECK(matrixFloatEquals(-b, a));
}

TEST(matrixAddition) {
    Mat<double> a(10, 10, 1);
    Mat<double> b(10, 10, 2);
    Mat<double> c(10, 10, 3);
    Mat<double> d(9, 9, 3);
    CHECK(matrixFloatEquals((a + b), c));
    CHECK_THROW(a + d, std::domain_error);

    a += b;
    CHECK(matrixFloatEquals(a, c));
    CHECK_THROW((a += d), std::domain_error);
}

TEST(matrixSubtraction) {
    Mat<double> a(10, 10, 5);
    Mat<double> b(10, 10, 2);
    Mat<double> c(10, 10, 3);
    Mat<double> d(9, 9, 3);
    CHECK(matrixFloatEquals((a-b), c));
    CHECK(matrixFloatEquals((b-a), -c));
    CHECK_THROW(a - d, std::domain_error);

    a -= b;
    CHECK(matrixFloatEquals(a, c));
    CHECK_THROW((a -= d), std::domain_error);
}

TEST(matrixScale) {
    Mat<double> a(5, 10, 4);
    Mat<double> b(5, 10, 24);
    Mat<double> c(5, 10, 24);

    CHECK(matrixFloatEquals(6.0 * a, c));
    CHECK(matrixFloatEquals(a * 6.0, c));
    CHECK(matrixFloatEquals(b / 6.0, a));

    b /= 6.0;
    CHECK(matrixFloatEquals(b, a));
    a *= 6.0;
    CHECK(matrixFloatEquals(a, c));
}

TEST(matrixProduct) {
    Mat<double> a(2, 2, 1);
    Mat<double> b(2, 2, 2);
    Mat<double> c(3, 2, 2);
    Mat<double> d(2, 3, 4);
    Mat<double> e(3, 2, 4);
    Mat<double> f(3, 3, 16);
    Mat<double> g(2, 2, 24);
//    CHECK((a*a) == b);
//    CHECK(c * a == e);
//    CHECK(c * d == f);
//    CHECK(d * c == g); 
    CHECK(matrixFloatEquals(a * a, b));
    CHECK(matrixFloatEquals(c * a, e));
    CHECK(matrixFloatEquals(c * d, f));
    CHECK(matrixFloatEquals(d * c, g));
    CHECK_THROW(a * c, std::domain_error);

    c *= a;
    a *= a;
    CHECK(matrixFloatEquals(c, e));
    CHECK(matrixFloatEquals(a, b));
    CHECK_THROW((a *= c), std::domain_error);
}

TEST(matrixMinor) {
    Mat<double> a(5, 5, 2);
    Mat<double> b(4, 4, 2);
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t  j = 0; j < a.numCols(); ++j)
            CHECK(matrixFloatEquals(a.first_minor(i,j), b));
    double c_vals[] = {
        9, 2, 3, 
        1, 7, 6,
        6, 8, 5
    };
    double d_vals[] = {
        7, 6, 
        8, 5
    };
    double e_vals[] = {
        1, 6,
        6, 5
    };
    double f_vals[] = {
        1, 7,
        6, 8
    };
    Mat<double> c(c_vals, 3, 3);
    Mat<double> d(d_vals, 2, 2);
    Mat<double> e(e_vals, 2, 2);
    Mat<double> f(f_vals, 2, 2);
    CHECK(matrixFloatEquals(c.first_minor(0,0), d));
    CHECK(matrixFloatEquals(c.first_minor(0,1), e));
    CHECK(matrixFloatEquals(c.first_minor(0,2), f));
    //CHECK(c.first_minor(0,0) == d);
    //CHECK(c.first_minor(0,1) == e);
    //CHECK(c.first_minor(0,2) == f);
}

TEST(matrixDeterminant) {
    double b_vals[] = {
        1, 3,
        2, 4
    };
    double c_vals[] = {
        9, 2, 3,
        1, 7, 6,
        6, 8, 5
    };
    double d_vals[] = {
        0, 6, 0,
        3, 0, 5,
        0, 7, 0
    };
    Mat<double> a(1, 1, 3);
    Mat<double> b(b_vals, 2, 2);
    Mat<double> c(c_vals, 3, 3);
    Mat<double> d(d_vals, 3, 3);
    CHECK(float_equals(a.determinant(), 3));
    CHECK(float_equals(b.determinant(), -2));
    CHECK(float_equals(c.determinant(), -157));
    CHECK(float_equals(d.determinant(), 0));
}

TEST(matrixRowCofactorExpansion) {
    double b_vals[] = {
        1, 3,
        2, 4
    };
    double c_vals[] = {
        9, 2, 3,
        1, 7, 6,
        6, 8, 5
    };
    double d_vals[] = {
        0, 6, 0,
        3, 0, 5,
        0, 7, 0
    };
    double e_vals[] = {
        3, 7, -1, 6,
        4, 7, 2, 4, 
        9, 9, 2, 3, 
        6, 7, 2, 3
    };
    Mat<double> a(1, 1, 3);
    Mat<double> b(b_vals, 2, 2);
    Mat<double> c(c_vals, 3, 3);
    Mat<double> d(d_vals, 3, 3);
    Mat<double> e(e_vals, 4, 4);
    CHECK(a.row_cofactor_expansion() == 3);
    CHECK(b.row_cofactor_expansion() == -2);
    CHECK(c.row_cofactor_expansion() == -157);
    CHECK(d.row_cofactor_expansion() == 0);
    CHECK(e.row_cofactor_expansion() == 21);

    CHECK(a.col_cofactor_expansion() == 3);
    CHECK(b.col_cofactor_expansion() == -2);
    CHECK(c.col_cofactor_expansion() == -157);
    CHECK(d.col_cofactor_expansion() == 0);
    CHECK(e.col_cofactor_expansion() == 21);
}

TEST(matrixRREF) {
    double b_vals[] = {
        1, 1, 1,
        0, 0, 0,
        0, 0, 0
    };
    double c_vals[] = {
        1, 2, -1, -4,
        2, 3, -1, -11,
        -2, 0, -3, 22
    };
    double d_vals[] = {
        1, 0, 0, -8,
        0, 1, 0, 1,
        0, 0, 1, -2
    };
    Mat<double> a(3,3,5);
    Mat<double> b(b_vals, 3, 3);
    Mat<double> c(c_vals, 3, 4);
    Mat<double> d(d_vals, 3, 4);
    CHECK(matrixFloatEquals(a.rref(), b));
    CHECK(matrixFloatEquals(c.rref(), d));
}

TEST(matrixInverse) {
    double a_vals[] = {
        4, 3,
        3, 2
    };
    double b_vals[] = {
        -2, 3,
        3, -4
    };

    double d_vals[] = {
        1, 2, 3,
        0, 1, 4,
        5, 6, 0
    };
    double e_vals[] = {
        -24,  18,  5,
         20, -15, -4,
         -5,   4,  1,
    };
    Mat<double> a(a_vals, 2, 2);
    Mat<double> b(b_vals, 2, 2);
    Mat<double> c(3, 3, 3);
    Mat<double> d(d_vals, 3, 3);
    Mat<double> e(e_vals, 3, 3);
    Mat<double> x(1, 1, 5);
    Mat<double> y(1, 1, 1.0/5);
    CHECK(matrixFloatEquals(x.inverse(), y));
    CHECK(matrixFloatEquals(a.inverse(), b));
    CHECK(matrixFloatEquals(d.inverse(), e));
    CHECK_THROW(c.inverse(), std::domain_error);


    Mat<double> z(3, 3);
    Mat<double> zz(3, 3);
    double z_vals[] = {
        1, 2, 3,
        4, 3, 4,
        5, 6, 5
    };
    double zz_vals[] = {
        -1.0/2, 4.0/9, -1.0/18,
        0, -5.0/9, 4.0/9,
        1.0/2, 2.0/9, -5.0/18
    };
    z.setEntries(z_vals, 9);
    zz.setEntries(zz_vals, 9);
    CHECK(matrixFloatEquals(z.inverse(), zz));
}

TEST(matrixTranspose) {
    double x_vals[] = {
        1,2,3,4,5,
        6,5,4,3,2
    };
    double y_vals[] = {
        1,6,
        2,5,
        3,4,
        4,3,
        5,2
    };
    Mat<double> x(x_vals, 2, 5);
    Mat<double> y(y_vals, 5, 2);
    CHECK(matrixFloatEquals(x.transpose(), y));
    CHECK(matrixFloatEquals(y.transpose(), x));
}

TEST(matrixTrace) {
    Mat<double> x(3, 3, 3);
    CHECK(x.trace() == 9);
    CHECK(Mat<double>::identityMatrix(10).trace() == 10);

    double z_vals[] = {
        1, 2, 3,
        4, 3, 4,
        5, 6, 5
    };
    Mat<double> z(z_vals, 3, 3);
    CHECK(z.trace() == 9);
}

} // namespace MatTest
