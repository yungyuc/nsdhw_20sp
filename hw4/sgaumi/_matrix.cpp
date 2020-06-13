#include <iostream>
#include <stdlib.h>
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

	void display(){
		for (size_t i=0;i<m_nrow;i++){
			for (size_t j=0;j<m_ncol;j++){
				std::cout<<(*this)(i,j)<<" ";
			}
			std::cout<<std::endl;
		}
	}


public:

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

/////////////HW4///////////////////////


Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t s)
{	

	const size_t tsize = s;


	Matrix ret(mat1.nrow(), mat2.ncol());
	for(size_t ii=0;ii<mat1.nrow();ii++){
		for(size_t jj=0;jj<mat2.ncol();jj++){
			ret(ii,jj)=0;
		}
	}	

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t ncol2 = mat2.ncol();

    size_t ntrow1 = nrow1 / tsize;
	if(nrow1%tsize!=0)ntrow1++;
    size_t ntcol1 = ncol1 / tsize;
	if(ncol1%tsize!=0)ntcol1++;
    size_t ntcol2 = ncol2 / tsize;
	if(ncol2%tsize!=0)ntcol2++;


    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t jt=0; jt<ntcol2; ++jt)
        {
			for(size_t kt=0;kt<ntcol1;kt++){

				for (size_t i=0;i<tsize;i++){
					for(size_t j=0;j<tsize;j++){
						for(size_t k=0;k<tsize;k++){
							if((it*tsize+i<nrow1) && (jt*tsize+j<ncol2) && (kt*tsize+k<ncol1)){
								ret(it*tsize+i,jt*tsize+j)+=mat1(it*tsize+i,kt*tsize+k)*mat2(kt*tsize+k,jt*tsize+j);
							}
						}

					}
				}

			}
        }
    }



	

    return ret;
}

////////////////HW4(end)///////////////////////




PYBIND11_MODULE(_matrix, m){
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
	m.def("multiply_tile", &multiply_tile);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init([](size_t r, size_t c) {
			return new Matrix(r, c);
		}))
        .def(py::init<Matrix&>())
        .def(py::init<std::vector<std::vector<double>>&>())
		.def("display", &Matrix::display)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        /*.def("__repr__", [](Matrix const& m){
            for (size_t i=0;i<m.nrow();i++){
				for (size_t j=0;j<m.ncol();j++){
					std::cout<<m(i,j)<<" ";
				}
				std::cout<<std::endl;
			}
        })*/
		.def("__setitem__", [](Matrix &m, std::pair<size_t,size_t> p, double val){
            m(p.first, p.second)= val;
        })
        .def("__getitem__", [](Matrix &m, std::pair<size_t,size_t> p){
            return m(p.first, p.second);
        })
        ;
}


