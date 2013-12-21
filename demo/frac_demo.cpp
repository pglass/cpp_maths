#include <iostream>
#include <sstream>
#include "../src/Frac.hpp"

using namespace std;

int main() {
    Frac x, y, z;
    x = Frac(5, 20);            // automatically reduces to 1/4
    y = Frac("-21/63");         // reduces to -1/3
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "x + y = " << (x + y) << endl;
    cout << "x - y = " << (x - y) << endl;
    cout << "x * y = " << (x * y) << endl;
    cout << "x / y = " << (x / y) << endl;
    cout << "x ^ 2 = " << (x ^ 2) << endl;
    cout << "x.reciprocal() = " << x.reciprocal() << endl;
    cout << "Frac::GCD(45, 40) = " << Frac::GCD(45, 40) << endl;
    cout << "Frac::from_double(0.2) = " << Frac::from_double(0.2) << endl;  // 1/5

    // usage of stream input
    stringstream ss("(41/189)");
    Frac f;
    ss >> f;
    cout << "'" << ss.str() << "' >> f; f = " << f << endl;
}

/* The output is the following:
    x = (1/4)
    y = (-1/3)
    x + y = (-1/12)
    x - y = (7/12)
    x * y = (-1/12)
    x / y = (-3/4)
    x ^ 2 = (1/16)
    x.reciprocal() = (4/1)
    Frac::GCD(45, 40) = 5
    Frac::from_double(0.2) = (1/5)
    '(41/189)' >> f; f = (41/189)
*/
