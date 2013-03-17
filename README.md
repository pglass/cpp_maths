Overview
========
This is a simple math library in C++. There is currently an arbitrary precision integer class and a matrix class.

What's included?
-------------
The following headers are provided. 

    #include "./numbers.hpp"  // for class Int
    #include "./matrix.hpp"   // for class Mat

### numbers.hpp ###
`Int` is an arbitrary precision integer implemented as a list of ints. For example, 123456789234567890345678901 is represented as {345678901, 2345678901, 123456789}. Then all operations are done int-by-int which is considerably faster than going digit-by-digit. An `Int` can be constructed from a string or a long (or an int). The standard arithmetic operators (`+`, `-`, `*`, `/`, `%`, `^` `+=`, `-=`, `*=`, `/=`, `%=`, `^=`) and relational operators (`<`, `>`, `>=`, `<=`, `!=`, `==`) are overloaded. Note that `^` is exponentiation and not a bitwise xor.

#### Usage ####
You should be able to use this mostly the same way you use an int. This is included in the file int_demo.cpp

    #include <iostream>
    #include "./numbers.hpp"
    using std::cout;
    using std::endl;
    
    int main() {
        Int x, y, z;  // these initialize to zero
        x = Int("123456789012345678901234567890");
        y = Int("987654321098765432109876543210");
        z = Int("-12345678901234567890");
        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
        cout << "z = " << z << endl;
        cout << "x + y = " << (x + y) << endl;
        cout << "x + z = " << (x + z) << endl;
        cout << "x - y = " << (x - y) << endl;
        cout << "x / y = " << (x / y) << endl;
        cout << "y / x = " << (y / x) << endl;
        cout << "x % z = " << (x % z) << endl;
        cout << "x ^ 5 = " << (x ^ Int(5)) << endl;
    }

If the above is in the file int_demo.cpp, then compile this with `g++ numbers.hpp Int.cpp int_demo.cpp -o int_demo`. The output looks like:

    x = 123456789012345678901234567890
    y = 987654321098765432109876543210
    z = -12345678901234567890
    x + y = 1111111110111111111011111111100
    x + z = 123456789000000000000000000000
    x - y = -864197532086419753208641975320
    x / y = 0
    y / x = 8
    x % z = -12345678900000000000
    x ^ 5 = 28679718617337040378138162708415496392486976564513250475184790028886798337811616713594453748240629383657483209495862454267363852838672048294900000


### matrix.hpp ###
`Mat` is a templated matrix class. Valid operators are overloaded for matrix-matrix operations (`+`, `-`, `*`, `+=`, `-=`, `*=`) and scalar-matrix operations (`*`, `/`, `*=`, `/=`). The following functions are provided as members of a `Mat<T> m`:

* `trace()`, `determinant()`, and `inverse()`
* `minor(i, j)` and `cofactor(i, j)` return the (i, j)-th minor or cofactor (signed minor), respectively
* `rref()` and `inplace_rref()` will find the reduced row echelon form

The determinant and inverse are computed using row reduction methods, and row reduction takes O(n<sup>3</sup>) time for an _n_ by _n_ matrix. You may also compute the determinant using a cofactor/Laplace expansion along the first row or the first column using `row_cofactor_expansion` or `col_cofactor_expansion`, which is much slower (O(n!) time).

#### Usage ####
Here's an example program using `Mat`, which is included in mat_demo.cpp:

    #include <iostream>
    #include "./matrix.hpp"
    using std::cout;
    using std::endl;
    
    int main() {
        Mat<double> x(3, 4);     // 3 rows, 4 columns; allocated but uninitialized
        Mat<double> y(4, 3, 5);  // 4 rows, 3 columns; entries initialized to 5
        Mat<double> z(3, 3);
        double x_vals[] = {
            1, 2, 3, 4,
            3, 4, 5, 6,
            5, 6, 7, 8
        };
        x.setEntries(x_vals, 12);  // 12 is the size of x_vals
        z.setEntries(x_vals, 12);  // only the first 9 values are used (z is 3x3)
        cout << "x = " << endl << x << endl;
        cout << "y = " << endl << y << endl;
        cout << "z = " << endl << z << endl;
        cout << "x * y = " << endl << (x * y) << endl; // matrix multiplication
        cout << "3.0 * y = " << endl << (3.0 * y) << endl;   // multiply by scalar
        cout << "z.trace() = " << z.trace() << endl;
        cout << "z.determinant() = " << z.determinant() << endl;
        cout << "z.inverse() = " << endl << z.inverse() << endl;
        cout << "x.rref() = " << endl << x.rref() << endl; // reduced row echelon form
    }

If the above is in mat_demo.cpp, then compile this with `g++ matrix.hpp matrix.cpp mat_demo.cpp -o mat_demo`. This outputs the following (indented for clarity):

    x = 
        1 2 3 4 
        3 4 5 6 
        5 6 7 8 
    y = 
        5 5 5 
        5 5 5 
        5 5 5 
        5 5 5 
    z = 
        1 2 3 
        4 3 4 
        5 6 5 
    x * y = 
        50 50 50 
        90 90 90 
        130 130 130 
    3.0 * y = 
        15 15 15 
        15 15 15 
        15 15 15 
        15 15 15 
    z.trace() = 9
    z.determinant() = 18
    z.inverse() = 
        -0.5 0.444444 -0.0555556 
        0 -0.555556 0.444444 
        0.5 0.222222 -0.277778 
    x.rref() = 
        1 0 -1 -2 
        -0 1 2 3 
        0 0 0 0 

Todo
----

* `Int`: be sure of the values of `sizeof(int)` and `sizeof(long)`
* `Int`: implement Karatsuba multiplication 
* `Int`: overload `++` and `--` operators
* `Mat`: write nicer printing functions that align columns
* `Mat`: add a backsubstitution function
* `Mat`: add functions to diagonalize a matrix and find the Jordan canonical form. The issue here is that finding the eignevalues normally requires finding the roots of some arbitrary polynomial, which is not easy (see: http://en.wikipedia.org/wiki/Abel-Ruffini_theorem). Still, finding eigenvalues for small sized matrices should be doable.

Future goals 
------------

* An arbitrary precision decimal type
* A ratio type and complex type. There's the templated std::complex class and function overloads to work with that (sin, cos, sqrt, etc) which looks acceptable at first glance. C++11 also defines a templated std::ratio class. 
* Common functions that work with the arbitrary precision types: sin, cos, tan, atan, sqrt, nroot, and so forth.
* Support for stochastic processes: define a list of states, define an initial probability distribution and a transition matrix, simulate paths of the process, stopping times and hitting times.
