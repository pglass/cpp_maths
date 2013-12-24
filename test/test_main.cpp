#include "UnitTest++.h"
#include "int_test.hpp"
#include "frac_test.hpp"
#include "vec_test.hpp"
#include "mat_test.hpp"
#include "poly_test.hpp"

#include <stdexcept>
#include <iostream>
int main() {
//	try {
//		UnitTest::RunAllTests();
//	} catch (std::out_of_range& e) {
//		std::cout << "OUTOFRANGE" << std::endl;
//		std::cout << e.what() << std::endl;
//	}
//    std::cout << "test" << std::endl;
//    Int y("70110028320997654320997654250887626");
//    Int prod("123456789123456789123456789000000000");
//    std::cout << (y - prod) << std::endl;
//    std::cout << IntTest::runDivision(Int("152415787806666675432666675280250887626"),
//                           Int("123456789123456789123456789"),
//                           Int("1234567891234")) << std::endl;
    UnitTest::RunAllTests();

//    std::cout << testOutput(Frac::from_double(0.123456789), "(123456789/1000000000)") << std::endl;
}

