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
    Matrix(size_t nrow, size_t ncol, int lsize);
    Matrix(Matrix const & other);
    Matrix(Matrix && other);
    Matrix(std::vector<std::vector<double>> const & other);

    ~Matrix() = default; // default destructor

    friend void validate_multiplication(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, int lsize);

    size_t index(size_t row, size_t col) const { return row*m_ncol + col; }
    double   operator() (size_t row, size_t col) const { if (row >= m_nrow || col >= m_ncol) return 0; else return m_buffer.at( index(row, col) ); }
    double & operator() (size_t row, size_t col)       { if (row >= m_nrow || col >= m_ncol) return trash; else return m_buffer.at( index(row, col) ); }

    bool operator== (Matrix const &other)
    {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    double *data() { return m_buffer.data(); }
    size_t real_row() const { return m_nrow; }
    size_t real_col() const { return m_ncol; }
    int nrow() const { return ((m_nrow/tsize) + (m_nrow%tsize != 0))*tsize; }
    int ncol() const { return ((m_ncol/tsize) + (m_ncol%tsize != 0))*tsize; }
    int tsize = 1;

private:

    size_t m_nrow;
    size_t m_ncol;
    double trash;
    std::vector<double> m_buffer;

};

// default contructor
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0)
{
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
}

Matrix::Matrix(size_t nrow, size_t ncol, int lsize)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0)
{
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
    tsize = lsize;
}

// copy constructor
Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_nrow * other.m_ncol, 0)
{
    std::copy(other.m_buffer.begin(), other.m_buffer.end(), m_buffer.begin());
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

class Block {

public:

    Block(size_t N);

    ~Block() = default; // default destructor

    size_t index(size_t row, size_t col) const { return row*NDIM + col; }
    double   operator() (size_t row, size_t col) const { if (row >= NDIM || col >= NDIM) return 0; else return m_buffer.at( index(row, col) ); }
    double & operator() (size_t row, size_t col)       { if (row >= NDIM || col >= NDIM) return trash; else return m_buffer.at( index(row, col) ); }

    Block &operator= (double v);
    Block &operator+= (Block const &other);

    void save(Matrix &mat, size_t it, size_t jt);

private:

    size_t NDIM;
    double trash;
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
    for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer.at(i) = v; }
    return *this;
}

Block& Block::operator+= (Block const &other)
{
    for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer.at(i) += other.m_buffer.at(i); }
    return *this;
}

void Block::save(Matrix &mat, size_t it, size_t jt)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            size_t x1 = (base_t + j) / mat.ncol();
            size_t y1 = (base_t + j) % mat.ncol();
            size_t x2 = (base_s + j) / NDIM;
            size_t y2 = (base_s + j) % NDIM;
            mat(x1, y1) += (*this)(x2, y2);
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
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            size_t x1 = (base_t + j) / NDIM;
            size_t y1 = (base_t + j) % NDIM;
            size_t x2 = (base_s + j) / mat1.ncol();
            size_t y2 = (base_s + j) % mat1.ncol();
            m_mat1(x1, y1) = mat1(x2, y2);
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            size_t x1 = (base_t + j) / NDIM;
            size_t y1 = (base_t + j) % NDIM;
            size_t x2 = (base_s + j) / mat2.ncol();
            size_t y2 = (base_s + j) % mat2.ncol();
            m_ret(x1, y1) = mat2(x2, y2);
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            size_t x1 = (j*NDIM + i) / NDIM;
            size_t y1 = (j*NDIM + i) % NDIM;
            size_t x2 = (base + j) / NDIM;
            size_t y2 = (base + j) % NDIM;
            m_mat2(x1, y1) = m_ret(x2, y2);
        }
    }
}

void Tiler::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base1 = i*NDIM;

        for (size_t k=0; k<NDIM; ++k)
        {
            const size_t base2 = k*NDIM;

            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                size_t x1 = (base1 + j) / NDIM;
                size_t y1 = (base1 + j) % NDIM;
                size_t x2 = (base2 + j) / NDIM;
                size_t y2 = (base2 + j) % NDIM;
                v += m_mat1(x1, y1) * m_mat2(x2, y2);
            }
            size_t x1 = (base1 + k) / NDIM;
            size_t y1 = (base1 + k) % NDIM;
            m_ret(x1, y1) = v;
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

    // New matrix to be returned
    Matrix ret(m1.m_nrow, m2.m_ncol);
    Matrix mat1(m1);
    Matrix mat2(m2);
    mat1.tsize = lsize;
    mat2.tsize = lsize;

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    //const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    std::cout << nrow1 << " " << m1.m_nrow << std::endl;
    std::cout << ncol1 << " " << m1.m_ncol << std::endl;
    std::cout << ncol2 << " " << m2.m_ncol << std::endl;

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

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init<Matrix const &>())
        .def(py::init<std::vector<std::vector<double>>&>())
        .def_property("nrow", &Matrix::real_row, nullptr)
        .def_property("ncol", &Matrix::real_col, nullptr)
        .def_buffer([] (Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),
                sizeof(double),
                py::format_descriptor<double>::format(),
                2,
                { m.real_row(), m.real_col() },
                { sizeof(double) * m.real_col(), sizeof(double)}
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
