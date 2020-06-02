#include <iostream>
#include <vector>
#include <string>
#include <iterator>  // for back_inserter
#include <algorithm> // for copy() and assign()
#include <iomanip>   // std::setw
#include "mkl.h"

class Matrix
{

public:
    // Constructor
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol)
    {
        // Initialize m_buffer with zeros
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol)
    {
        if (vec.size() == m_buffer.size())
        {
            m_buffer = vec;
        }
        else
        {
            throw std::out_of_range("Vector assignment mismatch!");
        }
    }

    Matrix(std::vector<std::vector<double>> const & other)
        : m_nrow(other.size()), m_ncol(other[0].size())
    {
        for(const auto &v: other)
            m_buffer.insert(m_buffer.end(), v.begin(), v.end());
    }

    /* Define the copy constructor */
    Matrix(Matrix const &other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    /* Define the copy assignment operator */
    Matrix &operator=(Matrix const &other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            throw std::out_of_range("number of elements mismatch");
        }
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

    /* Define the move constructor */
    Matrix(Matrix &&other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    /* Define the move assignment operator */
    Matrix &operator=(Matrix &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    // Desctructor.
    ~Matrix() = default;

    void check_range(size_t row_it, size_t col_it) const
    {
        if (row_it >= m_nrow || col_it >= m_ncol)
        {
            throw std::out_of_range("Matrix index out of range");
        }
    }

    // With bound check.
    double operator()(size_t row, size_t col) const
    {
        check_range(row, col);
        return m_buffer[row * m_ncol + col];
    }
    double &operator()(size_t row, size_t col)
    {
        check_range(row, col);
        return m_buffer[row * m_ncol + col];
    }

    bool operator== (Matrix const &other) const 
    {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    std::vector<double> m_buffer;

private:
    size_t m_nrow;
    size_t m_ncol;
};

/*
 * Naive matrix-matrix multiplication.
 */
Matrix multiply_naive(Matrix const &A, Matrix const &B)
{
    // inputs: A: n x m, B: m x p
    // return: C: n x p
    // time: O(nmp)

    if (A.ncol() != B.nrow())
    {
        throw std::out_of_range("Matrix multiplication: dimensions mismatch!");
    }

    Matrix C(A.nrow(), B.ncol());

    for (size_t i = 0; i < A.nrow(); i++)
    {
        for (size_t j = 0; j < B.ncol(); j++)
        {
            C(i, j) = 0;
            for (size_t k = 0; k < A.ncol(); k++)
            {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }

    return C;
}

/*
 * Matrix multiplication using DGEMM.
 */
Matrix multiply_mkl(Matrix const &A, Matrix const &B)
{
    // inputs: A: n x m, B: m x p
    // return: C: n x p

    if (A.ncol() != B.nrow())
    {
        throw std::out_of_range("Matrix multiplication: dimensions mismatch!");
    }

    Matrix C(A.nrow(), B.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.nrow(), B.ncol(), A.ncol(), 1.0, A.m_buffer.data(), A.ncol(), B.m_buffer.data(), B.ncol(), 0.0, C.m_buffer.data(), B.ncol());

    return C;
}

/*
 * Tiling matrix-matrix multiplication.
 */
Matrix multiply_tile(Matrix const &A, Matrix const &B, size_t s_tile)
{
    const size_t tile = s_tile;
    const size_t nrow1 = A.nrow();
    const size_t ncol1 = A.ncol();
    const size_t ncol2 = B.ncol();

    Matrix C(nrow1, ncol2);

    for (size_t i=0; i < nrow1; i+=tile)
    {
        size_t imax = i + tile > nrow1 ? nrow1 : i + tile;
        for (size_t j=0; j < ncol2; j+=tile)
        {
            size_t jmax = j + tile > ncol2 ? ncol2 : j + tile;
            for (size_t k=0; k< ncol1; k+=tile)
            {
                size_t kmax = k + tile > ncol1 ? ncol1 : k + tile;
                for (size_t s_i = i; s_i < imax; ++s_i)
                {
                    for (size_t s_j = j; s_j < jmax; ++s_j)
                    {
                        double sum = 0;
                        for (size_t s_k = k; s_k < kmax; ++s_k)
                        {
                            sum += A(s_i, s_k) * B(s_k, s_j);
                        }
                        C(s_i, s_j) += sum;
                    }
                }
            }
        }
    }

    return C;
}

std::ostream &operator<<(std::ostream &ostr, Matrix const &mat)
{
    for (size_t i = 0; i < mat.nrow(); ++i)
    {
        ostr << std::endl
             << " ";
        for (size_t j = 0; j < mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}

