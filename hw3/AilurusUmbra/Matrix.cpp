#include <stdexcept>

#include "Matrix.hpp"

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
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

Matrix &multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    double alpha = 1.0;
    double beta = 0.0;

    Matrix *mat = new Matrix(mat1.nrow(), mat2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,CblasNoTrans,
        mat->nrow(), mat->ncol(), mat1.ncol(),
        alpha,
        mat1.raw_data(), mat1.ncol(),
        mat2.raw_data(), mat2.ncol(),
        beta,
        mat->raw_data(), mat->ncol()
    );
    return *mat;
}