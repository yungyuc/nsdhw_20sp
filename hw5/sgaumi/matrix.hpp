#include <iostream>
#include <vector>
/*#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
#include "pybind11/numpy.h"*/
//#include </home/asministrateur/intel/compilers_and_libraries_2020.1.217/linux/mkl/include/mkl.h> 
#include "mkl.h"


namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow,ncol);
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

	Matrix(Matrix const & other, size_t start_r, size_t start_c, size_t size_r, size_t size_c)
      : m_nrow(size_r), m_ncol(size_c)
    {
        if(start_r + size_r > other.nrow() || start_c + size_c > other.ncol())
        {
            exit(EXIT_FAILURE);
        }

        reset_buffer(size_r, size_c);
        size_t ret_count = 0;
        for(size_t r = 0; r < size_r; ++r){
            for(size_t c = 0; c < size_c; ++c){
                m_buffer[ret_count++] = other(start_r + r, start_c + c);
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

	bool operator==(Matrix const & other) const
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

	Matrix operator+(Matrix const & other) const
    {
        auto ans = Matrix(other);
        double* buff = ans.buff();
        for(size_t i = 0; i < m_nrow*m_ncol; ++i)
            buff[i] += m_buffer[i];
        return ans;
    }

    Matrix& operator+=(Matrix const & other)
    {
        double* buff = other.buff();
        for(size_t i = 0; i < m_nrow*m_ncol; ++i)
            m_buffer[i] += buff[i];
        return *this;
    }

	
	//

    ~Matrix()
    {
        //delete[] m_buffer;
		reset_buffer(0,0);
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }
	
	void set(size_t i, double v){m_buffer[i]=v;}
	void set(size_t i, size_t j, double v){m_buffer[ind(i,j)]=v;}
	double get(size_t i) const {return m_buffer[i];}
	double get(size_t i,size_t j ) const {return m_buffer[ind(i,j)];}
	

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

/////////////
	size_t ind(size_t r,size_t c) const{return r*m_ncol+c;}

	void set_block(const Matrix& mat, size_t start_r, size_t start_c, size_t size_r, size_t size_c) {
        if(start_r + size_r > m_nrow) exit(EXIT_FAILURE);
        if(start_c + size_c > m_ncol) exit(EXIT_FAILURE);

        size_t ret_count = 0;
        for(size_t r = 0; r < size_r; ++r){
            size_t base_idx = ind(start_r + r, start_c);
			//size_t base_idx = (start_r+r)*m_ncol+start_c;
            for(size_t c = 0; c < size_c; ++c){
                m_buffer[base_idx++] = mat.get(ret_count++);
            }
        }
    }

    std::vector<std::vector<Matrix>> split_block(size_t size_block) const {
        std::vector<std::vector<Matrix>> ans;
        
        size_t b_row = m_nrow / size_block;
        //size_t b_col = m_ncol / size_block;
        
        for(size_t i = b_row + 1, r = 0; i > 0; --i, r += size_block){
            
            size_t size_r = size_block;
            if(i == 1){
                // check remaining row
                if(m_nrow - r > 0){
                    size_r = m_nrow - r;
                }
                else break;
            }

            // process column
            std::vector<Matrix> vec;
            int c;
            int cond_c = m_ncol - size_block; // c + size_block < m_ncol
            for(c = 0; c < cond_c; c += size_block){
                vec.push_back(Matrix(*this, r, c, size_r, size_block));
            }
            // remaining col
            if(m_ncol - c > 0){
                vec.push_back(Matrix(*this, r, c, size_r, m_ncol - c));
            }
            ans.push_back(vec);
        }

        return ans;
    }

////////////
	void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement>0) { m_buffer = new double[nelement]{0}; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }


private:

	
    size_t m_nrow=0;
    size_t m_ncol=0;
    double * m_buffer=nullptr;

};

Matrix multiply_naive(const Matrix& m1, const Matrix& m2){
	//size_t n1=m1.nrow();
	//size_t n2=m2.ncol();
	auto res = Matrix(m1.nrow(),m2.ncol());
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


/*Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t s)
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
}*/

////////////////HW4(end)///////////////////////

////////////////HW5////////////////////////////

Matrix multiply_tile(const Matrix& a, const Matrix& b, size_t tile_size)
{
    // 2D Vec
    auto A = a.split_block(tile_size);
    auto B = b.split_block(tile_size);

    auto ans = Matrix(a.nrow(), b.ncol());

    size_t m = A.size();
    size_t k = B.size();
    size_t n = B[0].size();

    // Block Multiplication
    for(size_t r = 0, row_id = 0; r < m; r++){
        size_t mm = A[r][0].nrow();
        
        for(size_t c = 0, col_id = 0; c < n; c++){
            size_t nn = B[0][c].ncol();
            Matrix s(mm, nn);
            
            for(size_t i = 0; i < k; i++){
                s += multiply_naive(A[r][i], B[i][c]);
				//s+=multiply_mkl(A[r][i], B[i][c]);
            }
            ans.set_block(s, row_id, col_id, mm, nn);
            col_id += nn;
        }
        row_id += mm;
    }
    return ans;
}

///////////////////////////////////////////////



/*
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
        .def("__repr__", [](Matrix const& m){
            for (size_t i=0;i<m.nrow();i++){
				for (size_t j=0;j<m.ncol();j++){
					std::cout<<m(i,j)<<" ";
				}
				std::cout<<std::endl;
			}
        })
		.def("__setitem__", [](Matrix &m, std::pair<size_t,size_t> p, double val){
            m(p.first, p.second)= val;
        })
        .def("__getitem__", [](Matrix &m, std::pair<size_t,size_t> p){
            return m(p.first, p.second);
        })
        ;
}*/


