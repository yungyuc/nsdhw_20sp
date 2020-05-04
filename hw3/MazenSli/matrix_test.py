import unittest
import pytest
import numpy as np
import os
import time

from _matrix import Matrix, multiply_naive, multiply_mkl

class matrix_test(unittest.TestCase):
    
    def naive_mkl(self):        
        m, n, k = np.random.randint(1, 3, size=3)
        A = np.random.randn(m, k)
        B = np.random.randn(k, n)            

        A_mat = _matrix.Matrix(m, k, A.flatten(order='C'))
        B_mat = _matrix.Matrix(k, n, B.flatten(order='C'))

        C_naive = _matrix.multiply_naive(A, B)
        C_mkl = _matrix.multiply_mkl(A, B)

        assert np.array(C_naive) == pytest.approx(np.matmul(A, B))
        assert np.array(C_mkl) == pytest.approx(np.matmul(A, B))


if __name__ == "__main__":
    unittest.main()
