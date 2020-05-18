#define COL_MAJOR // this code use column major

#include "matrix.hpp"

#include <chrono>
#include <functional>
#include <iostream>

struct MatrixMultiplicationResults
{
    Matrix ret;
    std::chrono::duration<double> elapsed;
};
typedef struct MatrixMultiplicationResults MatrixMultiplicationResults;

int main(int argc, char ** argv)
{
    auto MeasureElapsed = [](Matrix &A, Matrix &B, std::function<Matrix(Matrix, Matrix)> func) {
        auto start = std::chrono::high_resolution_clock::now();
        Matrix C = func(A, B);
        auto finish = std::chrono::high_resolution_clock::now();        
        std::chrono::duration<double> elapsed = finish - start;
        return MatrixMultiplicationResults { C, elapsed };
    };

    auto MeasureElapsedCache = [](Matrix &A, Matrix &B, int CacheSize, std::function<Matrix(Matrix, Matrix, int)> func) {
        auto start = std::chrono::high_resolution_clock::now();
        Matrix C = func(A, B, CacheSize);
        auto finish = std::chrono::high_resolution_clock::now();        
        std::chrono::duration<double> elapsed = finish - start;
        return MatrixMultiplicationResults { C, elapsed };
    };

    const int configs[][2] = {{6, 8}, {32, 64}, {128, 512}, {1024, 1024}};
    for(int test = 0; test < 4; ++test) {
        
        const int randomCol = configs[test][0];
        const int randomRow = configs[test][1];

        std::cout << "\n\n>>> B(" << randomRow << "x"  << randomCol << ") times A(" << randomCol << "x" << randomRow << "):" << std::endl;

        if (test == 3) {
            std::cout << "(Testing Large Matrix, it might take longer than one minutes...)"  << std::endl;
        }

        std::vector<double> randomVector;
        for(long i = 0; i < randomCol * randomRow; ++i) {
            randomVector.push_back(i % 10);
        }

        Matrix mat5(randomRow, randomCol, randomVector);
        Matrix mat4(randomCol, randomRow, randomVector);
        
        // version from yungyuc
        Matrix mat6 = mat5 * mat4;

        // version from yanagi
        MatrixMultiplicationResults mat6_naive_results = MeasureElapsed(mat5, mat4, Matrix::multiply_naive);
        Matrix mat6_naive = mat6_naive_results.ret;
        
        MatrixMultiplicationResults mat6_mkl_results = MeasureElapsed(mat5, mat4, Matrix::multiply_mkl);
        Matrix mat6_mkl = mat6_mkl_results.ret;

        for(int cacheline = 8; cacheline <= 512; cacheline *= 2) {       

            MatrixMultiplicationResults mat6_tiling_results = MeasureElapsedCache(mat5, mat4, cacheline, Matrix::multiply_tiling);
            Matrix mat6_tiling = mat6_tiling_results.ret;

            const double epislon = 1e9;
            bool failed = false;
            for (int i = 0; i < randomRow && !failed; ++i) {
                for(int j = 0; j < randomRow && !failed; ++j) {
                    if (
                        abs(mat6(i, j) - mat6_naive(i, j)) < epislon && 
                        abs(mat6(i, j) - mat6_mkl(i, j)) < epislon && 
                        abs(mat6(i, j) - mat6_tiling(i, j)) < epislon 
                    ) {
                        continue;
                    }
                    else {
                        failed = true;
                        std::cout << "Error Index: " << i << " "  << j << std::endl;
                        std::cout << mat6(i, j) << " "  << mat6_naive(i, j) << " " << mat6_mkl(i, j) << " " << mat6_tiling(i, j)  << std::endl;
                        std::cout << "Result Does not Match!" << std::endl;
                        break;
                    }
                }
            }

            std::cout << "  Tiler " << 
                cacheline <<
                ", Time of (Naive/MKL/Tiling) = (" << 
                mat6_naive_results.elapsed.count() << "/" << 
                mat6_mkl_results.elapsed.count() << "/" << 
                mat6_tiling_results.elapsed.count() <<
                "), Tiling Speed up: " << 
                mat6_naive_results.elapsed.count() / mat6_tiling_results.elapsed.count() << 
                "x" <<
                std::endl;
        }
    }
    
    return 0;
}