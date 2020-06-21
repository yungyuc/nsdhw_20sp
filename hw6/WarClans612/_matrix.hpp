// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once
#include <iostream>
#include <vector>

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);
    Matrix(Matrix const & other);
    Matrix(Matrix && other);
    Matrix(std::vector<std::vector<double>> const & other);

    ~Matrix() = default; // default destructor

    friend void validate_multiplication(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, int lsize);

    size_t index(size_t row, size_t col) const { return row*m_ncol + col; }
    double   operator() (size_t row, size_t col) const { return m_buffer.at( index(row, col) ); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at( index(row, col) ); }
    double   operator() (size_t idx) const { return m_buffer.at( idx ); }
    double & operator() (size_t idx)       { return m_buffer.at( idx ); }

    bool operator== (Matrix const &other)
    {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    void save(Matrix &mat, size_t it, size_t jt)
    {
        const size_t ncol = mat.ncol();

        for (size_t i=0; i<m_nrow; ++i)
        {
            size_t base_s = i*m_ncol;
            size_t base_t = (it+i)*ncol+jt;
            for (size_t j=0; j<m_ncol; ++j)
            {
                mat(base_t + j) = m_buffer.at(base_s + j);
            }
        }
    }

    double *data() { return m_buffer.data(); }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

};

void validate_multiplication(const Matrix &mat1, const Matrix &mat2);
Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, int lsize);
