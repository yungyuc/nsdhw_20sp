// Developer: Wilbert (wilbert.phen@gmail.com)

#include <iostream>
#include "mkl.h"

#include "_matrix.hpp"
#include "_block.hpp"
#include "_tiler.hpp"

// default contructor
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0)
{
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
}

// copy constructor
Matrix::Matrix(Matrix const & other, int x_pad=0, int y_pad=0)
    : m_nrow(other.m_nrow+x_pad), m_ncol(other.m_ncol+y_pad), m_buffer((other.m_nrow+x_pad) * (other.m_ncol+y_pad), 0)
{
    for(size_t i=0; i < m_nrow; ++i)
    {
        const size_t base_t = i*m_ncol;
        const size_t base_s = i*other.m_ncol;
        for (size_t j=0; j < m_ncol; ++j)
            if (i >= other.m_nrow || j >= other.m_ncol) m_buffer.at(base_t + j) = 0;
            else m_buffer.at(base_t + j) = other.m_buffer.at(base_s + j);
    }
}

// move constructor
Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_nrow * other.m_ncol, 0)
{
    other.m_buffer.swap(m_buffer);
}

Matrix::Matrix(std::vector<std::vector<double>> const & other)
    : m_nrow(other.size()), m_ncol(other[0].size())
{
    for(const auto &v: other)
        m_buffer.insert(m_buffer.end(), v.begin(), v.end()); 
}

void Matrix::unpad(int x_pad, int y_pad)
{
    size_t x = m_nrow-x_pad;
    size_t y = m_ncol-y_pad;
    std::vector<double> temp(x*y);
    for(size_t i=0; i < x; ++i)
    {
        const size_t base_t = i*y;
        const size_t base_s = i*m_ncol;
        for (size_t j=0; j < y; ++j)
            temp.at(base_t + j) = m_buffer.at(base_s + j);
    }
    m_buffer.resize(x*y);
    m_buffer.swap(temp);
    m_nrow = x;
    m_ncol = y;
}

void validate_multiplication(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.m_ncol != mat2.m_nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    validate_multiplication(mat1, mat2);

    // New matrix to be returned
    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i, k) = v;
        }
    }
    return ret;
};

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    validate_multiplication(mat1, mat2);

    // New matrix to be returned
    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.m_nrow, mat2.m_ncol, mat1.m_ncol, 1.0,
        mat1.m_buffer.data(), mat1.m_ncol,
        mat2.m_buffer.data(), mat2.m_ncol,
        0.0,
        ret.m_buffer.data(), mat2.m_ncol
    );
    return ret;
};

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, const int tsize)
{
    int lsize = tsize;
    validate_multiplication(m1, m2);

    const int nx1 = ((m1.m_nrow/lsize) + (m1.m_nrow%lsize != 0)) * lsize - m1.m_nrow;
    const int ny1 = ((m1.m_ncol/lsize) + (m1.m_ncol%lsize != 0)) * lsize - m1.m_ncol;
    const int nx2 = ((m2.m_nrow/lsize) + (m2.m_nrow%lsize != 0)) * lsize - m2.m_nrow;
    const int ny2 = ((m2.m_ncol/lsize) + (m2.m_ncol%lsize != 0)) * lsize - m2.m_ncol;
    Matrix mat1(m1, nx1, ny1);
    Matrix mat2(m2, nx2, ny2);

    // New matrix to be returned
    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    Block value(lsize);
    Tiler tiler(lsize);

    for (size_t it=0; it<mat1.m_nrow; it+=lsize)
    {
        for (size_t kt=0; kt<mat2.m_ncol; kt+=lsize)
        {
            value = 0;
            for (size_t jt=0; jt<mat1.m_ncol; jt+=lsize)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply(value);
            }
            value.save(ret, it, kt);
        }
    }

    ret.unpad(nx1, ny2);
    return ret;
}
