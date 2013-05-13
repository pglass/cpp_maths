#include <sstream>
#include <string>
#include <vector>
#include <UnitTest++.h>
#include "../Int.hpp"
#include "../common.hpp"

using std::cout;
using std::endl;

bool testLessThan(const Int& x, const Int& y, bool expected) {
    bool lt = (x < y);
    bool result = (lt == expected); 
    if (!result)
        cout << "FAILED: " << x << " < " << y << " = " 
             << lt << " : " << expected << endl;
    return result;
}

bool testAddition(const Int& x, const Int& y, const Int& expected) {
    Int sum1 = x + y;
    bool result1 = (sum1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " + " << y << " = " 
             << sum1 << " : " << expected << endl;
        return false;
    }

    Int sum2(x);
    sum2 += y;
    bool result2 = (sum2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " += " << y << " --> " 
             << sum2 << " : " << expected << endl;
        return false;
    }
    return true;
}

bool testSubtraction(const Int& x, const Int& y, const Int& expected) {
    Int diff1 = x - y;
    bool result1 = (diff1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " - " << y << " = "
             << diff1 << " : " << expected << endl;
        return false;
    }

    Int diff2(x);
    diff2 -= y;
    bool result2 = (diff2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " -= " << y << " --> " 
             << diff2 << " : " << expected << endl;
        return false;
    }
    return true;
}

bool testMultiplication(const Int& x, const Int& y, const Int& expected) {
    Int prod1 = x * y;
    bool result1 = (prod1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " * " << y << " = "
             << prod1 << " : " << expected << endl;
        return false;
    }

    Int prod2(x);
    prod2 *= y;
    bool result2 = (prod2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " *= " << y << " --> "
             << prod2 << " : " << expected << endl;
        return false;
    }
    return true;
}

bool testDivideByInt(const Int& x, int y, const Int& expected) {
    Int actual;
    divide_by_int(x, y, actual);
    bool result = (actual == expected);
    if (!result)
        cout << "FAILED: " << x << " div_int " << y << " = "
             << actual << " : " << expected << endl;
    return result;
}

bool testDivision(const Int& x, const Int& y, const Int& expected) {
    Int quotient1 = x / y;
    bool result1 = (quotient1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " / " << y << " = "
             << quotient1 << " : " << expected << endl;
        return false;
    }

    Int quotient2(x);
    quotient2 /= y;
    bool result2 = (quotient2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " /= " << y << " --> " 
             << quotient2 << " : " << expected << endl;
        return false;
    }
    return true;
}

bool testModulus(const Int& x, const Int& y, const Int& expected) {
    Int m1 = x % y;
    bool result1 = (m1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " % " << y << " = " 
             << m1 << " : " << expected << endl;
        return false;
    }

    Int m2(x);
    m2 %= y;
    bool result2 = (m2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " %= " << y << " --> " 
             << m2 << " : " << expected << endl;
        return false;
    }
    return true;
}


bool testExponentiation(const Int& x, const Int& y, const Int& expected) {
    Int exp1 = x ^ y;
    bool result1 = (exp1 == expected);
    if (!result1) {
        cout << "FAILED: " << x << " ^ " << y << " = "
             << exp1 << " : " << expected << endl;
        return false;
    }

    Int exp2(x);
    exp2 ^= y;
    bool result2 = (exp2 == expected);
    if (!result2) {
        cout << "FAILED: " << x << " ^= " << y << " --> " 
             << exp2 << " : " << expected << endl;
        return false;
    }
    return true;
}

bool testTimesPowerTen(const Int& x, int power, const Int& expected) {
    Int copy(x);
    copy.times_power_ten(power);
    bool result = (copy == expected);
    if (!result)
        cout << "FAILED: " << x << " times_power_ten(" << power << ") = " 
            << copy << " : " << expected << endl;
    return result;
}

TEST(printInt) {
    std::string x("123456789123456789123456789123456789");
    std::string y;
    for (size_t i = 0; i < x.size(); ++i) {
        y = x.substr(0, i + 1);
        CHECK(testOutput(Int(y), y));
        CHECK(testOutput(Int("-" + y), "-" + y));
    }

    // check zero is parsed properly
    x = "000000000000000000000";
    for (size_t i = 0; i < x.size(); ++i) {
        y = x.substr(0, i + 1);
        CHECK(testOutput(Int(y), "0"));
        CHECK(testOutput(Int("-" + y), "0"));
    }
    
    // check leading zeroes are properly ignored
    x = "000000000123456789123456789";
    std::string z;
    for (size_t i = 0; i < x.size()-10; ++i) {
        y = x.substr(0, i + 10);
        z = x.substr(9, i + 1);
        CHECK(testOutput(Int(y), z));
        CHECK(testOutput(Int("-" + y), "-" + z));
    }
    for (int i = 0; i < 9; ++i) {
        y = x.substr(i);
        z = x.substr(9);
        CHECK(testOutput(Int(y), z));
    }

    // check that exceptions are thrown 
    CHECK_THROW(Int("abcdefghij"), std::invalid_argument);
    CHECK_THROW(Int("1234567a9"), std::invalid_argument);
    CHECK_THROW(Int(""), std::invalid_argument);

    // check other constructors
    CHECK(testOutput(Int(9999, 2), "9999000000000000000000"));
    long k(1234567891234567);
    CHECK(testOutput(Int(k), "1234567891234567"));
    CHECK(testOutput(Int(k, 2), "1234567891234567000000000000000000"));
}

TEST(lessThan) {
    CHECK(testLessThan(Int( 1), Int( 2), true));
    CHECK(testLessThan(Int(-1), Int( 2), true));
    CHECK(testLessThan(Int(-2), Int( 1), true));
    CHECK(testLessThan(Int(-2), Int(-1), true));
    CHECK(testLessThan(Int( 2), Int( 1), false));
    CHECK(testLessThan(Int( 2), Int(-1), false));
    CHECK(testLessThan(Int( 1), Int(-2), false));
    CHECK(testLessThan(Int(-1), Int(-2), false));

    CHECK(testLessThan(Int(-1), Int(-1), false));
    CHECK(testLessThan(Int(1), Int(1), false));

    CHECK(testLessThan(Int(0), Int(0), false));
    CHECK(testLessThan(Int(0), Int(1), true));
    CHECK(testLessThan(Int(0), Int(-1), false));

    CHECK(testLessThan(Int("123456787123456788"),
                       Int("123456788123456787"), 
                       true));
    CHECK(testLessThan(Int("123456787123456788"),
                       Int("123456780123456788000000000"), 
                       true));
}

TEST(booleanOperators) {
    for (int i = -9; i < 9; ++i) {
        for (int j = -9; j < 9; ++j) {
            Int a(i);
            Int b(j);
            CHECK((a < b) == (i < j));
            CHECK((a <= b) == (i <= j));
            CHECK((a > b) == (i > j));
            CHECK((a >= b) == (i >= j));
            CHECK((a == b) == (i == j));
            CHECK((a != b) == (i != j));
        }
    }
}

TEST(negation) {
    Int x(3);
    Int y(-3);
    Int zero(0);
    CHECK(-x == y);
    CHECK(-y == x);
    CHECK(-y > zero);
    CHECK(-x < zero);
}

TEST(addition) {
    for (int i = -30; i < 30; ++i)
        for (int j = -50; j < 50; j += 7)
            CHECK(testAddition(Int(i), Int(j), Int(i + j)));
    CHECK(testAddition(Int("99999999999999"), 
                       Int("1"),
                       Int("100000000000000")));
    CHECK(testAddition(Int("123456789012345678"),
                       Int("9876543210"),
                       Int("123456798888888888")));
    CHECK(testAddition(Int("123456789012345678"),
                       Int("987654"),
                       Int("123456789013333332")));
    CHECK(testAddition(Int("999999999"),
                       Int("999999999"),
                       Int("1999999998")));
    CHECK(testAddition(Int("999999999999999999"),
                       Int("999999999999999999"),
                       Int("1999999999999999998")));
    // check += is safe to use on a single instance
    Int x("111111111222222222");
    x += x;
    CHECK(testOutput(x, "222222222444444444"));
    x = Int("-111111111222222222");
    x += x;
    CHECK(testOutput(x, "-222222222444444444"));
}

TEST(subtraction) {
    for (int i = -30; i < 30; ++i)
        for (int j = -50; j < 50; j += 5)
            CHECK(testSubtraction(Int(i), Int(j), Int(i - j)));
    CHECK(testSubtraction(Int("1000000000000"), 
                          Int("1"),
                          Int("999999999999")));
    CHECK(testSubtraction(Int("123456798888888888"),
                          Int("9876543210"),
                          Int("123456789012345678")));
    CHECK(testSubtraction(Int("123456789013333332"),
                          Int("987654"),
                          Int("123456789012345678")));
    CHECK(testSubtraction(Int("1"), 
                          Int("999999999999999999"),
                          Int("-999999999999999998")));
    CHECK(testSubtraction(Int("1000000001"),
                          Int("999999999999999999"),
                          Int("-999999998999999998")));
    // check -= is safe to use on a single instance
    Int x("111111111222222222");
    x -= x;
    CHECK(testOutput(x, "0"));
    x = Int("-111111111222222222");
    x -= x;
    CHECK(testOutput(x, "0"));
}

TEST(multiplication) {
    for (int i = -50; i < 50; ++i)
        for (int j = -100; j < 100; j += 7)
            CHECK(testMultiplication(Int(i), Int(j), Int(i * j)));
    CHECK(testMultiplication(Int("999999999"), 
                             Int("999999999"), 
                             Int("999999998000000001")));
    CHECK(testMultiplication(Int("9999999999"), 
                             Int("9999999999"),
                             Int("99999999980000000001")));
    CHECK(testMultiplication(Int("1234567891234"),
                             Int("123456789123456789123456789"),
                             Int("152415787806666675432666675280250887626")));
    // check *= is safe to use with a single instance
    Int x("111111111222222222");
    x *= x;
    CHECK(testOutput(x, "12345679037037036999999999950617284"));
    x = Int("-111111111222222222");
    x *= x;
    CHECK(testOutput(x, "12345679037037036999999999950617284"));
}

TEST(division) {
    CHECK(testDivideByInt(Int("999999999999999999"), 9, Int("111111111111111111")));
    CHECK(testDivideByInt(Int("999999999999999998"), 9, Int("111111111111111110")));
    CHECK(testDivideByInt(Int("999999999999999999"), 3456, Int("289351851851851")));
    CHECK(testDivideByInt(Int("123456789123456789"), 3456, Int("35722450556555")));

    for (int i = -50; i < 50; ++i)
        for (int j = 50; j < 50; ++j)
            CHECK(testDivision(Int(i), Int(j), Int(i / j)));

    for (long i = -100000000000000; i < 100000000000000; i += 7777777777777)
        for (long j = -100000000000000; j < 100000000000000; j += 9999999999999)
            CHECK(testDivision(Int(i), Int(j), Int(i / j)));

    CHECK(testDivision(Int("152415787806666675432666675280250887626"),
                       Int("123456789123456789123456789"),
                       Int("1234567891234")));
    // check /= is safe to use on a single instance
    Int x("111111111222222222");
    x /= x;
    CHECK(testOutput(x, "1"));
    x = Int("-111111111222222222");
    x /= x;
    CHECK(testOutput(x, "1"));

}

TEST(modulus) {
    CHECK(testModulus(Int("0"), Int("3"), Int(0)));

    CHECK(testModulus(Int("5"),  Int( "3"), Int(2)));
    CHECK(testModulus(Int("5"),  Int("-3"), Int(-1)));
    CHECK(testModulus(Int("-5"), Int( "3"), Int(1)));
    CHECK(testModulus(Int("-5"), Int("-3"), Int(-2)));

    CHECK(testModulus(Int("-5"), Int("-5"), Int(0)));
    CHECK(testModulus(Int( "5"), Int("-5"), Int(0)));
    CHECK(testModulus(Int("-5"), Int( "5"), Int(0)));
    CHECK(testModulus(Int( "5"), Int( "5"), Int(0)));

    CHECK(testModulus(Int( "987654321"), Int( "417"), Int(246)));
    CHECK(testModulus(Int("-987654321"), Int( "417"), Int(171)));
    CHECK(testModulus(Int( "987654321"), Int("-417"), Int(-171)));
    CHECK(testModulus(Int("-987654321"), Int("-417"), Int(-246)));

    CHECK_THROW(testModulus(Int("1234"), Int("0"), Int(0)), divide_by_zero_error);

    // check /= is safe to use on a single instance
    Int x("111111111222222222");
    x %= x;
    CHECK(testOutput(x, "0"));
    x = Int("-111111111222222222");
    x %= x;
    CHECK(testOutput(x, "0"));
}

// negative exponents are handled elsewhere, since the result must be a fraction or decimal    
TEST(exponentiation) {
    CHECK(testExponentiation(Int(2), Int(0), Int(1)));
    CHECK(testExponentiation(Int(2), Int(1), Int(2)));
    CHECK(testExponentiation(Int(2), Int(2), Int(4)));
    CHECK(testExponentiation(Int(2), Int(3), Int(8)));
    CHECK(testExponentiation(Int(-2), Int(0), Int(1)));
    CHECK(testExponentiation(Int(-2), Int(1), Int(-2)));
    CHECK(testExponentiation(Int(-2), Int(2), Int(4)));
    CHECK(testExponentiation(Int(-2), Int(3), Int(-8)));

    CHECK(testExponentiation(Int("2"), Int("8"), Int(256)));
    CHECK(testExponentiation(Int("2"), Int("9"), Int(512)));

    CHECK(testExponentiation(Int("987"), Int("20"),
        Int("769738223842181441453473918342107866070875104572033095898801")));

    // check ^= safe to use with single instances
    Int x("123");
    x ^= x;
    CHECK(testOutput(x, 
        std::string("11437436793461719009988029522806627674621807845185022977588797505")
                  + "23695047856668964466065683652015421696499747277306288423453431965"
                  + "81134895919942820874449837212099476648958359023796078549041949007"
                  + "807220625356526926729664064846685758382803707100766740220839267"));
}

TEST(timesPowerTen) {
    CHECK(testTimesPowerTen(Int(2),  0, Int("2"))); 
    CHECK(testTimesPowerTen(Int(2),  1, Int("20"))); 
    CHECK(testTimesPowerTen(Int(2),  2, Int("200"))); 
    CHECK(testTimesPowerTen(Int(2),  3, Int("2000"))); 
    CHECK(testTimesPowerTen(Int(2),  4, Int("20000"))); 
    CHECK(testTimesPowerTen(Int(2),  5, Int("200000"))); 
    CHECK(testTimesPowerTen(Int(2),  6, Int("2000000"))); 
    CHECK(testTimesPowerTen(Int(2),  7, Int("20000000"))); 
    CHECK(testTimesPowerTen(Int(2),  8, Int("200000000"))); 
    CHECK(testTimesPowerTen(Int(2),  9, Int("2000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 10, Int("20000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 11, Int("200000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 12, Int("2000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 13, Int("20000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 14, Int("200000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 15, Int("2000000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 16, Int("20000000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 17, Int("200000000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 18, Int("2000000000000000000"))); 
    CHECK(testTimesPowerTen(Int(2), 19, Int("20000000000000000000"))); 

    CHECK(testTimesPowerTen(Int("2"),                     0,  Int(2))); 
    CHECK(testTimesPowerTen(Int("20"),                   -1,  Int(2))); 
    CHECK(testTimesPowerTen(Int("200"),                  -2,  Int(2))); 
    CHECK(testTimesPowerTen(Int("2000"),                 -3,  Int(2))); 
    CHECK(testTimesPowerTen(Int("20000"),                -4,  Int(2))); 
    CHECK(testTimesPowerTen(Int("200000"),               -5,  Int(2))); 
    CHECK(testTimesPowerTen(Int("2000000"),              -6,  Int(2))); 
    CHECK(testTimesPowerTen(Int("20000000"),             -7,  Int(2))); 
    CHECK(testTimesPowerTen(Int("200000000"),            -8,  Int(2))); 
    CHECK(testTimesPowerTen(Int("2000000000"),           -9,  Int(2))); 
    CHECK(testTimesPowerTen(Int("20000000000"),          -10, Int(2))); 
    CHECK(testTimesPowerTen(Int("200000000000"),         -11, Int(2))); 
    CHECK(testTimesPowerTen(Int("2000000000000"),        -12, Int(2))); 
    CHECK(testTimesPowerTen(Int("20000000000000"),       -13, Int(2))); 
    CHECK(testTimesPowerTen(Int("200000000000000"),      -14, Int(2))); 
    CHECK(testTimesPowerTen(Int("2000000000000000"),     -15, Int(2))); 
    CHECK(testTimesPowerTen(Int("20000000000000000"),    -16, Int(2))); 
    CHECK(testTimesPowerTen(Int("200000000000000000"),   -17, Int(2))); 
    CHECK(testTimesPowerTen(Int("2000000000000000000"),  -18, Int(2))); 
    CHECK(testTimesPowerTen(Int("20000000000000000000"), -19, Int(2))); 

    CHECK(testTimesPowerTen(Int("2"),                    -1,  Int(0))); 
    CHECK(testTimesPowerTen(Int("20"),                   -2,  Int(0))); 
    CHECK(testTimesPowerTen(Int("200"),                  -3,  Int(0))); 
    CHECK(testTimesPowerTen(Int("2000"),                 -4,  Int(0))); 
    CHECK(testTimesPowerTen(Int("20000"),                -5,  Int(0))); 
    CHECK(testTimesPowerTen(Int("2"),                    -11, Int(0))); 
    CHECK(testTimesPowerTen(Int("20"),                   -12, Int(0))); 
    CHECK(testTimesPowerTen(Int("200"),                  -13, Int(0))); 
    CHECK(testTimesPowerTen(Int("2000"),                 -14, Int(0))); 
    CHECK(testTimesPowerTen(Int("20000"),                -15, Int(0))); 
    CHECK(testTimesPowerTen(Int("20000000000"),          -11, Int(0))); 
    CHECK(testTimesPowerTen(Int("200000000000"),         -12, Int(0))); 
    CHECK(testTimesPowerTen(Int("2000000000000"),        -13, Int(0))); 
    CHECK(testTimesPowerTen(Int("20000000000000"),       -14, Int(0))); 
    CHECK(testTimesPowerTen(Int("200000000000000"),      -15, Int(0))); 
    CHECK(testTimesPowerTen(Int("20000000000"),          -21, Int(0))); 
    CHECK(testTimesPowerTen(Int("200000000000"),         -22, Int(0))); 
    CHECK(testTimesPowerTen(Int("2000000000000"),        -23, Int(0))); 
    CHECK(testTimesPowerTen(Int("20000000000000"),       -24, Int(0))); 
    CHECK(testTimesPowerTen(Int("200000000000000"),      -25, Int(0))); 
}

/* Note: The Int::read(istream&) method is already tested since 
 * Int(string&) and the operator>> overload both call Int::read(istream&).
 * This just checks some functionality concerning the operator>> overload.
 */ 
TEST(inStream) {
    Int x;
    std::stringstream ss;

    // check operator>> is usable repeatedly on the same instance
    const std::vector<std::string> testStrings = {
        "5",
        "-5",
        "1234",
        "-1234",
        "1123456789",
        "-1123456789",
        "123456789123456789",
        "-123456789123456789",
    };
    for (int i = 0; i < testStrings.size(); ++i) {
        ss.str(testStrings[i]);
        ss.clear();
        ss >> x;
        CHECK(testOutput(x, testStrings[i]));
    }

    // check that the fail bit is set on unparseable content
    const std::vector<std::string> badStrings = {
        "-",
        "a",
        "a1234",
    };
    for (int i = 0; i < badStrings.size(); ++i) {
        ss.str(badStrings[i]);
        ss.clear();
        ss >> x;
        CHECK(ss.fail());
    }
}

int main() {
	return UnitTest::RunAllTests();
}
