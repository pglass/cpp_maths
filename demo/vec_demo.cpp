#include <iostream>
#include "../Vec.hpp"

using namespace std;

int main() {
    double y_vals[] = {-1.5, 2, -2.5};
    double z_vals[] = {3, -2, 1};
    Vec<double> zeroes(3);                             // Vec size 3 (entries initialize to zero)
    Vec<double> x = Vec<double>::constantVec(3, 2.5);  // Vec size 3 with all entries set to 2.5
    Vec<double> y = Vec<double>(y_vals, 3);
    Vec<double> z(3);
    z.setEntries(z_vals, 3);
    Vec<int> ix(x);

    cout << "zeroes = " << zeroes << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "z = " << z << endl;
    cout << "ix = " << ix << endl;
    cout << "z[0] = " << z[0] << ", z[1] = " << z[1] << ", z[2] = " << z[2] << endl;
    cout << "3.5 * x = " << (3.5 * x) << endl;
    cout << "x / 3.5 = " << (x / 3.5) << endl;
    cout << "x + y = " << (x + y) << endl;
    cout << "x - y = " << (x - y) << endl;
    cout << "x.concatenate(y) = " << x.concatenate(y) << endl;
    cout << "x.dot(y) = " << x.dot(y) << endl;
    cout << "x.cross(y) = " << x.cross(y) << endl;
    cout << "x.norm() = " << x.norm() << endl;
    cout << "x.unit_vector() = " << x.unit_vector() << endl;
    cout << "ix.norm() = " << ix.norm() << endl;
    cout << "ix.norm<double>() = " << ix.norm<double>() << endl;
    cout << "ix.unit_vector<double>() = " << ix.unit_vector<double>() << endl;
    cout << "scalar_triple_product(x, y, z) = " 
         << Vec<double>::scalar_triple_product(x, y, z) << endl;
    cout << "vector_triple_product(x, y, z) = " 
         << Vec<double>::vector_triple_product(x, y, z) << endl;
}

/*
    zeroes = (0, 0, 0)
    x = (2.5, 2.5, 2.5)
    y = (-1.5, 2, -2.5)
    z = (3, -2, 1)
    ix = (2, 2, 2)
    z[0] = 3, z[1] = -2, z[2] = 1
    3.5 * x = (8.75, 8.75, 8.75)
    x / 3.5 = (0.714286, 0.714286, 0.714286)
    x + y = (1, 4.5, 0)
    x - y = (4, 0.5, 5)
    x.concatenate(y) = (2.5, 2.5, 2.5, -1.5, 2, -2.5)
    x.dot(y) = -5
    x.cross(y) = (-11.25, 2.5, 8.75)
    x.norm() = 4.33013
    x.unit_vector() = (0.57735, 0.57735, 0.57735)
    ix.norm() = 3
    ix.norm<double>() = 3.4641
    ix.unit_vector<double>() = (0.57735, 0.57735, 0.57735)
    scalar_triple_product(x, y, z) = -30
    vector_triple_product(x, y, z) = (7.5, 0, -7.5)
*/
