#ifndef _MAT_HPP_
#define _MAT_HPP_
#include <cstring>
#include <iostream>
#include <vector>
#include <stdexcept>

/* Mat - a matrix class
 *   This is a templated matrix class, where the template parameter can
 *   be any number-like type with the arithmetic operators overloaded. 
 *   The matrix is stored as a single contiguous array, and offsets are
 *   computed for two-dimensional indexing.
 *   
 *   Note: "rref" stands for reduced row echelon form.
 */
template <typename T>
class Mat {
  public:
    static Mat<T> diagonalMatrix(const T* const _vals, size_t size);
    static Mat<T> identityMatrix(size_t size);
    static Mat<T> augmentedMatrix(const Mat<T>& a, const Mat<T>& b);
    
    Mat(const Mat<T>& x);
    Mat(size_t _rows, size_t _cols);                  // construct an uninitialized Mat 
    Mat(size_t _rows, size_t _cols, const T& _val);   // construct and set entries to val
    Mat(const T* _vals, size_t _rows, size_t _cols);  // construct and set entries to vals
    ~Mat();

    void setEntries(const T& val);                    // set all entries to val
    void setEntries(const T* vals, size_t size);      // set entries to the given array
    void setEntries(const std::vector<T>& vals);      // set entries to the given vector
    inline size_t numRows() const { return rows; }
    inline size_t numCols() const { return cols; }

    // use m(i, j) or m.entry(i, j) to access entries
    inline T& entry(size_t i, size_t j) const { return data[offset(i, j)]; }
    inline T& operator()(size_t i, size_t j) { return data[offset(i, j)]; }
    inline T operator()(size_t i, size_t j) const { return data[offset(i, j)]; }

    // scalar-matrix operations
    template <typename S> friend Mat<S> operator - (const Mat<S>& a);
    template <typename S> friend Mat<S> operator * (const S& a, const Mat<S>& b);
    template <typename S> friend Mat<S> operator * (const Mat<S>& m, const S& a);
    template <typename S> friend Mat<S> operator / (const Mat<S>& m, const S& a);
    template <typename S> friend void operator *= (Mat<S>& m, const S& a);
    template <typename S> friend void operator /= (Mat<S>& m, const S& a);

    // matrix-matrix operations
    template <typename S> friend Mat<S> operator + (const Mat<S>& a, const Mat<S>& b);
    template <typename S> friend Mat<S> operator - (const Mat<S>& a, const Mat<S>& b);
    template <typename S> friend Mat<S> operator * (const Mat<S>& a, const Mat<S>& b);
    template <typename S> friend void operator += (Mat<S>& a, const Mat<S>& b);
    template <typename S> friend void operator -= (Mat<S>& a, const Mat<S>& b);
    template <typename S> friend void operator *= (Mat<S>& a, const Mat<S>& b);
    template <typename S> friend bool operator == (const Mat<S>& a, const Mat<S>& b);
    template <typename S> friend bool operator != (const Mat<S>& a, const Mat<S>& b);

    T trace(); 
    T determinant();
    T row_cofactor_expansion();           // cofactor expansion along the first row
    T col_cofactor_expansion();           // cofactor expansion along the first column
    Mat<T> inverse();
    Mat<T> transpose();
    Mat<T> first_minor(size_t i, size_t j);     // compute the (i,j)-th minor
    Mat<T> cofactor(size_t i, size_t j);  // compute the (i,j)-th cofactor (signed minor)
    Mat<T> rref();                        // compute the reduced row echelon form
    void inplace_rref();

    friend void swap(Mat<T>& a, Mat<T>& b) {
        using std::swap;
        swap(a.data, b.data);
        swap(a.rows, b.rows);
        swap(a.cols, b.cols);
    }

    inline Mat<T>& operator=(Mat<T> a) {
        swap(*this, a);
        return *this;
    }
  private:
    size_t rows;
    size_t cols;
    T* data;

    void swap_rows(size_t a, size_t b);

    template <typename S>
    inline bool dimensionsMatch(const Mat<S>& other) const {
        return (rows == other.numRows()) && (cols == other.numCols());
    }
    inline size_t offset(size_t i, size_t j) const {
        return i * cols + j;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, Mat<T> m) {
    for (size_t i = 0; i < m.numRows(); ++i) {
        for (size_t j = 0; j < m.numCols(); ++j)
            out << m(i, j) << " ";
        if (i < m.numRows() - 1)
            out << std::endl;
    }
    return out;
}

template <typename T>
Mat<T>::Mat(const Mat<T>& x)
    : rows(x.numRows()), cols(x.numCols()), data(new T[rows * cols]) {
    setEntries(x.data, rows * cols);
}

template <typename T>
Mat<T>::Mat(size_t _rows, size_t _cols)
    : rows(_rows), cols(_cols), data(new T[rows * cols]) {
}

template <typename T>
Mat<T>::Mat(size_t _rows, size_t _cols, const T& _val)
    : rows(_rows), cols(_cols), data(new T[rows * cols]) {
    setEntries(_val);
}

template <typename T>
Mat<T>::Mat(const T* const _vals, size_t _rows, size_t _cols)
    : rows(_rows), cols(_cols), data(new T[rows * cols]) {
    setEntries(_vals, rows * cols);
}

template <typename T>
Mat<T>::~Mat() {
    delete[] data;
}

template <typename T>
void Mat<T>::setEntries(const T* const vals, size_t size) {
    if (size < rows * cols)
        throw std::domain_error("not enough values to fill the matrix");
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            entry(i, j) = vals[offset(i, j)];
}

template <typename T>
void Mat<T>::setEntries(const T& val) {
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            entry(i, j) = val;
}

template <typename T>
Mat<T> operator-(const Mat<T>& a) {
    Mat<T> r(a.numRows(), a.numCols());
    for (size_t i = 0; i < r.numRows(); ++i)
        for (size_t j = 0; j < r.numCols(); ++j)
            r(i, j) = -a(i, j);
    return r;
}

template <typename T>
Mat<T> operator+(const Mat<T>& a, const Mat<T>& b) {
    if (!a.dimensionsMatch(b))
        throw std::domain_error("matrix sizes don't match for +");
    Mat<T> r(a.numRows(), a.numCols());
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t j = 0; j < a.numCols(); ++j)
            r(i, j) = a(i, j) + b(i, j);
    return r;
}

template <typename T>
void operator+=(Mat<T>& a, const Mat<T>& b) {
    if (!a.dimensionsMatch(b))
        throw std::domain_error("matrix sizes don't match for +=");
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t j = 0; j < a.numCols(); ++j)
            a(i, j) += b(i, j);
}

template <typename T>
Mat<T> operator-(const Mat<T>& a, const Mat<T>& b) {
    if (!a.dimensionsMatch(b))
        throw std::domain_error("matrix sizes don't match for -");
    Mat<T> r(a.numRows(), a.numCols());
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t j = 0; j < a.numCols(); ++j)
            r(i, j) = a(i, j) - b(i, j);
    return r;
}

template <typename T>
void operator-=(Mat<T>& a, const Mat<T>& b) {
    if (!a.dimensionsMatch(b))
        throw std::domain_error("matrix sizes don't match for -=");
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t j = 0; j < a.numCols(); ++j)
            a(i, j) -= b(i, j);
}

template <typename T>
Mat<T> operator*(const T& a, const Mat<T>& m) {
    Mat<T> r(m.numRows(), m.numCols());
    for (size_t i = 0; i < m.numRows(); ++i)
        for (size_t j = 0; j < m.numCols(); ++j)
            r(i, j) = a * m(i, j);
    return r;
}

template <typename T>
Mat<T> operator*(const Mat<T>& m, const T& a) {
    return a * m;
}

template <typename T>
void operator*=(Mat<T>& m, const T& a) {
    for (size_t i = 0; i < m.numRows(); ++i)
        for (size_t j = 0; j < m.numCols(); ++j)
            m(i, j) *= a;
}

template <typename T>
Mat<T> operator/(const Mat<T>& m, const T& a) {
    Mat<T> r(m.numRows(), m.numCols());
    for (size_t i = 0; i < m.numRows(); ++i)
        for (size_t j = 0; j < m.numCols(); ++j)
            r(i, j) = m(i, j) / a;
    return r;
}

template <typename T>
void operator/=(Mat<T>& m, const T& a) {
    for (size_t i = 0; i < m.numRows(); ++i)
        for (size_t j = 0; j < m.numCols(); ++j)
            m(i, j) /= a;
}

template <typename T>
Mat<T> operator*(const Mat<T>& a, const Mat<T>& b) {
    if (a.numCols() != b.numRows())
        throw std::domain_error("matrix product undefined for given dimensions");
    T zero(0);
    Mat<T> r(a.numRows(), b.numCols(), zero);
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t k = 0; k < a.numCols(); ++k)
            for (size_t j = 0; j < b.numCols(); ++j)
                r(i, j) += a(i,k) * b(k, j);
    return r;
}

template <typename T>
void operator*=(Mat<T>& a, const Mat<T>& b) {
    if (a.numCols() != b.numRows())
        throw std::domain_error("matrix product undefined for given dimensions");
    T zero(0);
    Mat<T> r(a.numRows(), b.numCols(), zero);
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t k = 0; k < a.numCols(); ++k)
            for (size_t j = 0; j < b.numCols(); ++j)
                r(i, j) += a(i,k) * b(k, j);
    swap(a, r);
}

template <typename T>
bool operator==(const Mat<T>& a, const Mat<T>& b) {
    if (!a.dimensionsMatch(b))
        return false;
    for (size_t i = 0; i < a.numRows(); ++i)
        for (size_t j = 0; j < a.numCols(); ++j)
            if (a(i, j) != b(i , j))
                return false;
    return true;
}

template <typename T>
bool operator!=(const Mat<T>& a, const Mat<T>& b) {
    return !(a == b);
}

template <typename T>
Mat<T> Mat<T>::first_minor(size_t i, size_t j) {
    Mat<T> r(rows - 1, cols - 1);
    int ii, jj;
    for (size_t m = 0; m < r.numRows(); ++m) {
        ii = (m >= i) ? m + 1 : m;
        for (size_t n = 0; n < r.numCols(); ++n) {
            jj = (n >= j) ? n + 1 : n;
            r(m, n) = data[offset(ii, jj)];
        }
    }
    return r;
}

template <typename T>
Mat<T> Mat<T>::cofactor(size_t i, size_t j) {
    if ((i + j) % 2 == 1)
        return -first_minor(i, j);
    return first_minor(i, j);
}

template <typename T>
T Mat<T>::row_cofactor_expansion() {
    if (rows != cols)
        throw std::domain_error("cofactor expansion not defined for non-square matrices");
    else if (rows == 1)
        return data[0];
    else if (rows == 2)
        return data[0] * data[3] - data[1] * data[2];

    int row = 0; // compute cofactor expansion along this row
    T result(0);
    for (size_t j = 0; j < cols; ++j) {
        if (entry(row, j) != 0) {
            if ((row + j) % 2 == 1)
                result -= entry(row, j) * first_minor(row, j).row_cofactor_expansion();
            else
                result += entry(row, j) * first_minor(row, j).row_cofactor_expansion();
        }
    }
    return result;
}

template <typename T>
T Mat<T>::col_cofactor_expansion() {
    if (rows != cols)
        throw std::domain_error("cofactor expansion not defined for non-square matrices");
    else if (rows == 1)
        return data[0];
    else if (rows == 2)
        return data[0] * data[3] - data[1] * data[2];

    int col = 0;  // compute cofactor expansion along this column
    T result(0);
    for (size_t i = 0; i < rows; ++i) {
        if (entry(i, col) != 0) {
            if ((i + col) % 2 == 1)
                result -= entry(i, col) * first_minor(i, col).col_cofactor_expansion();
            else
                result += entry(i, col) * first_minor(i, col).col_cofactor_expansion();
        }
    }
    return result;
}

/* Compute the determinant via row recution.
 * Each row operation alters the determinant by a predictable factor.
 * We row reduce and accumulate these factors as we go, until the matrix
 * is in upper triangular form. The determinant of a triangular
 * matrix is simply the product of the main diagonal, so we multiple this
 * by the accumulated factor to get the determinant for the orginal matrix.
 *
 * This is an O(n^3) algorithm for computing the determinant, which
 * is significantly better than a cofactor expansion, which takes O(n!) (I think).
 * (and which involves allocating a buttload of space for minors as you go)
 *
 * TODO?: If the template parameter is int, this won't work due to truncation.
 */ 
template <typename T>
T Mat<T>::determinant() {
    if (rows != cols)
        throw std::domain_error("determinant is undefined for non-square matrices");
    else if (rows == 1)
        return data[0];
    else if (rows == 2)
        return data[0] * data[3] - data[1] * data[2];
    Mat<T> tmp(*this);
    T factor(1);
    size_t pivot = 0;
    for (size_t r = 0; r < tmp.numRows(); ++r) {
        if (tmp.numCols() <= pivot)
            goto end;

        /* ensure the pivot entry is non-zero by swapping rows */
        size_t i = r;
        while (tmp(i, pivot) == 0) {
            ++i;
            if (tmp.numRows() == i) {
                i = r;
                ++pivot;
                if (tmp.numCols() == pivot)
                    goto end;
            }
        }
        if (i != r) {  /* don't adjust the factor if don't need to swap */
            tmp.swap_rows(i, r);
            factor *= -1;
        }

        /* apply row operations to zero out all entries beneath the pivot */
        for (size_t i = pivot + 1; i < tmp.numRows(); ++i) {
            if (i != r) {
                T val = tmp(i, pivot);
                for (size_t j = pivot; j < tmp.numCols(); ++j)
                    tmp(i, j) -= val * tmp(r, j) / tmp(r, pivot);
            }
        }
        pivot++; 
    }
    end:
    T prod(1);
    for (size_t i = 0; i < tmp.numRows(); ++i)
        prod *= tmp(i, i);
    return prod / factor;
}

template <typename T>
Mat<T> Mat<T>::rref() {
    Mat<T> r(*this);
    r.inplace_rref();
    return r;
}

template <typename T>
void Mat<T>::swap_rows(size_t a, size_t b) {
    if (a != b) {
        for (size_t k = 0; k < cols; ++k)
            std::swap(entry(a, k), entry(b, k));
    }
}

/* to reduced row echelon form */
template <typename T>
void Mat<T>::inplace_rref() {
    size_t pivot = 0;
    for (size_t r = 0; r < rows; ++r) {
        if (cols <= pivot)
            return;

        size_t i = r;
        while (entry(i, pivot) == 0) {
            ++i;
            if (rows == i) {
                i = r;
                ++pivot;
                if (cols == pivot)
                    return;
            }
        }
        swap_rows(i, r);

        T val = entry(r, pivot);
        for (size_t j = 0; j < cols; ++j)
            entry(r, j) /= val;

        for (size_t i = 0; i < rows; ++i) {
            if (i != r) {
                val = entry(i, pivot);
                for (size_t j = 0; j < cols; ++j)
                    entry(i, j) -= val * entry(r, j);
            }
        }
        pivot++; 
    }
}

template <typename T>
Mat<T> Mat<T>::inverse() {
    if (rows != cols)
        throw std::domain_error("cannot invert a non-square matrix");
    /* (P | I) row reduces to (I | Q) where Q is the inverse of P */
    Mat<T> identity = Mat<T>::identityMatrix(rows);
    Mat<T> r = Mat<T>::augmentedMatrix(*this, identity);
    Mat<T> result(rows, cols);
    r.inplace_rref();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            /* invertible only if row reduction puts the identity on the left */
            if (r(i, j) != identity(i, j))
                throw std::domain_error("matrix is singular");
            result(i, j) = r(i, j + rows);
        }
    }
    return result;
}

template <typename T>
Mat<T> Mat<T>::diagonalMatrix(const T* const vals, size_t size) {
    Mat<T> r(size, size, 0);
    for (size_t i = 0; i < size; ++i)
        r(i, i) = vals[i];
    return r;
}

template <typename T>
Mat<T> Mat<T>::identityMatrix(size_t size) {
    Mat<T> r(size, size, 0);
    for (size_t i = 0; i < size; ++i)
        r(i, i) = 1;
    return r;
}

template <typename T>
Mat<T> Mat<T>::augmentedMatrix(const Mat<T>& a, const Mat<T>& b) {
    if (a.numRows() != b.numRows())
        throw std::domain_error("cannot augment matrices with different numbers of rows");
    Mat<T> r(a.numRows(), a.numCols() + b.numCols());
    for (size_t i = 0; i < a.numRows(); ++i) {
        for (size_t j = 0; j < a.numCols(); ++j)
            r(i, j) = a(i, j);
        for (size_t j = 0; j < b.numCols(); ++j)
            r(i, j + a.numCols()) = b(i, j);
    }
    return r;
}

template <typename T>
T Mat<T>::trace() {
    if (rows != cols)
        throw std::domain_error("trace undefined for non-square matrices");
    T result(0);
    for (size_t i = 0; i < rows; ++i)
        result += entry(i, i);
    return result;
}

template <typename T>
Mat<T> Mat<T>::transpose() {
    Mat<T> r(cols, rows);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            r(j, i) = entry(i, j);
    return r;
}

#endif
