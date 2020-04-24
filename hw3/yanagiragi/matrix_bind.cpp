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
    
    // row major version
    int m = left.nrow();
    int n = right.ncol();
    int k = left.ncol(); // equal to right.nrow()

    // Ref: https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm
    cblas_dgemm(
        CblasRowMajor,  //
        CblasNoTrans,   //
        CblasNoTrans,   //     
        m,              // M
        n,              // N
        k,              // K
        1.0,            // ALPHA,
        left.data(),    // A
        k,              // LDA,
        right.data(),   // B
        n,              // LDB        
        0.0,            // BETA,
        ret.data(),     // C
        n               // LDC
    );

    // col major version
    /*int m = static_cast<int>( left.nrow() );
    int n = static_cast<int>( right.ncol() );
    int k = static_cast<int>( left.ncol() ); // equal to right.nrow()

    std::cout << m << n << k << std::endl;

    cblas_dgemm(
        CblasColMajor,  //
        CblasNoTrans,   //
        CblasNoTrans,   //     
        m,              // M
        n,              // N
        k,              // K
        1.0,            // ALPHA,
        left.data(),    // A
        m,              // LDA,
        right.data(),   // B
        k,              // LDB        
        0.0,            // BETA,
        ret.data(),     // C
        n               // LDC
    );*/

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    PyEval_InitThreads();
    m.doc() = "nsdhw_20sp hw3 (yanagiragi)"; // optional module docstring
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_naive", &multiply_naive);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        
        // constructors
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, std::vector<double>>())
        
        .def("ncol", &Matrix::ncol)
        .def("nrow", &Matrix::nrow)
        .def("data", &Matrix::data)
        
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
        })
        
        .def_buffer([](Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),                               /* Pointer to buffer */
                sizeof(double),                          /* Size of one scalar */
                py::format_descriptor<double>::format(), /* Python struct-style format descriptor */
                2,                                      /* Number of dimensions */
                { m.nrow(), m.ncol() },                 /* Buffer dimensions */
                { sizeof(double) * m.ncol(),             /* Strides (in bytes) for each index */
                sizeof(double) }
            );
        });
}