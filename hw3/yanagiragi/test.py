import _matrix
import numpy as np

"""m, n, k = 6, 2, 5
A = np.array([[ 1.99879734,  0.08099073],
 [ 0.43733627,  1.64902958],
 [-1.7891116  , 0.66508498],
 [-0.30614268 ,-0.91738258],
 [ 0.93398375 ,-0.71693931],
 [-0.81638957 ,-0.22198592]])

B =  np.array([[-0.61751253,  1.11739029, -0.42299574, -0.41390509,  1.09480818], [-1.1671636,  -1.06161186,  0.1171699,  -0.83901581, -0.60271623]])

A = np.random.randint(1, 10, size=(m, n))
B = np.random.randint(1, 10, size=(n, k))

print(A.shape, B.shape)

a_mat = _matrix.Matrix(m, n, A.flatten(order='C')) # col major, F for fortran-style
b_mat = _matrix.Matrix(n, k, B.flatten(order='C')) # col major, F for fortran-style
c_mat_naive = _matrix.multiply_naive(a_mat, b_mat)
c_mat_mkl = _matrix.multiply_mkl(a_mat, b_mat)
c_np = np.matmul(A, B)
err = 0

for i in range(m):
    for j in range(k):
        err += c_np[i,j] - c_mat_naive[i, j]
        err += c_np[i,j] - c_mat_mkl[i, j]

print(m, n, k, err)
print('A = ', A)
print( A.flatten(order='C'))
print('A2 = ', a_mat)
print('B = ', B)
print('Result From Numpy = ', c_np)
print('Result From Naive = ', c_mat_naive)
print('Result From MKL   = ', c_mat_mkl)"""

def test_answer():
    totalErr = 0.0
    testIter = 10
    for idx in range(testIter):
        m, n, k = np.random.randint(1, 10, size=3)
        
        a_np = np.random.randn(m, n)
        b_np = np.random.randn(n, k)
        c_np = np.matmul(a_np, b_np)

        a_mat = _matrix.Matrix(m, n, a_np.flatten(order='C')) # row major, C for C-style
        b_mat = _matrix.Matrix(n, k, b_np.flatten(order='C')) # row major, C for C-style
        c_mat_naive = _matrix.multiply_naive(a_mat, b_mat)
        c_mat_mkl = _matrix.multiply_mkl(a_mat, b_mat)

        err = 0
        for i in range(m):
            for j in range(k):
                err += c_np[i,j] - c_mat_naive[i, j]
                err += c_np[i,j] - c_mat_mkl[i, j]
        
        totalErr += err
        print('Test {}: ({}x{}, {}x{}), err = {}'.format(idx, m, n, n, k, err))
        
        """
        print('A = ', a_np)
        print('B = ', b_np)
        print('Result From Numpy = ', c_np)
        print('Result From Naive = ', c_mat_naive)
        print('Result From MKL   = ', c_mat_mkl)
        """

    print('Avg err = {}'.format(totalErr / float(testIter)))
    
    assert (totalErr / float(testIter)) > 0

if __name__ == "__main__":
    test_answer()