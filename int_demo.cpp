#include <iostream>
#include "./numbers.hpp"
using std::cout;
using std::endl;

int main() {
    Int x, y, z;
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
