#!/usr/bin/env python3
# Developer: Wilbert (wilbert.phen@gmail.com)

import unittest
import math
import numpy as np
import _vector

class pytest(unittest.TestCase):

    def testInvalidVector(self):
        self.assertRaises(TypeError, _vector.angle, [0, 0], [])
        self.assertRaises(TypeError, _vector.angle, [], [0,0])
        self.assertRaises(TypeError, _vector.angle, [], [])

    def testZeroLengthVector(self):
        self.assertRaises(ValueError, _vector.angle, [1, 1], [0, 0])
        self.assertRaises(ValueError, _vector.angle, [0, 0], [1, 1])
        self.assertRaises(ValueError, _vector.angle, [0, 0], [0, 0])

    def testZeroAngle(self):
        for i in range(10):
            mult = np.random.randint(1, 10)
            vec1 = np.random.randint(1, 10, 2)
            vec2 = vec1 * mult
            self.assertAlmostEqual(_vector.angle(vec1, vec2), 0, places=5)

    def testRightAngle(self):
        for i in range(10):
            mult = np.random.randint(1, 10)
            vec1 = np.random.randint(1, 10)
            vec1 = np.array([vec1, vec1])
            vec2 = vec1 * np.array([-mult, mult])
            self.assertAlmostEqual(_vector.angle(vec1, vec2), math.pi/2, places=5)

    def testOpposite(self):
        for i in range(10):
            mult = np.random.randint(-10, -1)
            vec1 = np.random.randint(1, 10, 2)
            vec2 = vec1 * mult
            self.assertAlmostEqual(_vector.angle(vec1, vec2), math.pi, places=5)


if __name__ == '__main__':
    unittest.main()
