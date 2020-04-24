#include "matrix.hpp"

#include <mkl.h>

std::ostream & operator<< (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}

Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}


// Self Implementation starts here

Matrix Matrix::multiply_naive (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }

    Matrix ret(left.nrow(), right.ncol());

    for(int i = 0; i < ret.nrow(); ++i) {
        for (int j = 0; j < ret.ncol(); ++j) {
            double v = 0;
            for(int k = 0; k < left.ncol(); ++k) {
                v += left(i, k) * right(k, j);
            }
            ret(i, j) = v;
        }
    }

    return ret;
}

Matrix Matrix::multiply_mkl (const Matrix &left, const Matrix& right) 
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