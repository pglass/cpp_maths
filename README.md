Overview
========
This is a math library for C++ with a focus on symbolic computation.

The following headers are provided. 

    #include "Int.hpp"   // arbitrary-precision integer class Int
    #include "Frac.hpp"  // fraction class Frac
    #include "Mat.hpp"   // matrix class Mat
    #include "Vec.hpp"   // mathematical vector class Vec
    
Usage of all the classes can be found in the corresponding files in the `demo` folder.

## Building ##

This is known to compile with GCC 4.6.2 (MinGW on Windows) and GCC 4.7.2 (on Debian). Building requires CMake (version 2.6+).

    mkdir build
    cd build
    cmake ..
    make

This will build all of the demo and test executables. You can run tests with:

    make test

### Int.hpp ###
`Int` is an arbitrary precision integer implemented as a list of ints. For example, 111111111222222222333333333 is represented as {333333333, 222222222, 111111111}. Then all operations are done int-by-int which is considerably faster than going digit-by-digit. The standard arithmetic operators and relational operators are overloaded. Note that `^` is exponentiation and not a bitwise xor. An `Int` can be constructed from a string or a long: `Int("-1234")` or `Int(-1234)`. Int also works with streams (`<<` and `>>`).

### Frac.hpp ###
`Frac` is a fraction type -- a ratio of two Ints: `(355/113)`. A Frac can be constructed from two Ints, or from a string like `"-355/113"`. The arithmetic and relational operators are overloaded, and Frac works with streams. The following functions are provided as well:

* `reciprocal()` returns the inverse
* static function `GCD()` computes the greatest common denominator of two `Ints`
* (experimental): static function `from_double()` will produce a Frac that approximates the value of a double

### Mat.hpp ###
`Mat<T>` is a two-dimensional matrix. Operators are overloaded for matrix-matrix, scalar-matrix operations, and equality testing. The following functions are provided as members:

* `trace()`, `determinant()`, and `inverse()`
* `first_minor(i, j)` and `cofactor(i, j)` return the (i, j)-th minor or cofactor (signed minor), respectively
* `rref()` and `inplace_rref()` will find the reduced row echelon form
* `m(i, j)` returns the (i, j)-th entry

There are also the following static methods for convenince:

* `diagonalMatrix()`
* `identityMatrix()`
* `augmentedMatrix()`

The determinant and inverse are computed using row reduction methods, and row reduction takes O(n<sup>3</sup>) time for an _n_ by _n_ matrix. You may also compute the determinant using a cofactor/Laplace expansion along the first row or the first column using `row_cofactor_expansion` or `col_cofactor_expansion`, which is much slower (O(n!) time).

### Vec.hpp ###
`Vec<T>` is a mathematical vector. Operators are overloaded for vector-vector, scalar-vector operations, and equality-testing. The following functions are provided as members:

* `dot()`, `cross()`
* `norm()`, `unit_vector()`
* `concatenate()` will append another vector (producing a new vector)

And the following static methods:

* `constantVec()` returns a Vec with all entries set to the same value
* `scalar_triple_product()`
* `vector_triple_product()`

Todo
----

* Need documentation.
* `Int`: implement Karatsuba multiplication 
* `Int`: overload `++` and `--` operators
* `Mat`: write nicer printing functions that align columns
* `Mat`: add a backsubstitution function
* `Mat`: add a function to find the rank of a matrix. I believe this just requires row reducing and counting the number of nonzero rows.
* `Mat`: add functions to find the eigensystem/diagonalization of a matrix. Small cases can be solved symbolically via the characteristic polynomial, but this will not scale. Numerical algorithms [here](http://en.wikipedia.org/wiki/List_of_numerical_analysis_topics#Eigenvalue_algorithms) will work. The QR algorithm looks practical but will require other tools first. The Rayleigh method seems to be simple and has quick convergence.
* `Mat`: there are a number of other useful [matrix decompositions](http://en.wikipedia.org/wiki/Matrix_decomposition).

Future goals
------------

* An arbitrary precision decimal type
* A complex class
* A class that represents a multivariate polynomial
* Common functions that work with the arbitrary precision types: sin, cos, tan, atan, sqrt, nroot, and so forth.
* Support for stochastic processes: define a list of states, define an initial probability distribution and a transition matrix, simulate paths of the process, stopping times and hitting times.
