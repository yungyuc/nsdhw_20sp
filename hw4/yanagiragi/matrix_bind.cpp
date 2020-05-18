#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "matrix.hpp"
#include <mkl.h>

namespace py = pybind11;

Matrix multiply_naive (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }

    Matrix ret(left.nrow(), right.ncol());

    for(size_t i = 0; i < ret.nrow(); ++i) {
        for (size_t j = 0; j < ret.ncol(); ++j) {
            double v = 0;
            for(size_t k = 0; k < left.ncol(); ++k) {
                v += left(i, k) * right(k, j);
            }
            ret(i, j) = v;
        }
    }

    return ret;
}

// buggy when def_static, use explicitly defined function instead
// perhaps due to the life cycle? (Not sure)
// https://github.com/pybind/pybind11/issues/1598
Matrix multiply_mkl (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }
    
    Matrix ret(left.nrow(), right.ncol());

    // Ref: https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm

    int m = left.nrow();
    int k = left.ncol();
    int n = right.ncol();

    #ifdef ROW_MAJOR
        cblas_dgemm(        
            CblasRowMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.ncol(),    // LDA,
            right.data(),   // B
            right.ncol(),   // LDB
            0.0,            // BETA,
            ret.data(),     // C
            ret.ncol()      // LDC
        );
    #else
        cblas_dgemm(        
            CblasColMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.nrow(),    // LDA,
            right.data(),   // B
            right.nrow(),   // LDB        
            0.0,            // BETA,
            ret.data(),     // C
            ret.nrow()      // LDC
        );
    #endif
    return ret;
}

PYBIND11_MODULE(_matrix, m) {

    /*#ifdef ROW_MAJOR
        printf("MKL: Use Row Major\n");
    #else
        printf("MKL: Use Col Major\n");
    #endif*/
    
    m.doc() = "nsdhw_20sp hw3 (yanagiragi)"; // optional module docstring
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_naive", &multiply_naive);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())        
        
    // constructors
    .def(py::init<size_t, size_t>())
    .def(py::init<size_t, size_t, std::vector<double>>())
    
    // field
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol)
    
    // for output stream
    .def("__str__", [](const Matrix &m){
        const size_t ncol = m.ncol();
        const size_t nrow = m.nrow();
        std::string out;
        out += std::string("[");
        for(size_t i = 0; i < ncol; ++i) {
            out += std::string("[");
            for(size_t j = 0; j < nrow; ++j) {
                out += std::to_string(m(j, i));
                if (j != (nrow - 1)) {
                    out += std::string(", ");
                }
            }
            out += std::string("]");
            if (i != (ncol - 1)) {
                out += std::string(",\n");
            }
        }
        out += std::string("]");
        return out;
    })

    // for assertEqual
    .def("__eq__", [](const Matrix &m1, const Matrix &m2){
        if (m1.ncol() != m2.ncol() || m1.nrow() != m2.nrow()) {
            return false;
        }
        
        for(size_t i = 0; i < m1.ncol(); ++i) {
            for(size_t j = 0; j < m1.nrow(); ++j) {
                if ( m1(j, i) != m2(j, i)) {
                    return false;
                }
            }
        }
        return true;
    })
    
    /// Bare bones interface
    .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> i) {
        if (i.first >= m.nrow() || i.second >= m.ncol())
            throw py::index_error();
        return m(i.first, i.second);
    })
    .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> i, float v) {
        if (i.first >= m.nrow() || i.second >= m.ncol())
            throw py::index_error();
        m(i.first, i.second) = v;
    });
}