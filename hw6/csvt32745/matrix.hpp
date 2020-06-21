#include <iostream>
#include <array>
#include <vector>
#include <sstream>
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

    Matrix(Matrix const & other, size_t start_r, size_t start_c, size_t size_r, size_t size_c)
      : m_nrow(size_r), m_ncol(size_c)
    {
        // std::cout << start_r << ", " << size_r << ", " << other.nrow() << "\n";
        // std::cout << start_c << ", " << size_c << ", " << other.ncol() << "\n";
        if(start_r + size_r > other.nrow() || start_c + size_c > other.ncol())
        {
            exit(EXIT_FAILURE);
        }

        reset_data(size_r, size_c);
        size_t ret_count = 0;
        for(size_t r = 0; r < size_r; ++r){
            // size_t base_idx = index(start_r + r, start_c);
            for(size_t c = 0; c < size_c; ++c){
                m_data[ret_count++] = other(start_r + r, start_c + c);
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
    size_t size() const {return m_nrow*m_ncol;}

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
                if((*this)(i,j) != other(i,j))
                    return false;
            }
        }
        return true;
    }

    Matrix operator+(Matrix const & other) const
    {
        auto ans = Matrix(other);
        double* data = ans.data();
        for(size_t i = 0; i < m_nrow*m_ncol; ++i)
            data[i] += m_data[i];
        return ans;
    }

    Matrix& operator+=(Matrix const & other)
    {
        double* data = other.data();
        for(size_t i = 0; i < m_nrow*m_ncol; ++i)
            m_data[i] += data[i];
        return *this;
    }

    /* 
        Methods
     */

    double get_data(size_t idx) const { return m_data[idx]; }
    double get_data(size_t r, size_t c) const { return m_data[index(r, c)]; }

    void set_data(size_t idx, double val) { m_data[idx] = val; }
    void set_data(size_t r, size_t c, double val) { m_data[index(r, c)] = val; }

    void set_block(const Matrix& mat, size_t start_r, size_t start_c, size_t size_r, size_t size_c) {
        if(start_r + size_r > m_nrow) exit(EXIT_FAILURE);
        if(start_c + size_c > m_ncol) exit(EXIT_FAILURE);

        size_t ret_count = 0;
        for(size_t r = 0; r < size_r; ++r){
            size_t base_idx = index(start_r + r, start_c);
            for(size_t c = 0; c < size_c; ++c){
                m_data[base_idx++] = mat.get_data(ret_count++);
            }
        }
    }

    std::vector<std::vector<Matrix>> split_block(size_t size_block) const {
        std::vector<std::vector<Matrix>> ans;
        
        size_t b_row = m_nrow / size_block;
        size_t b_col = m_ncol / size_block;
        // if(b_row == 0 && b_col == 0)
        //     return ans;
        
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

    std::string reprString() const{
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
        if(size > 0){
            m_data = new double[size]{0};
        } 
        else m_data = nullptr;

        m_nrow = nrow;
        m_ncol = ncol;
    }

    private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double* m_data = nullptr;
};


/* ========================
       Multiplication
   ========================*/

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



Matrix multiply_tile(const Matrix& a, const Matrix& b, size_t tile_size)
{
    // 2D Vec
    auto A = a.split_block(tile_size);
    auto B = b.split_block(tile_size);
    // std::cout << A.size() << ", " << A[0].size() << "\n";
    // std::cout << B.size() << ", " << B[0].size() << "\n";
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
            }
            ans.set_block(s, row_id, col_id, mm, nn);
            col_id += nn;
        }
        row_id += mm;
    }
    return ans;
}
