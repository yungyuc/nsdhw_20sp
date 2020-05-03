import unittest
import numpy as np
import random
import math
import _vector as v

class pytest(unittest.TestCase):
    def setUp(self):
        self.places = 4
        self.rd = lambda x=1, y=100: float(random.randrange(x, y))

    def Answer(self, vecA, vecB):
        lenA = np.dot(vecA, vecA)
        lenB = np.dot(vecB, vecB)
        d = float(np.dot(vecA, vecB)/(math.sqrt(lenA*lenB)))
        d = max(-1.0, min(1.0, d))
        return math.acos(d)

    def testInvalidInput_Len0(self):
       self.assertRaises(TypeError, v.radian_between_2dvecs, [1, 2], [])

    def testInvalidInput_Len3(self):
       self.assertRaises(TypeError, v.radian_between_2dvecs, [1, 2, 3], [1, 2])
    
    def testUniformRandom(self):
        a = list(map(lambda x : self.rd(), range(2)))
        b = list(map(lambda x : self.rd(), range(2)))
        self.assertAlmostEqual(v.radian_between_2dvecs(a, b), self.Answer(a, b), places=self.places)

    def testOrthogonal(self):
        r = self.rd()
        a = list(map(lambda x : self.rd(), range(2)))
        b = [-a[0]*r, a[1]*r]
        self.assertAlmostEqual(v.radian_between_2dvecs(a, b), self.Answer(a, b), places=self.places)
    
    def testSameDirection(self):
        r = self.rd()
        a = list(map(lambda x : self.rd(), range(2)))
        b = list(map(lambda x: x*r, a))
        self.assertAlmostEqual(v.radian_between_2dvecs(a, b), self.Answer(a, b), places=self.places)
    
    def testOppositeDirection(self):
        r = self.rd(1,10)
        a = list(map(lambda x : self.rd(), range(2)))
        b = list(map(lambda x: -x*r, a))
        self.assertAlmostEqual(v.radian_between_2dvecs(a, b), self.Answer(a, b), places=self.places)


if __name__ == "__main__":
    unittest.main()