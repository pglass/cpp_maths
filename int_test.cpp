#include <sstream>
#include <UnitTest++.h>
#include "./numbers.hpp"

using std::cout;
using std::endl;

bool testOutput(const Int& y, const std::string& expected) {
    std::stringstream ss;
    ss << y;
    bool result = (ss.str().compare(expected) == 0);
    if (!result)
        cout << "FAILED: " << ss.str() << " : " << expected << endl;
    return result;
}

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
}

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
}

int main() {
	return UnitTest::RunAllTests();
}