import unittest
import numpy as np
from numpy.testing import assert_equal
from scan.common.math import *

class TestGrayCode(unittest.TestCase):
    def test_gray_code_list(self):
        x = np.arange(4)
        x_gray = np.array([0, 1, 3, 2])
        assert_equal(gray_code(x), x_gray)
    def test_gray_code_scalar(self):
        x = 5
        x_gray = 7
        assert_equal(gray_code(x), x_gray)
    def test_gray_code_matrix(self):
        shape = (4,2)
        x = np.arange(8).reshape(shape)
        x_gray = np.array([0, 1, 3, 2, 6, 7, 5, 4]).reshape(shape)
        assert_equal(gray_code(x), x_gray)
        
class TestBitArray(unittest.TestCase):
    def test_scalar(self):
        x = 4
        nbits = 4
        x_bits = np.array([0, 1, 0, 0])
        assert_equal(bit_array(x, n_bits=nbits), x_bits)

    def test_scalar_max_bits(self):
        x = 6
        x_bits = np.array([1, 1, 0])
        assert_equal(bit_array(x), x_bits)

    def test_vector(self):
        x = np.array([3, 1, 5])
        x_bits = np.array([[0, 1, 1],
                           [0, 0, 1],
                           [1, 0, 1]])
        assert_equal(bit_array(x), x_bits)

    def test_matrix(self):
        x = np.array([[3, 1, 2],
                      [0, 2, 1]])
        x_bits = np.array([[[1, 1], [0, 1], [1, 0]],
                           [[0, 0], [1, 0], [0, 1]]])
        assert_equal(bit_array(x), x_bits)

    def test_3_dims(self):
        x = np.array([[[3, 0],
                      [1, 1]],
                     [[2, 1],
                      [3, 3]]])
        x_bits = np.array([[[[1, 1], [0, 0]],
                            [[0, 1], [0, 1]]],
                           [[[1, 0], [0, 1]],
                            [[1, 1], [1, 1]]]])
        assert_equal(bit_array(x), x_bits)
        
    def test_not_enough_bits(self):
        with self.assertRaises(Exception):
            bit_array(8, nbits=2)

class TestPackBitArray(unittest.TestCase):
    def test_scalar(self):
        x = 4
        nbits = 4
        x_bits = np.array([0, 1, 0, 0])
        assert_equal(pack_bit_array(x_bits), x)

    def test_scalar_max_bits(self):
        x = 6
        x_bits = np.array([1, 1, 0])
        assert_equal(bit_array(x), x_bits)

    def test_vector(self):
        x = np.array([3, 1, 5])
        x_bits = np.array([[0, 1, 1],
                           [0, 0, 1],
                           [1, 0, 1]])
        assert_equal(pack_bit_array(x_bits), x)

    def test_matrix(self):
        x = np.array([[3, 1, 2],
                      [0, 2, 1]])
        x_bits = np.array([[[1, 1], [0, 1], [1, 0]],
                           [[0, 0], [1, 0], [0, 1]]])
        assert_equal(pack_bit_array(x_bits), x)

    def test_3_dims(self):
        x = np.array([[[3, 0],
                      [1, 1]],
                     [[2, 1],
                      [3, 3]]])
        x_bits = np.array([[[[1, 1], [0, 0]],
                            [[0, 1], [0, 1]]],
                           [[[1, 0], [0, 1]],
                            [[1, 1], [1, 1]]]])
        assert_equal(pack_bit_array(x_bits), x)

class TestBitArrayConsistency(unittest.TestCase):
    def rand_int(self, max_size=100):
        return int((max_size-1) * np.random.random()) + 1

    def uint32_array(self, ndims, max_size=100):
        dims = [self.rand_int(max_size) for i in range(ndims)]
        x = 2**32 * np.random.random(dims)
        return x.astype(np.uint32)
    
    def test_scalar_consistency(self):
        x = self.uint32_array(0)
        assert_equal(pack_bit_array(bit_array(x)), x)

    def test_vector_consistency(self):
        x = self.uint32_array(1)
        assert_equal(pack_bit_array(bit_array(x)), x)        
        
    def test_matrix_consistency(self):
        x = self.uint32_array(2)
        assert_equal(pack_bit_array(bit_array(x)), x)

    def test_3_dim_consistency(self):
        x = self.uint32_array(3, max_size=50)
        assert_equal(pack_bit_array(bit_array(x)), x)

    def test_4_dim_consistency(self):
        x = self.uint32_array(3, max_size=10)
        assert_equal(pack_bit_array(bit_array(x)), x)
        
