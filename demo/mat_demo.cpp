#include <iostream>
#include "../src/Mat.hpp"

using namespace std;

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

/* The output is the following, indented for clarity: 
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
*/
