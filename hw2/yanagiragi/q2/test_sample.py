import example

import numpy as np
import math

def test_answer():
    first = [1,2]
    second = [3,4]
    diff = np.array(first) - np.array(second)
    diff = diff / np.linalg.norm(diff)
    assert abs(example.calc(first, second) - math.atan2(diff[1], diff[0])) < 0.0001