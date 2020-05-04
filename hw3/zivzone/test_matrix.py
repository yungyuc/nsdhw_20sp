import unittest


class MatrixTest(unittest.TestCase):
    def setUp(self):
        import _matrix
        self.matrix = _matrix.Matrix
        self.multiply = {
            'multiply_naive': _matrix.multiply_naive,
            'multiply_mkl': _matrix.multiply_mkl
        }

    def test_matrix_create(self):
        shape = [(1, 1), (15, 10), (10, 15), (1100, 1100)]
        for row, col in shape:
            first_matrix = self.matrix(row, col)
            second_matrix = self.matrix(row, col)
            self.assertEqual(first_matrix.nrow, row)
            self.assertEqual(first_matrix.ncol, col)
            self.assertEqual(second_matrix.nrow, row)
            self.assertEqual(second_matrix.ncol, col)
            self.assertEqual(first_matrix, second_matrix)

    def square_test(self, first_method, second_method):
        shape = [1, 10, 100, 1000]
        for size in shape:
            I = self.matrix(size, size)
            for i in range(size):
                for j in range(size):
                    if i == j:
                        I[i, j] = 1
                    else:
                        I[i, j] = 0

            self.assertEqual(first_method(I, I), second_method(I, I))

    def example_test(self, first_method, second_method):
        first_matrix = self.matrix(3, 2)
        second_matrix = self.matrix(2, 2)
        result_matrix = self.matrix(3, 2)
        
        first_matrix[0, 0] = 0
        first_matrix[0, 1] = -1
        first_matrix[1, 0] = 1
        first_matrix[1, 1] = 0
        first_matrix[2, 0] = 1
        first_matrix[2, 1] = 1
        second_matrix[0, 0] = 0
        second_matrix[0, 1] = 1
        second_matrix[1, 0] = -1
        second_matrix[1, 1] = 0
        result_matrix[0, 0] = 1
        result_matrix[0, 1] = 0
        result_matrix[1, 0] = 0
        result_matrix[1, 1] = 1
        result_matrix[2, 0] = -1
        result_matrix[2, 1] = 1
        
        self.assertEqual(first_method(first_matrix, second_matrix), result_matrix)
        self.assertEqual(second_method(first_matrix, second_matrix), result_matrix)
        self.assertEqual(first_method(first_matrix, second_matrix), second_method(first_matrix, second_matrix))

    def test_multiply_mkl_accuracy(self):
        method = [self.multiply[x] for x in ('multiply_mkl', ) * 2]
        self.square_test(*method)
        self.example_test(*method)

    def test_multiply_naive_accuracy(self):
        method = [self.multiply[x] for x in ('multiply_naive', ) * 2]
        self.square_test(*method)
        self.example_test(*method)

    def test_multiply_two_mehod_equal(self):
        method = [self.multiply[x] for x in ('multiply_mkl', 'multiply_naive')]
        self.square_test(*method)
        self.example_test(*method)


if __name__ == '__main__':
    unittest.main()
