#include <sstream>
#include <string>
#include <vector>
#include <UnitTest++.h>
#include <cassert>
#include "../Int.hpp"
#include "../common.hpp"

using std::cout;
using std::endl;

namespace IntTest {

bool runLessThan(const Int& x, const Int& y, bool expected) {
    bool lt = (x < y);
    bool result = (lt == expected); 
    if (!result) {
        cout << "FAILED: " << x << " < " << y << " = " 
             << lt << " : " << expected << endl;
    }
    return result;
}

bool runAddition(const Int& x, const Int& y, const Int& expected) {
    const Int& sum1 = x + y;
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

bool runSubtraction(const Int& x, const Int& y, const Int& expected) {
    const Int& diff1 = x - y;
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

bool runMultiplication(const Int& x, const Int& y, const Int& expected) {
    const Int& prod1 = x * y;
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

bool runDivideByInt(const Int& x, int32_t y, const Int& expected) {
    Int actual;
    divide_by_int(x, y, actual);
    bool result = (actual == expected);
    if (!result)
        cout << "FAILED: " << x << " div_int " << y << " = "
             << actual << " : " << expected << endl;
    return result;
}

bool runDivision(const Int& x, const Int& y, const Int& expected) {
    const Int& quotient1 = x / y;
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

bool runModulus(const Int& x, const Int& y, const Int& expected) {
    const Int& m1 = x % y;
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

bool runExponentiation(const Int& x, const Int& y, const Int& expected) {
    const Int& exp1 = x ^ y;
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

bool runTimesPowerTen(const Int& x, int32_t power, const Int& expected) {
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
    for (size_t i = 0; i < x.size(); ++i) {
        const std::string& y = x.substr(0, i + 1);
        CHECK(testOutput(Int(y), y));
        CHECK(testOutput(Int("-" + y), "-" + y));
    }

    // check zero is parsed properly
    x = "000000000000000000000";
    for (size_t i = 0; i < x.size(); ++i) {
        const std::string& y = x.substr(0, i + 1);
        CHECK(testOutput(Int(y), "0"));
        CHECK(testOutput(Int("-" + y), "0"));
    }
    
    // check leading zeroes are properly ignored
    x = "000000000123456789123456789";
    for (size_t i = 0; i < x.size()-10; ++i) {
        const std::string& y = x.substr(0, i + 10);
        const std::string& z = x.substr(9, i + 1);
        CHECK(testOutput(Int(y), z));
        CHECK(testOutput(Int("-" + y), "-" + z));
    }
    for (int i = 0; i < 9; ++i) {
        const std::string& y = x.substr(i);
        const std::string& z = x.substr(9);
        CHECK(testOutput(Int(y), z));
    }

    // check that exceptions are thrown 
    CHECK_THROW(Int("abcdefghij"), std::invalid_argument);
    CHECK_THROW(Int("1234567a9"), std::invalid_argument);
    CHECK_THROW(Int(""), std::invalid_argument);

    // check other constructors
    CHECK(testOutput(Int(9999, 2), "9999000000000000000000"));

    assert(sizeof(int64_t) == 8);
    int64_t k = 1234567891234567;
    CHECK(testOutput(Int(k), "1234567891234567"));
    CHECK(testOutput(Int(k, 2), "1234567891234567000000000000000000"));
}

TEST(lessThan) {
    CHECK(runLessThan(Int( 1), Int( 2), true));
    CHECK(runLessThan(Int(-1), Int( 2), true));
    CHECK(runLessThan(Int(-2), Int( 1), true));
    CHECK(runLessThan(Int(-2), Int(-1), true));
    CHECK(runLessThan(Int( 2), Int( 1), false));
    CHECK(runLessThan(Int( 2), Int(-1), false));
    CHECK(runLessThan(Int( 1), Int(-2), false));
    CHECK(runLessThan(Int(-1), Int(-2), false));

    CHECK(runLessThan(Int(-1), Int(-1), false));
    CHECK(runLessThan(Int(1), Int(1), false));

    CHECK(runLessThan(Int(0), Int(0), false));
    CHECK(runLessThan(Int(0), Int(1), true));
    CHECK(runLessThan(Int(0), Int(-1), false));

    CHECK(runLessThan(Int("123456787123456788"),
                       Int("123456788123456787"), 
                       true));
    CHECK(runLessThan(Int("123456787123456788"),
                       Int("123456780123456788000000000"), 
                       true));
}

TEST(booleanOperators) {
    for (int32_t i = -9; i < 9; ++i) {
        for (int32_t j = -9; j < 9; ++j) {
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
            CHECK(runAddition(Int(i), Int(j), Int(i + j)));
    CHECK(runAddition(Int("99999999999999"),
                       Int("1"),
                       Int("100000000000000")));
    CHECK(runAddition(Int("123456789012345678"),
                       Int("9876543210"),
                       Int("123456798888888888")));
    CHECK(runAddition(Int("123456789012345678"),
                       Int("987654"),
                       Int("123456789013333332")));
    CHECK(runAddition(Int("999999999"),
                       Int("999999999"),
                       Int("1999999998")));
    CHECK(runAddition(Int("999999999999999999"),
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
            CHECK(runSubtraction(Int(i), Int(j), Int(i - j)));
    CHECK(runSubtraction(Int("1000000000000"),
                          Int("1"),
                          Int("999999999999")));
    CHECK(runSubtraction(Int("123456798888888888"),
                          Int("9876543210"),
                          Int("123456789012345678")));
    CHECK(runSubtraction(Int("123456789013333332"),
                          Int("987654"),
                          Int("123456789012345678")));
    CHECK(runSubtraction(Int("1"),
                          Int("999999999999999999"),
                          Int("-999999999999999998")));
    CHECK(runSubtraction(Int("1000000001"),
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
            CHECK(runMultiplication(Int(i), Int(j), Int(i * j)));
    CHECK(runMultiplication(Int("999999999"),
                             Int("999999999"), 
                             Int("999999998000000001")));
    CHECK(runMultiplication(Int("9999999999"),
                             Int("9999999999"),
                             Int("99999999980000000001")));
    CHECK(runMultiplication(Int("1234567891234"),
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
    CHECK(runDivideByInt(Int("999999999999999999"),    9, Int("111111111111111111")));
    CHECK(runDivideByInt(Int("999999999999999998"),    9, Int("111111111111111110")));
    CHECK(runDivideByInt(Int("999999999999999999"), 3456, Int("289351851851851")));
    CHECK(runDivideByInt(Int("123456789123456789"), 3456, Int("35722450556555")));

    for (int i = -50; i < 50; ++i)
        for (int j = 50; j < 50; ++j)
            CHECK(runDivision(Int(i), Int(j), Int(i / j)));

    assert(sizeof(int64_t) == 8);
    for (int64_t i = -100000000000000; i < 100000000000000; i += 7777777777777)
        for (int64_t j = -100000000000000; j < 100000000000000; j += 9999999999999)
            CHECK(runDivision(Int(i), Int(j), Int(i / j)));


    CHECK(runDivision(Int("152415787806666675432666675280250887626"),
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
    CHECK(runModulus(Int("0"), Int("3"), Int(0)));

    CHECK(runModulus(Int("5"),  Int( "3"), Int(2)));
    CHECK(runModulus(Int("5"),  Int("-3"), Int(-1)));
    CHECK(runModulus(Int("-5"), Int( "3"), Int(1)));
    CHECK(runModulus(Int("-5"), Int("-3"), Int(-2)));

    CHECK(runModulus(Int("-5"), Int("-5"), Int(0)));
    CHECK(runModulus(Int( "5"), Int("-5"), Int(0)));
    CHECK(runModulus(Int("-5"), Int( "5"), Int(0)));
    CHECK(runModulus(Int( "5"), Int( "5"), Int(0)));

    CHECK(runModulus(Int( "987654321"), Int( "417"), Int(246)));
    CHECK(runModulus(Int("-987654321"), Int( "417"), Int(171)));
    CHECK(runModulus(Int( "987654321"), Int("-417"), Int(-171)));
    CHECK(runModulus(Int("-987654321"), Int("-417"), Int(-246)));

    CHECK_THROW(runModulus(Int("1234"), Int("0"), Int(0)), divide_by_zero_error);

    // check /= is safe to use on a single instance
    Int x("111111111222222222");
    x %= x;
    CHECK(testOutput(x, "0"));
    x = Int("-111111111222222222");
    x %= x;
    CHECK(testOutput(x, "0"));
}

// negative exponents are handled elsewhere (than Int), since the result must be a fraction or decimal
TEST(exponentiation) {
    // check (arg0 ^ arg1) == arg2
    CHECK(runExponentiation(Int(2), Int(0), Int(1)));
    CHECK(runExponentiation(Int(2), Int(1), Int(2)));
    CHECK(runExponentiation(Int(2), Int(2), Int(4)));
    CHECK(runExponentiation(Int(2), Int(3), Int(8)));
    CHECK(runExponentiation(Int(-2), Int(0), Int(1)));
    CHECK(runExponentiation(Int(-2), Int(1), Int(-2)));
    CHECK(runExponentiation(Int(-2), Int(2), Int(4)));
    CHECK(runExponentiation(Int(-2), Int(3), Int(-8)));

    CHECK(runExponentiation(Int(2), Int(8), Int(256)));
    CHECK(runExponentiation(Int(2), Int(9), Int(512)));

    CHECK(runExponentiation(Int(987), Int(20),
        Int("769738223842181441453473918342107866070875104572033095898801")));

    // check ^= safe to use with single instances
    Int x("123");
    x ^= x;
    CHECK(testOutput(x, 
        std::string("11437436793461719009988029522806627674621807845185022977588797505"
                    "23695047856668964466065683652015421696499747277306288423453431965"
                    "81134895919942820874449837212099476648958359023796078549041949007"
                    "807220625356526926729664064846685758382803707100766740220839267")));
}

TEST(timesPowerTen) {
    CHECK(runTimesPowerTen(Int(2),  0, Int("2")));
    CHECK(runTimesPowerTen(Int(2),  1, Int("20")));
    CHECK(runTimesPowerTen(Int(2),  2, Int("200")));
    CHECK(runTimesPowerTen(Int(2),  3, Int("2000")));
    CHECK(runTimesPowerTen(Int(2),  4, Int("20000")));
    CHECK(runTimesPowerTen(Int(2),  5, Int("200000")));
    CHECK(runTimesPowerTen(Int(2),  6, Int("2000000")));
    CHECK(runTimesPowerTen(Int(2),  7, Int("20000000")));
    CHECK(runTimesPowerTen(Int(2),  8, Int("200000000")));
    CHECK(runTimesPowerTen(Int(2),  9, Int("2000000000")));
    CHECK(runTimesPowerTen(Int(2), 10, Int("20000000000")));
    CHECK(runTimesPowerTen(Int(2), 11, Int("200000000000")));
    CHECK(runTimesPowerTen(Int(2), 12, Int("2000000000000")));
    CHECK(runTimesPowerTen(Int(2), 13, Int("20000000000000")));
    CHECK(runTimesPowerTen(Int(2), 14, Int("200000000000000")));
    CHECK(runTimesPowerTen(Int(2), 15, Int("2000000000000000")));
    CHECK(runTimesPowerTen(Int(2), 16, Int("20000000000000000")));
    CHECK(runTimesPowerTen(Int(2), 17, Int("200000000000000000")));
    CHECK(runTimesPowerTen(Int(2), 18, Int("2000000000000000000")));
    CHECK(runTimesPowerTen(Int(2), 19, Int("20000000000000000000")));

    CHECK(runTimesPowerTen(Int("2"),                     0,  Int(2)));
    CHECK(runTimesPowerTen(Int("20"),                   -1,  Int(2)));
    CHECK(runTimesPowerTen(Int("200"),                  -2,  Int(2)));
    CHECK(runTimesPowerTen(Int("2000"),                 -3,  Int(2)));
    CHECK(runTimesPowerTen(Int("20000"),                -4,  Int(2)));
    CHECK(runTimesPowerTen(Int("200000"),               -5,  Int(2)));
    CHECK(runTimesPowerTen(Int("2000000"),              -6,  Int(2)));
    CHECK(runTimesPowerTen(Int("20000000"),             -7,  Int(2)));
    CHECK(runTimesPowerTen(Int("200000000"),            -8,  Int(2)));
    CHECK(runTimesPowerTen(Int("2000000000"),           -9,  Int(2)));
    CHECK(runTimesPowerTen(Int("20000000000"),          -10, Int(2)));
    CHECK(runTimesPowerTen(Int("200000000000"),         -11, Int(2)));
    CHECK(runTimesPowerTen(Int("2000000000000"),        -12, Int(2)));
    CHECK(runTimesPowerTen(Int("20000000000000"),       -13, Int(2)));
    CHECK(runTimesPowerTen(Int("200000000000000"),      -14, Int(2)));
    CHECK(runTimesPowerTen(Int("2000000000000000"),     -15, Int(2)));
    CHECK(runTimesPowerTen(Int("20000000000000000"),    -16, Int(2)));
    CHECK(runTimesPowerTen(Int("200000000000000000"),   -17, Int(2)));
    CHECK(runTimesPowerTen(Int("2000000000000000000"),  -18, Int(2)));
    CHECK(runTimesPowerTen(Int("20000000000000000000"), -19, Int(2)));

    CHECK(runTimesPowerTen(Int("2"),                    -1,  Int(0)));
    CHECK(runTimesPowerTen(Int("20"),                   -2,  Int(0)));
    CHECK(runTimesPowerTen(Int("200"),                  -3,  Int(0)));
    CHECK(runTimesPowerTen(Int("2000"),                 -4,  Int(0)));
    CHECK(runTimesPowerTen(Int("20000"),                -5,  Int(0)));
    CHECK(runTimesPowerTen(Int("2"),                    -11, Int(0)));
    CHECK(runTimesPowerTen(Int("20"),                   -12, Int(0)));
    CHECK(runTimesPowerTen(Int("200"),                  -13, Int(0)));
    CHECK(runTimesPowerTen(Int("2000"),                 -14, Int(0)));
    CHECK(runTimesPowerTen(Int("20000"),                -15, Int(0)));
    CHECK(runTimesPowerTen(Int("20000000000"),          -11, Int(0)));
    CHECK(runTimesPowerTen(Int("200000000000"),         -12, Int(0)));
    CHECK(runTimesPowerTen(Int("2000000000000"),        -13, Int(0)));
    CHECK(runTimesPowerTen(Int("20000000000000"),       -14, Int(0)));
    CHECK(runTimesPowerTen(Int("200000000000000"),      -15, Int(0)));
    CHECK(runTimesPowerTen(Int("20000000000"),          -21, Int(0)));
    CHECK(runTimesPowerTen(Int("200000000000"),         -22, Int(0)));
    CHECK(runTimesPowerTen(Int("2000000000000"),        -23, Int(0)));
    CHECK(runTimesPowerTen(Int("20000000000000"),       -24, Int(0)));
    CHECK(runTimesPowerTen(Int("200000000000000"),      -25, Int(0)));
}

/* Note: The Int::read(istream&) method is already tested since 
 * Int(string&) and the operator>> overload both call Int::read(istream&).
 * This just checks some functionality concerning the operator>> overload.
 */ 
TEST(inStream) {
    Int x;
    std::stringstream ss;

    // check operator>> is usable repeatedly on the same instance
    std::vector<std::string> testStrings;
    testStrings.push_back("5");
    testStrings.push_back("-5");
    testStrings.push_back("1234");
    testStrings.push_back("-1234");
    testStrings.push_back("1123456789");
    testStrings.push_back("-1123456789");
    testStrings.push_back("123456789123456789");
    testStrings.push_back("-123456789123456789");

    for (int i = 0; i < testStrings.size(); ++i) {
        ss.str(testStrings[i]);
        ss.clear();
        ss >> x;
        CHECK(testOutput(x, testStrings[i]));
    }

    // check that the fail bit is set on unparseable content
    std::vector<std::string> badStrings;
    badStrings.push_back("-");
    badStrings.push_back("a");
    badStrings.push_back("a1234");

    for (int i = 0; i < badStrings.size(); ++i) {
        ss.str(badStrings[i]);
        ss.clear();
        ss >> x;
        CHECK(ss.fail());
    }
}

} // namespace IntTest
