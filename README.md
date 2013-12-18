Overview
========
This is a math library in C++ focused on symbolic over numerical computation. I work on it as I have time away from school. That is, I still intend to work on it even if the last commit is weeks or months ago.

The following headers are provided. 

    #include "./Int.hpp"   // arbitrary-precision integer class Int
    #include "./Frac.hpp"  // fraction class Frac
    #include "./Mat.hpp"   // matrix class Mat
    #include "./Vec.hpp"   // mathematical vector class Vec
    
Usage of all the classes can be found in the corresponding files in the `demo` folder.

## Building ##

You will need to have CMake installed (version 2.6 or greater):

    mkdir build
    cd build
    cmake ..
    make

This will build all of the demo and test executables. You can run tests with:

    make test

### Int.hpp ###
`Int` is an arbitrary precision integer implemented as a list of ints. For example, 123456789234567890345678901 is represented as {345678901, 234567890, 123456789}. Then all operations are done int-by-int which is considerably faster than going digit-by-digit. The standard arithmetic operators (`+`, `-`, `*`, `/`, `%`, `^` `+=`, `-=`, `*=`, `/=`, `%=`, `^=`) and relational operators (`<`, `>`, `>=`, `<=`, `!=`, `==`) are overloaded. Note that `^` is exponentiation and not a bitwise xor. An `Int` can be constructed from a string or a long: `Int("-1234")` or `Int(-1234)`. Int also works with streams (`<<` and `>>`).

### Frac.hpp ###
`Frac` is a fraction type, or a ratio of two numbers: `(355/113)`. A Frac can be constructed from two Ints, or from a string like `"-355/113"`. The arithmetic and relational operators are overloaded, and the following functions are provided.

* `reciprocal()` returns the inverse
* static function `GCD()` computes the greatest common denominator of two `Ints`
* static function `from_double()` will produce a Frac that approximates the value of a double

### Mat.hpp ###
`Mat` is a two-dimensional matrix. Operators are overloaded for matrix-matrix operations (`+`, `-`, `*`, `+=`, `-=`, `*=`), scalar-matrix operations (`*`, `/`, `*=`, `/=`), and comparison (`==`, `!=`). The following functions are provided as members of a `Mat<T> m`:

* `trace()`, `determinant()`, and `inverse()`
* `first_minor(i, j)` and `cofactor(i, j)` return the (i, j)-th minor or cofactor (signed minor), respectively
* `rref()` and `inplace_rref()` will find the reduced row echelon form
* `m(i, j)` returns the (i, j)-th entry

There are also the following static methods for convenince:

* `diagonalMatrix()`
* `identityMatrix()`
* `augmentedMatrix()`

The contained type is passed as a template argument (e.g. `Mat<Int>`). The number of rows and columns are passed upon construction for flexibility and simpler code. The determinant and inverse are computed using row reduction methods, and row reduction takes O(n<sup>3</sup>) time for an _n_ by _n_ matrix. You may also compute the determinant using a cofactor/Laplace expansion along the first row or the first column using `row_cofactor_expansion` or `col_cofactor_expansion`, which is much slower (O(n!) time).

### Vec.hpp ###
`Vec` is a templated vector class. Operators are overloaded for vector-vector operations (`+`, `-`, `+=`, `-=`), scalar-vector operations (`*`, `/`, `*=`, `/=`), and comparision (`==`, `!=`). The following functions are provided as members:

* `dot()`, `cross()`
* `norm()`, `unit_vector()`
* `concatenate()` will append one vector to another

And the following static methods:

* `constantVec()` returns a Vec with all entries set to the same value
* `scalar_triple_product()`
* `vector_triple_product()`

Todo
----

* Need documentation.
* More test cases is always good.
* Compilation is slow. Use object files.
* `Int`: implement Karatsuba multiplication 
* `Int`: overload `++` and `--` operators
* `Frac`: implement the `read(istream&)` function
* `Mat`: write nicer printing functions that align columns
* `Mat`: add a backsubstitution function
* `Mat`: add a function to find the rank of a matrix. I believe this just requires a row reduction and counting the number of nonzero rows.
* `Mat`: add functions to find the eigensystem/diagonalization of a matrix. Small cases can be solved symobilcally via the characteristic polynomial, but this will not scale. Numerical algorithms [here](http://en.wikipedia.org/wiki/List_of_numerical_analysis_topics#Eigenvalue_algorithms) will work. The QR algorithm looks practical but will require other tools first. The Rayleigh method seems to be simple and has quick convergence.
* `Mat`: there are a number of other useful [matrix decompositions](http://en.wikipedia.org/wiki/Matrix_decomposition).

Future goals
------------

* An arbitrary precision decimal type
* A complex class
* A class that represents a multivariate polynomial
* Common functions that work with the arbitrary precision types: sin, cos, tan, atan, sqrt, nroot, and so forth.
* Support for stochastic processes: define a list of states, define an initial probability distribution and a transition matrix, simulate paths of the process, stopping times and hitting times.
