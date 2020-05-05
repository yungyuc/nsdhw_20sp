// Developer: Wilbert (wilbert.phen@gmail.com)

#include <iostream>
#include "mkl.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);
    Matrix(Matrix const & other, int x_pad, int y_pad);
    Matrix(Matrix && other);
    Matrix(std::vector<std::vector<double>> const & other);

    ~Matrix() = default; // default destructor

    friend void validate_multiplication(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, int lsize);

    double   operator() (size_t row, size_t col) const { return m_buffer.at( row*m_ncol + col ); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at( row*m_ncol + col ); }
    double   operator() (size_t idx) const { return m_buffer.at( idx ); }
    double & operator() (size_t idx)       { return m_buffer.at( idx ); }
    void unpad(int x_pad, int y_pad);

    bool operator== (Matrix const &other)
    {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    double *data() { return m_buffer.data(); }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

};

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


class Block {

public:

    Block(size_t N);

    ~Block() = default; // default destructor

    double   operator() (size_t row, size_t col) const { return m_buffer.at( row*NDIM + col ); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at( row*NDIM + col ); }
    double   operator() (size_t idx) const { return m_buffer.at( idx ); }
    double & operator() (size_t idx)       { return m_buffer.at( idx ); }

    Block &operator= (double v);
    Block &operator+= (Block const &other);

    void save(Matrix &mat, size_t it, size_t jt);

private:

    size_t NDIM;
    std::vector<double> m_buffer;
};

// default contructor
Block::Block(size_t N)
    : NDIM(N), m_buffer(N * N, 0)
{
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
}

Block& Block::operator= (double v)
{
    std::fill(m_buffer.begin(), m_buffer.end(), v);
    return *this;
}

Block& Block::operator+= (Block const &other)
{
    const size_t max_i = NDIM*NDIM;
    for (size_t i=0; i<max_i; ++i) { m_buffer.at(i) += other.m_buffer.at(i); }
    return *this;
}

void Block::save(Matrix &mat, size_t it, size_t jt)
{
    const size_t ncol = mat.ncol();
    const size_t a = it*NDIM;
    const size_t b = jt*NDIM;

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (a + i)*ncol + b;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat(base_t + j) += (*this)(base_s + j);
        }
    }
}

class Tiler {

public:

    Tiler(size_t N);

    ~Tiler() = default; // default destructor

    void load(
        Matrix const & mat1, size_t it1, size_t jt1, 
        Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block ret() { return m_ret; }

private:

    size_t NDIM;
    Block m_mat1; // row-major
    Block m_mat2; // column-major
    Block m_ret; // row-major
};

// default contructor
Tiler::Tiler(size_t N)
    : NDIM(N), m_mat1(N), m_mat2(N), m_ret(N)
{

}

void Tiler::load(
    Matrix const & mat1, size_t it1, size_t jt1,
    Matrix const & mat2, size_t it2, size_t jt2
)
{
    //const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t a1 = it1*NDIM;
    const size_t b1 = jt1*NDIM;
    //const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();
    const size_t a2 = it2*NDIM;
    const size_t b2 = jt2*NDIM;

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s1 = (a1 + i)*ncol1 + b1;
        const size_t base_s2 = (a2 + i)*ncol2 + b2;
        const size_t base_t1 = i*NDIM;

        for (size_t j=0, base_t2=0; j<NDIM; ++j, base_t2+=NDIM)
        {
            m_mat1(base_t1 + j) = mat1(base_s1 + j);
            m_mat2(base_t2 + i) = mat2(base_s2 + j);
        }
    }
}

void Tiler::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        for (size_t k=0; k<NDIM; ++k)
        {
            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += m_mat1(i, j) * m_mat2(k, j);
            }
            m_ret(i, k) = v;
        }
    }
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
    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    for (size_t i=0; i<ret.m_nrow; ++i)
    {
        for (size_t k=0; k<ret.m_ncol; ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.m_ncol; ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
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

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, int lsize)
{
    validate_multiplication(m1, m2);

    int nx1 = ((m1.nrow()/lsize) + (m1.nrow()%lsize != 0)) * lsize - m1.nrow();
    int ny1 = ((m1.ncol()/lsize) + (m1.ncol()%lsize != 0)) * lsize - m1.ncol();
    int nx2 = ((m2.nrow()/lsize) + (m2.nrow()%lsize != 0)) * lsize - m2.nrow();
    int ny2 = ((m2.ncol()/lsize) + (m2.ncol()%lsize != 0)) * lsize - m2.ncol();
    Matrix mat1(m1, nx1, ny1);
    Matrix mat2(m2, nx2, ny2);

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    //const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    // New matrix to be returned
    Matrix ret(nrow1, ncol2);

    const size_t ntrow1 = nrow1 / lsize;
    const size_t ntcol1 = ncol1 / lsize;
    //const size_t ntrow2 = nrow2 / lsize;
    const size_t ntcol2 = ncol2 / lsize;

    Block value(lsize);
    Tiler tiler(lsize);

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {

            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.ret();
            }
            value.save(ret, it, kt);
        }
    }

    ret.unpad(nx1, ny2);
    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        //.def(py::init<Matrix const &>())
        .def(py::init<std::vector<std::vector<double>>&>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def_buffer([] (Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),
                sizeof(double),
                py::format_descriptor<double>::format(),
                2,
                { m.nrow(), m.ncol() },
                { sizeof(double) * m.ncol(), sizeof(double)}
            );
        })
        .def("__eq__", &Matrix::operator==)
        .def("__setitem__", [](Matrix &mat1, std::pair<size_t, size_t> i, double v) {
            mat1(i.first, i.second) = v;
        })
        .def("__getitem__", [](Matrix &mat1, std::pair<size_t, size_t> i) {
            return mat1(i.first, i.second);
        });
}
