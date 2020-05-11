import pytest
import unittest
import numpy as np
import numpy.random as rd
import math
import time
from _matrix import Matrix, multiply_naive, multiply_mkl

class matrix_test(unittest.TestCase):
    def setUp(self):
        self.m = rd.randint(300, 500)
        self.n = rd.randint(100, 500)
        self.k = rd.randint(300, 500)

    def testMultiplication(self):
        print(" size m={}, k={}, n={} ".format(self.m, self.k, self.n))
        a = Matrix(rd.random((self.m, self.k)))
        b = Matrix(rd.random((self.k, self.n)))
        ans_nav = multiply_naive(a, b)
        ans_mkl = multiply_mkl(a, b)

        self.assertEqual(ans_nav.nrow, self.m)
        self.assertEqual(ans_nav.ncol, self.n)
        self.assertEqual(ans_mkl.nrow, self.m)
        self.assertEqual(ans_mkl.ncol, self.n)

        # self.assertEqual(ans_nav, ans_mkl)
        self.assertTrue(np.allclose(ans_nav, ans_mkl))

    def testConstructor(self):
        a = Matrix(rd.random((self.m, self.m)))
        b = Matrix(self.m, self.n)
        c = Matrix(a)

        self.assertEqual(a.nrow, a.ncol)
        self.assertEqual(a.nrow, b.nrow)
        self.assertEqual(a.nrow, c.nrow)
        self.assertEqual(a.ncol, c.ncol)
        self.assertEqual(a, c)
        self.assertNotEqual(id(a), id(c))

    def testTime(self):
        m = 1234
        n = 1234
        k = 1234
        a = Matrix(rd.random((m, k)))
        b = Matrix(rd.random((k, n)))

        timer_a = []
        timer_b = []
        for i in range(5):
            timer = time.time()
            ans_nav = multiply_naive(a, b)
            timer_a.append(time.time() - timer)

            timer = time.time()
            ans_mkl = multiply_mkl(a,b)
            timer_b.append(time.time() - timer)
            self.assertTrue(np.allclose(ans_nav, ans_mkl))
        
        with open("performance.txt", "w") as f:
            f.write("Average time measured for 5 times\n")
            f.write("size: m={} k={} n={}\n".format(m, k, n))
            f.write("multiply_naive: {} secs\n".format("%.4f"%np.average(timer_a)))
            f.write("multiply_mkl: {} secs\n".format("%.4f"%np.average(timer_b)))
            f.close()

        

if __name__ == "__main__":
    unittest.main()