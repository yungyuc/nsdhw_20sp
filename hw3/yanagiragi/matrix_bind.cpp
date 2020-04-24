#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "matrix.hpp"
#include <mkl.h>

namespace py = pybind11;

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
    /*int m = left.nrow();
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
    );*/

    // col major version
    int m = left.nrow();
    int n = right.ncol();
    int k = left.ncol(); // equal to right.nrow()

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
    );

    return ret;
}

PYBIND11_MODULE(_vector, m) {
    PyEval_InitThreads();
    m.doc() = "nsdhw_20sp hw3 (yanagiragi)"; // optional module docstring
    m.def("multiply_mkl", &multiply_mkl);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, std::vector<double>>())
        .def("ncol", &Matrix::ncol)
        .def("nrow", &Matrix::nrow)
        .def("data", &Matrix::data)
        .def("__str__", [](const Matrix &m){
            const int ncol = m.ncol();
            const int nrow = m.nrow();
            std::string out;
            out += std::string("[");
            for(int i = 0; i < ncol; ++i) {
                out += std::string("[");
                for(int j = 0; j < nrow; ++j) {
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
            out += std::string("]\n");
            return out;
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