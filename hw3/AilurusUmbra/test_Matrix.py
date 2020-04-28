from _matrix import Matrix
from _matrix import multiply_naive, multiply_mkl
import pytest
import numpy as np
import time

def get_rand_float(low=0, high=10, decimal=1):
  return np.random.randint(low, high, size=(1,1)).astype(float)

def get_rand_dim(low=300, high=500):
  m = np.random.randint(low, high)
  n = np.random.randint(low, high)
  k = np.random.randint(low, high)
  return m, n, k

def compareMat(mat1, mat2, m, n):
  for i in range(m):
    for j in range(n):
      if mat1[i,j]!= mat2[i,j]:
        print(f'm1[i,j]:{mat1[i,j]}')
        return False
  return True

# ============
# === Test ===
# ============

def test_constructor():
  m, n, _ = get_rand_dim()
  m1 = Matrix(m, n)
  m2 = Matrix(m, n)
  for i in range(m):
    for j in range(n):
      m1[i,j] = 666.666
      m2[i,j] = 666.666

  assert(compareMat(m1, m2, m, n))

def test_mat_dim():
  m, n, _ = get_rand_dim()
  m1 = Matrix(m, n)
  assert(m==m1.nrow)
  assert(n==m1.ncol)

def test_multiply():
  
  m, n, k = get_rand_dim()
  m1 = Matrix(m, k)
  m2 = Matrix(k, n)

  
  for i in range(m):
    for j in range(k):
      m1[i,j] = get_rand_float() 

  for i in range(k):
    for j in range(n):
      m2[i,j] = get_rand_float()
 

  
  with open('performance.txt', 'w') as fobj:
    repeat = 5
   
    # naive 

    naive_time_diff = []
    for i in range(repeat):
      start = time.time()
      m3 = multiply_naive(m1, m2)
      end = time.time()
      naive_time_diff.append(end-start)
    naivesec = np.mean(naive_time_diff)

    # mkl 

    mkl_time_diff = []
    for i in range(repeat):
      start = time.time()
      m4 = multiply_mkl(m1, m2)
      end = time.time()
      mkl_time_diff.append(end-start)
    mklsec = np.mean(mkl_time_diff)

    # write to performance.txt

    fobj.write('in_test\n')
    fobj.write(f'Start multiply_naive (repeat={repeat}), take avg = ')
    fobj.write(f'{naivesec} seconds\n')

    fobj.write(f'Start multiply_mkl (repeat={repeat}), take avg = ')
    fobj.write(f'{mklsec} seconds\n')

    fobj.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))
  
  assert(compareMat(m3, m4, m, n))
