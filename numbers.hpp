#ifndef _NUMBERS_HPP_
#define _NUMBERS_HPP_
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
    virtual std::istream& read(std::istream& in) = 0; 
    friend std::ostream& operator<<(std::ostream& out, const Number& x);
    friend std::istream& operator>>(std::istream& in, Number& x);
};

#endif
