import _vector

import numpy

a = _vector.Matrix(2,2,[1,2,3,4])
b = _vector.Matrix(2,2,[1,2,3,4])
c = _vector.multiply_mkl(a, b)

a1 = numpy.array([[1,3],[2,4]])
b1 = numpy.array([[1,3],[2,4]])

print(c)
print(numpy.matmul(a1, b1))