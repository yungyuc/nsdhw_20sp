#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <mkl.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;



class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(const Matrix &) = default;
    Matrix(Matrix &&) = default;
    //~Matrix() = default;

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }
    friend Matrix multiply_naive(const Matrix &, const Matrix &);
    friend Matrix multiply_mkl(const Matrix &, const Matrix &);

private:

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

/*
 * Naive matrix matrix multiplication.
 */
Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

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
            ret(i,k) = v;
        }
    }

    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}


Matrix multiply_naive(const Matrix &lhs, const Matrix &rhs) {
  const size_t m = lhs.m_nrow, n = lhs.m_ncol, l = rhs.m_ncol;
  Matrix ret(m, l);
  std::fill(std::begin(ret.m_buffer), std::end(ret.m_buffer), 0);
  for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < l; ++j)
      for (size_t k = 0; k < n; ++k)
        ret(i, j) += lhs(i, k) * rhs(k, j);
  return ret;
}

Matrix multiply_mkl(const Matrix &lhs, const Matrix &rhs) {
  // assert(lhs.ncol() == rhs.nrow());
  Matrix ret(lhs.nrow(), rhs.ncol());
  std::fill(std::begin(ret.data_), std::end(ret.data_), 0);
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, lhs.nrow(), rhs.ncol(),
              lhs.ncol(), 1., lhs.data_.data(), lhs.ncol(), rhs.data_.data(),
              rhs.ncol(), 1., ret.data_.data(), ret.ncol());
  return ret;
}

/* 
int main(int argc, char ** argv)
{
    std::cout << ">>> A(2x3) times B(3x2):" << std::endl;
    Matrix mat1(2, 3, std::vector<double>{1, 2, 3, 4, 5, 6});
    Matrix mat2(3, 2, std::vector<double>{1, 2, 3, 4, 5, 6});

    Matrix mat3 = mat1 * mat2;

    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "result matrix C (2x2) = AB:" << mat3 << std::endl;

    std::cout << ">>> B(3x2) times A(2x3):" << std::endl;
    Matrix mat4 = mat2 * mat1;
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "result matrix D (3x3) = BA:" << mat4 << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
*/

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "class Matrix";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_mkl", &multiply_mkl);
  pybind11::class_<Matrix>(m, "Matrix")
      .def(pybind11::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__",
           [](const Matrix &lhs, const Matrix &rhs) { return lhs == rhs; })
      .def("__ne__",
           [](const Matrix &lhs, const Matrix &rhs) { return lhs != rhs; })
      .def("__getitem__",
           [](const Matrix &m, std::pair<size_t, size_t> i) {
             return m(i.first, i.second);
           })
      .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> i, double v) {
        m(i.first, i.second) = v;
      });
}
