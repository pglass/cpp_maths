cmake_minimum_required(VERSION 2.6)
project(cpp_maths)

#add_definitions(-std=c++0x)

add_subdirectory(unittest-cpp)
include_directories(unittest-cpp/UnitTest++)

add_executable(int_demo demo/int_demo.cpp numbers.* common.* Int.*)
add_executable(vec_demo demo/vec_demo.cpp Vec.hpp)
add_executable(frac_demo demo/frac_demo.cpp numbers.* common.* Int.* Frac.*)
add_executable(mat_demo demo/mat_demo.cpp Mat.hpp)

add_executable(int_test test/int_test.cpp numbers.* common.* Int.*)
#add_executable(vec_test test/vec_test.cpp Vec.hpp)
#add_executable(frac_test test/frac_test.cpp numbers.* common.* Int.* Frac.*)
#add_executable(mat_test test/mat_test.cpp Mat.hpp)

target_link_libraries(int_test UnitTest++)
#target_link_libraries(vec_test UnitTest++)
#target_link_libraries(frac_test UnitTest++)
#target_link_libraries(mat_test UnitTest++)

enable_testing()
add_test(IntTest int_test)
#add_test(VecTest vec_test)
#add_test(FracTest frac_test)
#add_test(MatTest mat_test)
