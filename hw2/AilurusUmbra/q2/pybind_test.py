from angle import _vector, get_angle
import pytest
import numpy as np
from numpy.linalg import norm
import math

def test_invalid_3():
    with pytest.raises(Exception):
        _vector(3, 3, 2);

def test_invalid_1():
    with pytest.raises(Exception):
        _vector(1);

def test_invalid_0():
    with pytest.raises(Exception):
        _vector();

def test_zero_vecotr():
    p1 = _vector(0, 0)
    p2 = _vector(0, 1)
    p3 = _vector(1, 0)
    r_angle1 = get_angle(p1, p2)
    assert math.isclose(r_angle1, 0, abs_tol=1e-06)
    r_angle2 = get_angle(p1, p3) 
    assert math.isclose(r_angle2, 0, abs_tol=1e-06)

def test_parallel():
    coef = np.random.randint(1, 5)
    x, y = np.random.randint(1, 5, 2)
    
    # test 0 degree
    p1 = _vector(x, y)
    p2 = _vector(x*coef, y*coef)
    r_angle1 = get_angle(p1, p2)
    assert math.isclose(r_angle1, 0, abs_tol=1e-06)
   
    # test 180 degree
    p3 = _vector(x*coef*(-1), y*coef*(-1)) 
    r_angle2 = get_angle(p1, p3) 
    assert math.isclose(r_angle2, math.pi, abs_tol=1e-06)

def test_45_degree():
    p1 = _vector(1, 1)
    p2 = _vector(0, 1)
    r_angle = get_angle(p1, p2)
    assert math.isclose(r_angle, math.pi/4, abs_tol=1e-06)

def test_90_degree():
    p1 = _vector(1, 0)
    p2 = _vector(0, 1)
    r_angle = get_angle(p1, p2)
    assert math.isclose(r_angle, math.pi/2, abs_tol=1e-06)

