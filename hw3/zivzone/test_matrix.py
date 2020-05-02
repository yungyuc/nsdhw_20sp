import unittest


class MatrixTest(unittest.TestCase):
    def setUp(self):
        import _matrix
        self.matrix = _matrix.Matrix
        self.multiply = {
            'naive': _matrix.multiply_naive,
            'mkl': _matrix.multiply_mkl
        }

    def test_insert(self):
        shape = [(1, 1), (2, 3), (30, 20), (1000, 1000)]
        for row, col in shape:
            m = self.matrix(row, col)
            self.assertEqual(m.nrow, row)
            self.assertEqual(m.ncol, col)

    def test_two_matrixs_are_equal(self):
        shape = [(1, 1), (2, 3), (30, 20), (1000, 1000)]
        for row, col in shape:
            first_matrix = self.matrix(row, col)
            second_matrix = self.matrix(row, col)
            self.assertEqual(first_matrix, second_matrix)

    def identity_test(self, lmul, rmul):
        shape = [1, 2, 30, 100]
        for s in shape:
            # idm: identity matrix
            idm = self.matrix(s, s)
            for i in range(s):
                for j in range(s):
                    idm[i, j] = 0 if i != j else 1
            # m: arbitrary matrix
            m = self.matrix(s, s)
            for i in range(s):
                for j in range(s):
                    idm[i, j] = i + j
            self.assertEqual(lmul(idm, m), rmul(idm, m))
            self.assertEqual(lmul(m, idm), rmul(m, idm))
            self.assertEqual(lmul(idm, m), lmul(m, idm))
            self.assertEqual(rmul(idm, m), rmul(m, idm))

    def example(self, lmul, rmul):
        first_matrix = self.matrix(3, 2)
        first_matrix[0, 0] = 0
        first_matrix[0, 1] = -1
        first_matrix[1, 0] = 1
        first_matrix[1, 1] = 0
        first_matrix[2, 0] = 1
        first_matrix[2, 1] = 1
        second_matrix = self.matrix(2, 2)
        second_matrix[0, 0] = 0
        second_matrix[0, 1] = 1
        second_matrix[1, 0] = -1
        second_matrix[1, 1] = 0
        result_matrix = self.matrix(3, 2)
        result_matrix[0, 0] = 1
        result_matrix[0, 1] = 0
        result_matrix[1, 0] = 0
        result_matrix[1, 1] = 1
        result_matrix[2, 0] = -1
        result_matrix[2, 1] = 1
        self.assertEqual(lmul(first_matrix, second_matrix), result_matrix)
        self.assertEqual(rmul(first_matrix, second_matrix), result_matrix)
        self.assertEqual(lmul(first_matrix, second_matrix), rmul(first_matrix, second_matrix))

    def test_multiply_mkl_accuracy(self):
        muls = [self.multiply[x] for x in ('mkl', ) * 2]
        self.identity_test(*muls)
        self.example(*muls)

    def test_multiply_naive_accuracy(self):
        muls = [self.multiply[x] for x in ('naive', ) * 2]
        self.identity_test(*muls)
        self.example(*muls)

    def test_multiply_two_mehod_equal(self):
        muls = [self.multiply[x] for x in ('mkl', 'naive')]
        self.identity_test(*muls)
        self.example(*muls)


if __name__ == '__main__':
    unittest.main()
