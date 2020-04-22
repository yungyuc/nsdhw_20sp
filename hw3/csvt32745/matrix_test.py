import pytest
import unittest
import numpy as np
import numpy.random as rd
import math
from _matrix import Matrix, multiply_naive, multiply_mkl

class matrix_test(unittest.TestCase):
    def setUp(self):
        self.m = rd.randint(300, 500)
        self.n = rd.randint(100, 500)
        self.k = rd.randint(300, 500)

    def testCorrectnessRandom(self):
        a = Matrix(rd.random((self.m, self.k)))
        b = Matrix(rd.random((self.k, self.n)))
        self.assertTrue(np.allclose(multiply_naive(a, b), multiply_mkl(a,b)))

if __name__ == "__main__":
    unittest.main()