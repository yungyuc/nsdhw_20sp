import _matrix
import numpy as np
import os
import time

def measure_time(m, k, n):
    
    testIter = 10
    elapsed_naive = 0
    elapsed_mkl = 0

    for idx in range(testIter):
        
        a_np = np.random.randn(m, k)
        b_np = np.random.randn(k, n)            

        a_mat = _matrix.Matrix(m, k, a_np.flatten(order='C')) # row major, C for C-style
        b_mat = _matrix.Matrix(k, n, b_np.flatten(order='C'))
        
        start = time.time()
        c_mat_naive = _matrix.multiply_naive(a_mat, b_mat)
        end = time.time()
        elapsed_naive += end - start

        a_np = np.random.randn(m, k)
        b_np = np.random.randn(k, n)            

        a_mat = _matrix.Matrix(m, k, a_np.flatten(order='C')) # row major, C for C-style
        b_mat = _matrix.Matrix(k, n, b_np.flatten(order='C'))

        start = time.time()
        c_mat_mkl = _matrix.multiply_mkl(a_mat, b_mat)
        end = time.time()
        elapsed_mkl += end - start

    with open('performance.txt', 'a') as f:
        f.write('({}, {}, {}) Naive: {} sec, MKL: {} sec\n'.format(m, k, n, elapsed_naive, elapsed_mkl))

def test_answer():
    
    config = [[10, 11, 12], [100, 101, 102], [500, 501, 502]] # mkl is faster when matrix is bigger    
    for m, k, n in config:
        measure_time(m, k, n)

    totalErr = 0.0
    testIter = 100

    elapsed_naive = 0
    elapsed_mkl = 0

    for idx in range(testIter):
        m, n, k = np.random.randint(1, 10, size=3)
        
        a_np = np.random.randn(m, k)
        b_np = np.random.randn(k, n)            

        c_np = np.matmul(a_np, b_np, order='C')
        a_mat = _matrix.Matrix(m, k, a_np.flatten(order='C')) # row major, C for C-style
        b_mat = _matrix.Matrix(k, n, b_np.flatten(order='C'))
        
        start = time.time()
        c_mat_naive = _matrix.multiply_naive(a_mat, b_mat)
        end = time.time()
        elapsed_naive += end - start

        start = time.time()
        c_mat_mkl = _matrix.multiply_mkl(a_mat, b_mat)
        end = time.time()
        elapsed_mkl += end - start

        for i in range(m):
            for j in range(n):
                assert abs( c_np[i,j] - c_mat_naive[i, j] ) < 0.001, '{} {}'.format(c_np[i,j], c_mat_naive[i, j])
                assert abs( c_np[i,j] - c_mat_mkl[i, j] ) < 0.001

        """
        print('A = ', a_np)
        print('B = ', b_np)
        print('Result From Numpy = ', c_np)
        print('Result From Naive = ', c_mat_naive)
        print('Result From MKL   = ', c_mat_mkl)
        """

if __name__ == "__main__":
    
    test_answer()