#include "numbers.hpp"

std::ostream& operator<<(std::ostream& out, const Number& x) {
    return x.print(out);
}

std::istream& operator>>(std::istream& in, Number& x) {
    return x.read(in);
}
