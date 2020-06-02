#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "pybind11/operators.h"
#include "pybind11/pytypes.h"

#include "matrix.hpp"


PYBIND11_MODULE(_matrix, m){
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);

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
        .def("array", [](Matrix &m) -> py::array_t<double>{
            
            /*  allocate the buffer */
            size_t arr_size = m.size();
            py::array_t<double> result = py::array_t<double>(arr_size);

            py::capsule free_when_done(m.data(), [](void *f) {
                double *foo = reinterpret_cast<double *>(f);
                // std::cerr << "Element [0] = " << foo[0] << "\n";
                // std::cerr << "freeing memory @ " << f << "\n";
                delete[] foo;
            });

            return py::array_t<double, py::array::c_style|py::array::forcecast>(
                { m.nrow(), m.ncol() },
                { sizeof(double)*m.ncol(), sizeof(double) },
                m.data(),
                free_when_done
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
