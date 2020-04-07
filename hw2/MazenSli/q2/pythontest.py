import unittest
import angle

class testAngle(unittest.TestCase):

    def test_method1(self):
        assert angle.calc_angle([1, 1], [1, 1]) == 0


if __name__ == "__main__":
    unittest.main()
