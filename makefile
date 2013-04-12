CXX=g++
CXXFLAGS=-Wall -O2
#CXXFLAGS= -g -O0

INCLUDE=/usr/local/include
LIB=/usr/local/lib
TEST_DIR=./test
DEMO_DIR=./demo

frac_test: common.hpp numbers.hpp Int.hpp Frac.hpp $(TEST_DIR)/frac_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^ -lUnitTest++
	mv a.out $@
frac_demo: common.hpp numbers.hpp Int.hpp Frac.hpp $(TEST_DIR)/frac_demo.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^
	mv a.out $@

vec_test: common.hpp Vec.hpp $(TEST_DIR)/vec_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^ -lUnitTest++
	mv a.out $@
vec_demo: common.hpp Vec.hpp $(DEMO_DIR)/vec_demo.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^
	mv a.out $@

int_test: common.hpp numbers.hpp Int.hpp $(TEST_DIR)/int_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^ -lUnitTest++
	mv a.out $@
int_demo: common.hpp numbers.hpp Int.hpp $(DEMO_DIR)/int_demo.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^
	mv a.out $@

mat_test: common.hpp Mat.hpp $(TEST_DIR)/mat_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^ -lUnitTest++
	mv a.out $@
mat_demo: common.hpp Mat.hpp $(DEMO_DIR)/mat_demo.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) $^
	mv a.out $@

all_tests: int_test mat_test vec_test
	./int_test
	./mat_test
	./vec_test

clean:
	rm -rf *.o *.gch int_test int_demo mat_test mat_demo vec_test vec_demo frac_test
