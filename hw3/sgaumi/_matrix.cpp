#include <iostream>
#include <vector>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
#include "pybind11/numpy.h"
#include "mkl.h"

namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }

    Matrix(Matrix const& other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; i++){
            for (size_t j=0; j<m_ncol; j++){
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix(std::vector<std::vector<double>> const & vec){
        m_nrow = vec.size();
        m_ncol = vec[0].size();
        reset_buffer(m_nrow, m_ncol);
        for (size_t i=0; i<m_nrow; i++){
            for (size_t j=0; j<m_ncol; j++){
                (*this)(i,j) = vec[i][j];
            }
        }
    }

    // TODO: copy and move constructors and assignment operators.
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
                if((*this)(i,j) != other(i,j)){return false;}
            }
        }
        return true;
    }

	
	//

    ~Matrix()
    {
        delete[] m_buffer;
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }

	void set(size_t i, size_t j, double v){m_buffer[i*m_ncol+j]=v;}
	double get(size_t i,size_t j ){return m_buffer[i*m_ncol+j];}

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
	double *buff() const { return m_buffer; }
	double *buff() { return m_buffer; }


private:

	void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow=0;
    size_t m_ncol=0;
    double * m_buffer=nullptr;

};

Matrix multiply_naive(const Matrix& m1, const Matrix& m2){
	//size_t n1=m1.nrow();
	//size_t n2=m2.ncol();
	Matrix res(m1.nrow(),m2.ncol());
	for (size_t i=0;i<res.nrow();i++){
		for (size_t j=0;j<res.ncol();j++){
			double s=0;
			for(size_t k=0;k<m1.ncol();k++){
				s=s+m1(i,k)*m2(k,j);
			}
			//std::cout<<"i"<<i<<"j"<<j<<"s"<<s<<std::endl;
			res(i,j)=s;
			//std::cout<<"resij"<<res(i,j)<<std::endl;
		}
	}
	return res;
	
}

Matrix multiply_mkl(Matrix const& m1, Matrix const& m2)
{
    size_t n1 = m1.nrow();
    size_t k = m1.ncol();
    size_t n2 = m2.ncol();
    Matrix m = Matrix(n1, n2);

	cblas_dgemm(CblasRowMajor, //CblasRowMajor=101
	CblasNoTrans, //CblasNoTrans=111
	CblasNoTrans, //CblasNoTrans=111
    n1, 
	n2, 
	k, 
	1,
    m1.buff(), 
	k,
    m2.buff(), 
	n2,
    0,
	m.buff(), 
	n2);

    return m;
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
        //.def("__repr__", )


		.def("__setitem__", [](Matrix &m, std::pair<size_t,size_t> p, double val){
            m(p.first, p.second)= val;
        })
        .def("__getitem__", [](Matrix &m, std::pair<size_t,size_t> p){
            return m(p.first, p.second);
        })
        ;
}


