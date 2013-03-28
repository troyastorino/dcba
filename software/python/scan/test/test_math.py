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
        
class TestBits(unittest.TestCase):
    def test_bits_scalar(self):
        x = 4
        nbits = 4
        x_bits = np.array([0, 1, 0, 0])
        assert_equal(bits(x, nbits=nbits), x_bits)

    def test_bits_scalar_max_bits(self):
        x = 6
        x_bits = np.array([1, 1, 0])
        assert_equal(bits(x), x_bits)

    def test_bits_vector(self):
        x = np.array([3, 1, 5])
        x_bits = np.array([[0, 1, 1],
                           [0, 0, 1],
                           [1, 0, 1]])
        assert_equal(bits(x), x_bits)

    def test_bits_matrix(self):
        x = np.array([[3, 1, 2],
                      [0, 2, 1]])
        x_bits = np.array([[[1, 1], [0, 1], [1, 0]],
                           [[0, 0], [1, 0], [0, 1]]])
        assert_equal(bits(x), x_bits)

    def test_not_enough_bits(self):
        with self.assertRaises(Exception):
            bits(8, nbits=2)
