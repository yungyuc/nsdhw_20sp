import _matrix
import numpy as np
import os
import time
import pytest, unittest

def measure_time(m, k, n):    
    
    testIter = 10
    elapsed_naive = 0
    elapsed_mkl = 0
    elapsed_tile = 0

    for idx in range(testIter):        
        a_np = np.random.randn(m, k)
        b_np = np.random.randn(k, n)            

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

        start = time.time()
        # c_mat_tile = _matrix.multiply_tile(a_mat, b_mat, 64)
        end = time.time()
        elapsed_tile += end - start

    with open('performance.txt', 'a') as f:
        f.write('({}, {}, {}) Naive: {} sec, MKL: {} sec, Tile: {} sec\n'.format(m, k, n, elapsed_naive, elapsed_mkl, elapsed_tile))
        f.write('Tiling Speedup: {}x\n'.format(elapsed_naive / elapsed_tile))

class matrix_test(unittest.TestCase):
    
    def test_performance(self):
        config = [[10, 11, 12], [100, 101, 102], [500, 501, 502]] # mkl is faster when matrix is bigger    
        for m, k, n in config:
            measure_time(m, k, n)

    def test_answer(self):        
        testIter = 100
        for idx in range(testIter):
            
            m, n, k = np.random.randint(1, 10, size=3)
            
            a_np = np.random.randn(m, k)
            b_np = np.random.randn(k, n)            

            c_np = np.matmul(a_np, b_np, order='C')
            a_mat = _matrix.Matrix(m, k, a_np.flatten(order='C')) # row major, C for C-style
            b_mat = _matrix.Matrix(k, n, b_np.flatten(order='C'))
            
            c_mat_naive = _matrix.multiply_naive(a_mat, b_mat)
            c_mat_mkl = _matrix.multiply_mkl(a_mat, b_mat)
            c_mat_tile = _matrix.multiply_tile(a_mat, b_mat, 64)
            #print(c_mat_tile)
            
            for i in range(m):
                for j in range(n):
                    self.assertTrue( abs( c_np[i,j] - c_mat_naive[i, j] ) < 0.001 )
                    self.assertTrue( abs( c_np[i,j] - c_mat_mkl[i, j] ) < 0.001 )
                    #print(c_np[i,j], c_mat_tile[i, j])
                    self.assertTrue( abs( c_np[i,j] - c_mat_tile[i, j] ) < 0.001 )


if __name__ == "__main__":
    unittest.main()