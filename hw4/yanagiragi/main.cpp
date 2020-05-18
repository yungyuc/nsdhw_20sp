#define COL_MAJOR // this code use column major

#include "matrix.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
    Matrix mat1(2, 3, std::vector<double>{1, 2, 3, 4, 5, 6});
    Matrix mat2(3, 2, std::vector<double>{1, 2, 3, 4, 5, 6});

    // version from yungyuc
    Matrix mat3 = mat1 * mat2;

    // version from yanagi
    Matrix mat3_naive = Matrix::multiply_naive(mat1, mat2);
    Matrix mat3_mkl = Matrix::multiply_mkl(mat1, mat2);

    std::cout << ">>> A(2x3) times B(3x2):" << std::endl;    
    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "result matrix C (2x2) = AB:" << mat3 << std::endl;
    std::cout << "result matrix C (2x2) From Naive = AB:" << mat3_naive << std::endl;
    std::cout << "result matrix C (2x2) From MKL   = AB:" << mat3_mkl << std::endl;

    // version from yungyuc
    Matrix mat4 = mat2 * mat1;

    // version from yanagi
    Matrix mat4_naive = Matrix::multiply_naive(mat2, mat1);
    Matrix mat4_mkl = Matrix::multiply_mkl(mat2, mat1);

    std::cout << ">>> B(3x2) times A(2x3):" << std::endl;    
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "result matrix D (3x3) = BA:" << mat4 << std::endl;
    std::cout << "result matrix D (3x3) From Naive = BA:" << mat4_naive << std::endl;
    std::cout << "result matrix D (3x3) From MKL   = BA:" << mat4_mkl << std::endl;

    return 0;
}