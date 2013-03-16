CXX=g++
CXXFLAGS=-Wall -O2
#CXXFLAGS= -g -O0

INCLUDE=/usr/local/include
LIB=/usr/local/lib

int_test: Int.o numbers.hpp int_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) Int.o int_test.cpp -o $@ -lUnitTest++

matrix_test: matrix.o matrix.hpp matrix_test.cpp
	$(CXX) $(CXXFLAGS) -L$(LIB) -I$(INCLUDE) matrix.o matrix_test.cpp -o $@ -lUnitTest++

all_tests:
	make int_test
	make matrix_test
	./int_test
	./matrix_test

Int.o: numbers.hpp Int.cpp
	$(CXX) $(CXXFLAGS) -c $^

matrix.o: matrix.hpp matrix.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -rf *.o *.gch int_test matrix_test
