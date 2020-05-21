import pytest
import numpy as np
import os
import time
import math

from _matrix import Matrix, multiply_naive, multiply_mkl, multiply_tile

def test_naive():
        m = np.random.randint(500, 1000)
        n = np.random.randint(500, 1000)
        k = np.random.randint(500, 1000)
        mat1 = np.random.random((m, k))
        mat2 = np.random.random((k, n))
        matA = Matrix(mat1)
        matB = Matrix(mat2)
        matC = multiply_naive(matA, matB)
        assert matC.nrow == m
        assert matC.ncol == n
        assert np.array(matC) == pytest.approx(np.matmul(mat1, mat2))

def test_mkl():
        m = np.random.randint(500, 1000)
        n = np.random.randint(500, 1000)
        k = np.random.randint(500, 1000)
        mat1 = np.random.random((m, k))
        mat2 = np.random.random((k, n))
        matA = Matrix(mat1)
        matB = Matrix(mat2)
        matC = multiply_mkl(matA, matB)
        assert matC.nrow == m
        assert matC.ncol == n
        assert np.array(matC) == pytest.approx(np.matmul(mat1, mat2))

def test_tile():
        tile = 16
        m = np.random.randint(500, 1000)
        n = np.random.randint(500, 1000)
        k = np.random.randint(500, 1000)
        mat1 = np.random.random((m, k))
        mat2 = np.random.random((k, n))
        matA = Matrix(mat1)
        matB = Matrix(mat2)
        matC = multiply_tile(matA, matB, tile)
        assert matC.nrow == m
        assert matC.ncol == n
        assert np.array(matC) == pytest.approx(np.matmul(mat1, mat2))

def test_performance():
    m = np.random.randint(500, 501)
    n = np.random.randint(500, 501)
    k = np.random.randint(500, 501)

    tile1 = 16
    tile2 = 17
    tile3 = 18

    mat1 = np.random.random((m, k))
    mat2 = np.random.random((k, n))
    matA = Matrix(mat1)
    matB = Matrix(mat2)

    timer = []

    start = time.time()
    matC_naive = multiply_naive(matA, matB)
    end = time.time()
    timer.append(end - start)

    start = time.time()
    matC_mkl = multiply_mkl(matA, matB)
    end = time.time()
    timer.append(end - start)

    start = time.time()
    matC_tile1 = multiply_tile(matA, matB, tile1)
    end = time.time()
    timer.append(end - start)

    start = time.time()
    matC_tile2 = multiply_tile(matA, matB, tile2)
    end = time.time()
    timer.append(end - start)

    start = time.time()
    matC_tile3 = multiply_tile(matA, matB, tile3)
    end = time.time()
    timer.append(end - start)

    with open('performance.txt', 'w') as f:
        print('Performance test for matrix multiplication', file=f)
        print('Matrix 1: {}x{}'.format(m, k), file=f)
        print('Matrix 2: {}x{}'.format(k, n), file=f)
        print('multiply_naive: {} s'.format(timer[0]), file=f)
        print('multiply_mkl: {} s'.format(timer[1]), file=f)
        print('multiply_tile with tile=16: {} s'.format(timer[2]), file=f)
        print('multiply_time with tile=17: {} s'.format(timer[3]), file=f)
        print('multiply_time with tile=18: {} s'.format(timer[4]), file=f)
