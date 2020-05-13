import pytest
import unittest
import numpy.random as rd
import time 
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
import numpy as np


def multiply_naive_python(a,b):
	res = Matrix(a.nrow,b.ncol)
	for i in range(0,a.nrow):
		for j in range(0,b.ncol):
			s=0
			for k in range(0,a.ncol):
				s+=a[i,k]*b[k,j]
			res[i,j]=s
	return res


################## TEST ##########################
class test_python(unittest.TestCase):

	def test_matrix(self):
		row=rd.randint(1,1000)
		col=rd.randint(1,1000)
		test1=Matrix(row,col)
		test2=Matrix(test1)
		test3=Matrix(rd.random((row,col)))

		self.assertEqual(test1.nrow,row)
		self.assertEqual(test1.ncol,col)

		self.assertEqual(test2.nrow,row)
		self.assertEqual(test2.ncol,col)

		self.assertEqual(test3.nrow,row)
		self.assertEqual(test3.ncol,col)

		self.assertEqual(test1,test2)
		self.assertTrue(test1==test2)



	def test_multiplynaive(self):
		r=rd.randint(1,100)
		c=rd.randint(1,100)
		k=rd.randint(1,100)
		m1=Matrix(rd.random((r,k)))
		m2=Matrix(rd.random((k,c)))
		mulc=multiply_naive(m1,m2)
		mulp=multiply_naive_python(m1,m2)

		self.assertEqual(mulc.nrow,r)
		self.assertEqual(mulc.ncol,c)

		self.assertEqual(mulc,mulp)
		

	def test_multiplymkl(self):
		r=rd.randint(1,100)
		c=rd.randint(1,100)
		k=rd.randint(1,100)
		m1=Matrix(rd.random((r,k)))
		m2=Matrix(rd.random((k,c)))
		mulc=multiply_mkl(m1,m2)
		mulp=multiply_naive_python(m1,m2)

		self.assertEqual(mulc.nrow,r)
		self.assertEqual(mulc.ncol,c)

		for i in range(0,r):
			for j in range(0,c):
				self.assertTrue(np.allclose(mulc[i,j],mulp[i,j]))


	def test_multiplytile(self):
		r=rd.randint(1,100)
		c=rd.randint(1,100)
		k=rd.randint(1,100)
		m1=Matrix(rd.random((r,k)))
		m2=Matrix(rd.random((k,c)))
		mulc=multiply_tile(m1,m2)
		mulp=multiply_naive_python(m1,m2)

		self.assertEqual(mulc.nrow,r)
		self.assertEqual(mulc.ncol,c)

		for i in range(0,r):
			for j in range(0,c):
				print("i={},j={} : tile : {} == naive : {}".format(i,j,mulc[i,j],mulp[i,j]))
				self.assertTrue(np.allclose(mulc[i,j],mulp[i,j]))



	def test_performancemul(self):
		r=rd.randint(1000,1500)
		c=rd.randint(1000,1500)
		k=rd.randint(1000,1500)
		m1=Matrix(rd.random((r,k)))
		m2=Matrix(rd.random((k,c)))
		timer=[]
		i=0

		timer_start=time.time()
		mulc_naive=multiply_naive(m1,m2)
		timer_stop=time.time()
		timer.append(timer_stop-timer_start)
		i_mulcnaive=i
		i+=1

#		timer_start=time.time()
#		mulp_naive=multiply_naive_python(m1,m2)
#		timer_stop=time.time()
#		timer.append(timer_stop-timer_start)
#		i_mulpnaive=i
#		i+=1

		timer_start=time.time()
		mulc_mkl=multiply_mkl(m1,m2)
		timer_stop=time.time()
		timer.append(timer_stop-timer_start)
		i_mulcmkl=i
		i+=1

		timer_start=time.time()
		mulc_mkl=multiply_tile(m1,m2)
		timer_stop=time.time()
		timer.append(timer_stop-timer_start)
		i_mulctile=i
		i+=1

		runtime=timer[i_mulctile]/timer[i_mulcnaive]*100.
		speedup=(1.-timer[i_mulctile]/timer[i_mulcnaive])*100.
		with open('performance.txt','w') as fil:
			fil.write("matrix 1 : {} rows {} columns\n".format(r,k))
			fil.write("matrix 2 : {} rows {} columns\n".format(k,c))
			fil.write("Performance test for matrix multiplication:\n")
			fil.write("multiply_naive : {} seconds\n".format(timer[i_mulcnaive]))
			fil.write("multiply_mkl : {} seconds\n".format(timer[i_mulcmkl]))
			fil.write("multiply_tile : {} seconds\n".format(timer[i_mulctile]))
			#fil.write("( multiply_naive_python : {} seconds )\n".format(timer[i_mulpnaive]))
			fil.write("speedup of {}% from naive to tile multiplication\n".format(speedup))
			fil.write("(runtime for tile multiplication is {}% of naive multiplication one)\n".format(runtime))
			fil.close()		
	






