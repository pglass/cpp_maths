#include <iostream>
#include <limits>
#include <UnitTest++.h>
#include "../Frac.hpp"
#include "../common.hpp"

using namespace std;

numeric_limits<double> dbl_limits;

TEST(constructionAndOutput) {
    CHECK(testOutput(Frac(0), "(0/1)"));
    CHECK(testOutput(Frac(1), "(1/1)"));
    CHECK(testOutput(Frac(12), "(12/1)"));

    CHECK(testOutput(Frac(5, 2), "(5/2)"));
    CHECK(testOutput(Frac(10, 4), "(5/2)"));
    CHECK(testOutput(Frac(10, 100), "(1/10)"));
    CHECK(testOutput(Frac(-11, 44), "(-1/4)"));
    CHECK(testOutput(Frac(22, -44), "(-1/2)"));
    CHECK(testOutput(Frac(-33, -44), "(3/4)"));

    CHECK_THROW(Frac(Int(4), Int(0)), divide_by_zero_error);

    CHECK(testOutput(Frac("0/1"), "(0/1)"));
    CHECK(testOutput(Frac("-0/1"), "(0/1)"));
    CHECK(testOutput(Frac("-12/-13"), "(12/13)"));
    CHECK(testOutput(Frac("12/-14"), "(-6/7)"));

    CHECK_THROW(Frac("-"), invalid_argument);
    CHECK_THROW(Frac("-/"), invalid_argument);
    CHECK_THROW(Frac("1/"), invalid_argument);
    CHECK_THROW(Frac("  1   /   "), invalid_argument);

    CHECK(testOutput(Frac::from_double(0.0), "(0/1)"));
    CHECK(testOutput(Frac::from_double(1.0), "(1/1)"));
    CHECK(testOutput(Frac::from_double(-1.0), "(-1/1)"));
    CHECK(testOutput(Frac::from_double(1.1), "(11/10)"));
    CHECK(testOutput(Frac::from_double(11.1), "(111/10)"));
    CHECK(testOutput(Frac::from_double(1.11), "(111/100)"));
    CHECK(testOutput(Frac::from_double(0.111), "(111/1000)"));
    CHECK(testOutput(Frac::from_double(0.1111), "(1111/10000)"));
    CHECK(testOutput(Frac::from_double(0.11111), "(11111/100000)"));
    CHECK(testOutput(Frac::from_double(0.123456789), "(123456789/1000000000)"));
//    CHECK(testOutput(Frac::from_double(987654321.123456789), "(987654321123456789/1000000000000000000)"));
    CHECK(testOutput(Frac::from_double(dbl_limits.max()), 
        "(1797693134862315708145274237317043567980705675258449965989174768031"
         "5726078002853876058955863276687817154045895351438246423432132688946"
         "4182768467546703537516986049910576551282076245490090389328944075868"
         "5084551339423045832369032229481658085593321233482747978262041447231"
         "68738177180919299881250404026184124858368/1)"));

	CHECK(testOutput(Frac::from_double(1.00000000000000000000000000001), "(1/1)"));
    CHECK(testOutput(Frac::from_double(dbl_limits.min()), 
		"(0/1)"));

    CHECK_THROW(Frac::from_double(dbl_limits.infinity()), invalid_argument);
    CHECK_THROW(Frac::from_double(dbl_limits.quiet_NaN()), invalid_argument);
    CHECK_THROW(Frac::from_double(dbl_limits.signaling_NaN()), invalid_argument);
}

TEST(relationalOperators) {
    Frac x(3, 2);
    Frac y(6, 4);
    Frac z(3, 1);
    Frac a(4, 5);
    Frac b(6, 5);
    CHECK(x == x);
    CHECK(x == y);
    CHECK(x != z);
    CHECK(x != a);
    CHECK(x != b);
    CHECK(a < z);
    CHECK(a < b);
    CHECK(a < x);
    CHECK(a < x);
    CHECK(x <= x);
    CHECK(x <= z);
    CHECK(x <= y);
    CHECK(x >= x);
    CHECK(x >= y);
    CHECK(z >= x);
}

TEST(additionAndSubtraction) {
    Frac x(3, 2);
    Frac y(-7, 3);
    Frac z(3, 1);
    Frac a(4, 5);
    Frac b(6, 5);
    CHECK(x + x == z);
    CHECK(a + b == Frac(2, 1));
    CHECK(x + a == Frac(23, 10));
    CHECK(x + y == Frac(-5, 6));
    CHECK(a - b == Frac(-2, 5));
    CHECK(x - x == Frac(0, 1));
    CHECK(b - x == Frac(-3, 10));

    Frac c(x);
    c += c;
    CHECK(c == z);
    c = Frac(x);
    c += a;
    CHECK(c == Frac(23, 10));
    c = Frac(a);
    c -= b;
    CHECK(c == Frac(-2, 5));
    c = Frac(x);
    c -= c;
    CHECK(c == Frac(0));

    // check +=, -= with an Int
    Int i(3);
    c = Frac(x);
    c += i;
    CHECK(c == Frac(9, 2));
    c -= i;
    CHECK(c == Frac(3, 2));
}

TEST(multiplicationAndDivision) {
    Frac x(3, 2);
    Frac y(-7, 3);
    Frac a(4, 5);
    Frac b(6, 5);
    CHECK(x * x == Frac(9, 4));
    CHECK(x * y == Frac(-7, 2));
    CHECK(x * a == b);
    CHECK(x * Frac(0) == Frac(0));
    CHECK(x / x == Frac(1));
    CHECK(x / y == Frac(-9, 14));
    CHECK(x / a == Frac(15, 8));
    CHECK(a / b == Frac(2, 3));

    Frac c(x);
    c *= c;
    CHECK(c == Frac(9, 4));
    c /= c;
    CHECK(c == Frac(1, 1));
    c = Frac(x);
    c *= y;
    CHECK(c == Frac(-7, 2));
    c = Frac(a);
    c /= b;
    CHECK(c == Frac(2, 3));

    // check *=, /= with an Int
    Int i(3);
    c = Frac(x);
    c *= i;
    CHECK(c == Frac(9, 2));
    c /= i;
    CHECK(c == Frac(3, 2));
}

TEST(exponentiation) {
    Frac x(3, 2);
    CHECK((x ^ 3) == Frac(27, 8));
    x ^= 3;
    CHECK(x == Frac(27, 8));
}

int main() {
    UnitTest::RunAllTests();
}
