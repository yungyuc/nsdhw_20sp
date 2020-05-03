import _vector

import numpy as np
import math

def run_test(first, second, case):
    if np.linalg.norm(first) * np.linalg.norm(second) == 0:
        ans = 0
    else:
        ans = math.acos( np.dot(first, second) / (np.linalg.norm(first) * np.linalg.norm(second)) )
    calc = _vector.calc(first, second)
    # print(calc, ans)
    if case == 1:
        assert math.isnan(calc)
    else:
        assert abs(calc - ans) < 1e-5

def test_answer():
    run_test([0,0], [0,0], 1)
    run_test([1,2], [1,2], 2)
    run_test([1,0], [0,4], 3)
    run_test([1,2], [3,4], 4)

if __name__ == "__main__":
    test_answer()