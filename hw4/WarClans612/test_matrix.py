import numpy as np
import math
import time
import pytest

from _matrix import Matrix, multiply_naive, multiply_mkl, multiply_tile

def test_multiply_naive():
    for i in range(2):
        m = np.random.randint(200, 400)
        n = np.random.randint(200, 400)
        k = np.random.randint(200, 400)
        np_mat1 = np.random.random((m, k))
        np_mat2 = np.random.random((k, n))
        mat1 = Matrix(np_mat1)
        mat2 = Matrix(np_mat2)
        naive_ans = multiply_naive(mat1, mat2)

        assert naive_ans.nrow == m
        assert naive_ans.ncol == n
        assert np.array(naive_ans) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_multiply_mkl():
    for i in range(2):
        m = np.random.randint(200, 400)
        n = np.random.randint(200, 400)
        k = np.random.randint(200, 400)
        np_mat1 = np.random.random((m, k))
        np_mat2 = np.random.random((k, n))
        mat1 = Matrix(np_mat1)
        mat2 = Matrix(np_mat2)
        mkl_ans = multiply_mkl(mat1, mat2)

        assert mkl_ans.nrow == m
        assert mkl_ans.ncol == n
        assert np.array(mkl_ans) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_multiply_tile():
    tsize_l = [16, 17, 19]
    for tsize in tsize_l:
        for i in range(2):
            m = np.random.randint(200, 400)
            n = np.random.randint(200, 400)
            k = np.random.randint(200, 400)
            np_mat1 = np.random.random((m, k))
            np_mat2 = np.random.random((k, n))
            mat1 = Matrix(np_mat1)
            mat2 = Matrix(np_mat2)
            tile_ans = multiply_tile(mat1, mat2, tsize)

            assert tile_ans.nrow == m
            assert tile_ans.ncol == n
            assert np.array(tile_ans) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_performance():
    m = np.random.randint(500, 501)
    n = np.random.randint(500, 501)
    k = np.random.randint(500, 501)
    np_mat1 = np.random.random((m, k))
    np_mat2 = np.random.random((k, n))
    mat1 = Matrix(np_mat1)
    mat2 = Matrix(np_mat2)

    naive_timing = []
    for i in range(2):
        start = time.time()
        naive_ans = multiply_naive(mat1, mat2)
        end = time.time()
        naive_timing.append(end - start)

    mkl_timing = []
    for i in range(2):
        start = time.time()
        mkl_ans = multiply_mkl(mat1, mat2)
        end = time.time()
        mkl_timing.append(end - start)

    tile16_timing = []
    for i in range(2):
        start = time.time()
        tile16_ans = multiply_tile(mat1, mat2, 16)
        end = time.time()
        tile16_timing.append(end - start)

    tile17_timing = []
    for i in range(2):
        start = time.time()
        tile17_ans = multiply_tile(mat1, mat2, 17)
        end = time.time()
        tile17_timing.append(end - start)

    tile19_timing = []
    for i in range(2):
        start = time.time()
        tile19_ans = multiply_tile(mat1, mat2, 19)
        end = time.time()
        tile19_timing.append(end - start)

    naive_timing = np.mean(naive_timing)
    mkl_timing = np.mean(mkl_timing)
    tile16_timing = np.mean(tile16_timing)
    tile17_timing = np.mean(tile17_timing)
    tile19_timing = np.mean(tile19_timing)

    with open('performance.txt', 'w') as f:
        print('Performance Measurement', file=f)
        print('Input Matrix size:', file=f)
        print('Matrix 1: {} x {}'.format(m, k), file=f)
        print('Matrix 2: {} x {}'.format(k, n), file=f)
        print('Average Time for multiply_naive: {}'.format(naive_timing), file=f)
        print('Average Time for multiply_mkl: {}'.format(mkl_timing), file=f)
        print('Average Time for multiply_tile16: {}'.format(tile16_timing), file=f)
        print('Average Time for multiply_tile17: {}'.format(tile17_timing), file=f)
        print('Average Time for multiply_tile19: {}'.format(tile19_timing), file=f)

    assert tile16_timing / naive_timing <= 0.8
    assert tile17_timing / naive_timing <= 0.8
    assert tile19_timing / naive_timing <= 0.8
