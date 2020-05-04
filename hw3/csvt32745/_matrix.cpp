#include <iostream>
#include <array>
#include <sstream>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
#include "pybind11/numpy.h"
#include "mkl.h"

namespace py = pybind11;

class Matrix{
    public:

    
    Matrix(size_t nrow, size_t ncol)
        :m_nrow(nrow), m_ncol(ncol)
    {
        reset_data(nrow, ncol);
    }

    /* Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_data(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_data, other.m_data);
    } */

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_data(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix(std::vector<std::vector<double>> const & v){
        m_nrow = v.size();
        m_ncol = v[0].size();
        reset_data(m_nrow, m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = v[i][j];
            }
        }
    }

    ~Matrix()
    {
        reset_data(0, 0);
    }

    /* 
        Attributes
     */

    double *data() const { return m_data; }
    double *data() { return m_data; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }


    /* 
        Operator Overloading
     */

    double   operator() (size_t r, size_t c) const { return m_data[index(r, c)]; }
    double & operator() (size_t r, size_t c)       { return m_data[index(r, c)]; }
     
    /* Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_data(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_data, other.m_data);
        return *this;
    } */

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_data(other.m_nrow, other.m_ncol);
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

    bool operator==(Matrix const & other)
    {
        if (this == &other) { return true; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            return false;
        }
        
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                if((*this)(i,j) != other(i,j))
                    return false;
            }
        }
        return true;
    }

    /* 
        Methods
     */

    void set_data(size_t r, size_t c, double val){
        m_data[index(r, c)] = val;
    }

    double get_data(size_t r, size_t c) const{
        return m_data[index(r, c)];
    }

    std::string reprString(){
        std::stringstream stm;
        size_t idx = 0;
        for(size_t i = 0; i < m_nrow; ++i){
            for(size_t j = 0; j < m_ncol; ++j){
                stm << m_data[idx] << " ";
                idx++;
            }
            stm << std::endl;
        }
        return stm.str();
    }

    size_t index(size_t row, size_t col) const{
        return row*m_ncol + col;
    }

    void reset_data(size_t nrow, size_t ncol) {
        if(m_data) delete[] m_data;

        const size_t size = nrow * ncol;
        if(size > 0) m_data = new double[size];
        else m_data = nullptr;

        m_nrow = nrow;
        m_ncol = ncol;
    }

    private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double* m_data = nullptr;
};

Matrix multiply_naive(const Matrix& a, const Matrix& b)
{
    auto ans = Matrix(a.nrow(), b.ncol());
    for(size_t r = 0; r < ans.nrow(); r++){
        for(size_t c = 0; c < ans.ncol(); c++){
            double s = 0;
            for(size_t i = 0; i < a.ncol(); i++){
                s += a(r, i) * b(i, c);
            }
            ans(r, c) = s;
        }
    }
    return ans;
}

Matrix multiply_mkl(const Matrix& a, const Matrix& b)
{
    size_t m = a.nrow();
    size_t k = a.ncol();
    size_t n = b.ncol();
    Matrix c = Matrix(m, n);
    
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
    m, n, k, 1,
    a.data(), k,
    b.data(), n,
    0, c.data(), n);

    return c;
}
 
PYBIND11_MODULE(_matrix, m){
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init([](size_t r, size_t c) {return new Matrix(r, c);}))
        .def(py::init<Matrix&>())
        .def(py::init<std::vector<std::vector<double>>&>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__repr__", &Matrix::reprString)
        .def_buffer([](Matrix &m) -> py::buffer_info{
            return py::buffer_info(
                m.data(),
                sizeof(double),
                py::format_descriptor<double>::format(),
                2,
                { m.nrow(), m.ncol() },
                { sizeof(double)*m.ncol(), sizeof(double) }
            );
        })
        .def("__setitem__", [](Matrix &m, std::array<double, 2> tp, double val){
            m.set_data(tp[0], tp[1], val);
        })
        .def("__getitem__", [](Matrix &m, std::array<double, 2> tp){
            return m.get_data(tp[0], tp[1]);
        })
        ;
}
