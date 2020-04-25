import numpy as np
import math
import time
import pytest

from _matrix import Matrix, multiply_naive, multiply_mkl

def test_multiply_naive():
    for i in range(3):
        m = np.random.randint(400, 1000)
        n = np.random.randint(400, 1000)
        k = np.random.randint(400, 1000)
        np_mat1 = np.random.random((m, k))
        np_mat2 = np.random.random((k, n))
        mat1 = Matrix(np_mat1)
        mat2 = Matrix(np_mat2)
        naive_ans = multiply_naive(mat1, mat2)

        assert naive_ans.nrow == m
        assert naive_ans.ncol == n
        assert np.array(naive_ans) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_multiply_mkl():
    for i in range(3):
        m = np.random.randint(400, 1000)
        n = np.random.randint(400, 1000)
        k = np.random.randint(400, 1000)
        np_mat1 = np.random.random((m, k))
        np_mat2 = np.random.random((k, n))
        mat1 = Matrix(np_mat1)
        mat2 = Matrix(np_mat2)
        mkl_ans = multiply_mkl(mat1, mat2)

        assert mkl_ans.nrow == m
        assert mkl_ans.ncol == n
        assert np.array(mkl_ans) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_performance():
    m = np.random.randint(1000, 2000)
    n = np.random.randint(1000, 2000)
    k = np.random.randint(1000, 2000)
    np_mat1 = np.random.random((m, k))
    np_mat2 = np.random.random((k, n))
    mat1 = Matrix(np_mat1)
    mat2 = Matrix(np_mat2)

    naive_timing = []
    for i in range(3):
        start = time.time()
        naive_ans = multiply_naive(mat1, mat2)
        end = time.time()
        naive_timing.append(end - start)

    mkl_timing = []
    for i in range(3):
        start = time.time()
        mkl_ans = multiply_mkl(mat1, mat2)
        end = time.time()
        mkl_timing.append(end - start)

    with open('performance.txt', 'w') as f:
        print('Performance Measurement', file=f)
        print('Input Matrix size:', file=f)
        print('Matrix 1: {} x {}'.format(m, k), file=f)
        print('Matrix 2: {} x {}'.format(k, n), file=f)
        print('Average Time for multiply_naive: {}'.format(np.mean(naive_timing)), file=f)
        print('Average Time for multiply_mkl: {}'.format(np.mean(mkl_timing)), file=f)
