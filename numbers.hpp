#ifndef _NUMBERS_H_
#define _NUMBERS_H_
#include <stdexcept>
#include <iostream>
#include <string>

struct divide_by_zero_error : public std::domain_error { 
    divide_by_zero_error() : std::domain_error("cannot divide by zero") { } 
};

/* Abstract Number base class.
 * Inherit from Number and implement print() instead of overriding operator<<
 */
struct Number { 
    virtual std::ostream& print(std::ostream& out) const = 0; 
};

std::ostream& operator<<(std::ostream& out, const Number& x) { return x.print(out); }

#endif
