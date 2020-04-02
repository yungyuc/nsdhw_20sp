import _vector as agl
import numpy as np
import random as rd
import pytest as pt

ERROR_ALLOWED = 1e-4 #we assume that the result given by c++ and python are not exactly the same. So we allow an error 

def angle(u,v):
	mul_uv=np.dot(u,v)
	mul_u=np.dot(u,u)
	mul_v=np.dot(v,v)
	return np.arccos(mul_uv/(np.sqrt(mul_u*mul_v)))



############################ TEST ###############################

def test_basic():
	assert agl.calcul_angle([1,1],[2,2])==0

def test_basic2():
	assert agl.calcul_angle([1,1],[2,2])==angle([1,1],[2,2])

def test_basic3():
	assert agl.calcul_angle([1.1,1.2],[2.3,2.4])<=(angle([1.1,1.2],[2.3,2.4])+ERROR_ALLOWED) and agl.calcul_angle([1.1,1.2],[2.3,2.4])>=(angle([1.1,1.2],[2.3,2.4])-ERROR_ALLOWED)

def test_random():
	u=[rd.random()*100,rd.random()*100]
	v=[rd.random()*100,rd.random()*100]
	assert agl.calcul_angle(u,v)<=(angle(u,v)+ERROR_ALLOWED) and agl.calcul_angle(u,v)>=(angle(u,v)-ERROR_ALLOWED)

def test_pion2():
	assert agl.calcul_angle([0,1],[1,0])>=(np.pi/2-ERROR_ALLOWED) and agl.calcul_angle([0,1],[1,0])<=(np.pi/2+ERROR_ALLOWED)

def test_pi():
	assert agl.calcul_angle([0,1],[-1,0])>=(np.pi/2-ERROR_ALLOWED) and agl.calcul_angle([0,1],[-1,0])<=(np.pi/2+ERROR_ALLOWED)

def test_wronglength():
	with pt.raises(RuntimeError):
		agl.calcul_angle([1,2,3],[1,2])

def test_wronglength2():
	with pt.raises(RuntimeError):
		agl.calcul_angle([1,2],[])

def test_zerodivision():
	with pt.raises(RuntimeError):
		agl.calcul_angle([0,0],[0,0])



	
