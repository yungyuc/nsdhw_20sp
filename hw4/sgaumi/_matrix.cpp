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

template<size_t N>
struct Block
{
    static constexpr const size_t NDIM = N;

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block<N> & operator= (double v)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block<N> & operator+= (Block<N> const & other)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    double m_buffer[N * N];
};

template<size_t N> void Block<N>::save(
    Matrix & mat, size_t it, size_t jt
)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat.m_buffer[base_t + j] = m_buffer[base_s + j];
        }
    }
}

template<size_t N>
struct Tiler
{
    static constexpr const size_t NDIM = N;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block<N> m_mat1; // row-major
    Block<N> m_mat2; // column-major
    Block<N> m_ret; // row-major
};

template<size_t N> void Tiler<N>::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
)
{
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat1[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_ret[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat2[j*NDIM + i] = m_ret[base + j];
        }
    }
}

template<size_t N> void Tiler<N>::multiply()
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
                v += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = v;
        }
    }
}

//util//
Matrix copy_fill0(Matrix const& m,size_t r,size_t c){
	
	Matrix ret(m.nrow()+r,m.ncol()+c);
	for(size_t i=0;i<m.nrow();i++){
		for(size_t j=0;j<m.ncol();j++){
			ret(i,j)=m(i,j);
		}
	}
	for(size_t i=0;i<r;i++){
		for(size_t j=0;j<c;j++){
			ret(m.nrow()+i,m.ncol()+j)=0;
		}
	}
	return ret;
}

Matrix suppr0(Matrix const& m,size_t r,size_t c){
	
	Matrix ret(m.nrow()-r,m.ncol()-c);
	for(size_t i=0;i<m.nrow()-r;i++){
		for(size_t j=0;j<m.ncol()-c;j++){
			ret(i,j)=m(i,j);
		}
	}
	return ret;
}
////


//Tiled matrix matrix multiplication.
Matrix multiply_tile(Matrix const & mat1_0, Matrix const & mat2_0, size_t size)
{

    //constexpr const size_t tsize = 64 / sizeof(double);
	constexpr const size_t tsize = size / sizeof(double);

	size_t r1 = tsize - (mat1_0.nrow()%tsize);
	size_t c1= tsize - (mat1_0.ncol()%tsize);
	size_t r2 = tsize - (mat2_0.nrow()%tsize);
	size_t c2= tsize - (mat2_0.ncol()%tsize);

	//work with matrix where nrow ncol are divider of tsize
	Matrix mat1 = copy_fill0(mat1_0,r1,c1);
	Matrix mat2 = copy_fill0(mat2_0,r2,c2);

	Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tsize;
    const size_t ntcol1 = ncol1 / tsize;
    const size_t ntcol2 = ncol2 / tsize;

    Block<tsize> value;
    Tiler<tsize> tiler;

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            value.save(ret, it, kt);
        }
    }

	Matrix ret_0 = suppr0(ret,r1,c2);
    return ret_0;
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


