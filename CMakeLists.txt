cmake_minimum_required(VERSION 2.6)
project(cpp_maths)

add_definitions(-Wall)

add_subdirectory(unittest-cpp)
include_directories(unittest-cpp/UnitTest++)

aux_source_directory(${PROJECT_SOURCE_DIR} SOURCES)

add_executable(int_demo demo/int_demo.cpp ${SOURCES})
add_executable(vec_demo demo/vec_demo.cpp ${SOURCES})
add_executable(frac_demo demo/frac_demo.cpp ${SOURCES})
add_executable(mat_demo demo/mat_demo.cpp ${SOURCES})

add_executable(test_main test/test_main.cpp
    test/int_test.hpp
    test/frac_test.hpp
    test/vec_test.hpp
    test/mat_test.hpp
    ${SOURCES})
target_link_libraries(test_main UnitTest++)

enable_testing()
add_test(Tests test_main)
