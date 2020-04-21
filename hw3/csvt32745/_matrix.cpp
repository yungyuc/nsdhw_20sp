#include <iostream>
#include <array>
#include <tuple>
#include <sstream>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
// #include <mkl.h>

namespace py = pybind11;

class Matrix{
    public:

    
    Matrix(size_t nrow, size_t ncol)
        :m_nrow(nrow), m_ncol(ncol)
    {
        reset_data(nrow, ncol);
    }

    float *data() { return m_data; }
    size_t row() const { return m_nrow; }
    size_t col() const { return m_ncol; }

    void set_data(size_t r, size_t c, float val){
        m_data[index(r, c)] = val;
    }

    float get_data(size_t r, size_t c) const{
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
        if(size > 0) m_data = new float[size];
        else m_data = nullptr;

        m_nrow = nrow;
        m_ncol = ncol;
    }

    private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    float* m_data = nullptr;
};

PYBIND11_MODULE(_matrix, m){
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init([](size_t r, size_t c) {return new Matrix(r, c);}))
        .def("__repr__", &Matrix::reprString)
        .def_buffer([](Matrix &m) -> py::buffer_info{
            return py::buffer_info(
                m.data(),
                sizeof(float),
                py::format_descriptor<float>::format(),
                2,
                { m.row(), m.col() },
                { sizeof(float)*m.col(), sizeof(float) }
            );
        })
        .def("__setitem__", [](Matrix &m, std::array<float, 2> tp, float val){
            m.set_data(tp[0], tp[1], val);
        })
        .def("__getitem__", [](Matrix &m, std::array<float, 2> tp){
            return m.get_data(tp[0], tp[1]);
        })
        ;
}
