import _vector

import numpy as np
import math

def test_answer():
    first = [1,2]
    second = [3,4]
    ans = math.acos( np.dot(first, second) / (np.linalg.norm(first) * np.linalg.norm(second)) )
    err = abs(_vector.calc(first, second) - ans)
    print('Error = {}'.format(err))
    assert abs(err) < 1e-9

if __name__ == "__main__":
    test_answer()