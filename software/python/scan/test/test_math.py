import unittest
import numpy as np
from numpy.testing import assert_equal, assert_allclose
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
        
class TestFromGrayCode(unittest.TestCase):
    def test_from_gray_code_list(self):
        x = np.arange(4)
        x_gray = np.array([0, 1, 3, 2])
        assert_equal(from_gray_code(x_gray), x)
    def test_from_gray_code_scalar(self):
        x = 5
        x_gray = 7
        assert_equal(from_gray_code(x_gray), x)
    def test_from_gray_code_matrix(self):
        shape = (4,2)
        x = np.arange(8).reshape(shape)
        x_gray = np.array([0, 1, 3, 2, 6, 7, 5, 4]).reshape(shape)
        assert_equal(from_gray_code(x_gray), x)

class TestGrayCodeConversionConsistency(unittest.TestCase):
    @staticmethod
    def rand_int(max_size=10):
        return int((max_size-1) * np.random.random()) + 1

    @staticmethod
    def uint32_array(ndims, max_size=10):
        dims = [TestGrayCodeConversionConsistency.rand_int(max_size) for i in range(ndims)]
        x = 2**32 * np.random.random(dims)
        return x.astype(np.uint32)
    
    def test_scalar_consistency(self):
        x = self.uint32_array(0)
        assert_equal(from_gray_code(gray_code(x)), x)

    def test_vector_consistency(self):
        x = self.uint32_array(1)
        assert_equal(from_gray_code(gray_code(x)), x)        
        
    def test_matrix_consistency(self):
        x = self.uint32_array(2)
        assert_equal(from_gray_code(gray_code(x)), x)

    def test_3_dim_consistency(self):
        x = self.uint32_array(3, max_size=50)
        assert_equal(from_gray_code(gray_code(x)), x)

    def test_4_dim_consistency(self):
        x = self.uint32_array(3, max_size=10)
        assert_equal(from_gray_code(gray_code(x)), x)
        
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
        
class TestNormalize(unittest.TestCase):
    rtol = 1e-7
    
    def test_vector(self):
        x = 5 * np.ones((4,))
        x_normalized = [1.0/2, 1.0/2, 1.0/2, 1.0/2]
        assert_allclose(normalize(x), x_normalized, rtol=self.rtol)

    def test_other_vector(self):
        x = [49, 0]
        x_normalized = [1, 0]
        assert_allclose(normalize(x), x_normalized, rtol=self.rtol)

    def test_array_of_vecs(self):
        x = [[0, 4, 3],
             [2, 1, 2]]
        x_normalized = [[0, 4.0/5, 3.0/5],
                        [2.0/3, 1.0/3, 2.0/3]]
        assert_allclose(normalize(x, axis=1), x_normalized, rtol=self.rtol)

    def test_grid_of_vecs(self):
        x = [[[4, 3], [5 ,12]],
             [[40 ,9], [15 ,8]]]
        x_normalized = [[[4.0/5, 3.0/5], [5.0/13 ,12.0/13]],
                        [[40.0/41 ,9.0/41], [15.0/17 ,8.0/17]]]
        assert_allclose(normalize(x, axis=2), x_normalized, rtol=self.rtol)

class TestFitPlane(unittest.TestCase):
    rtol = 1e-7

    def test_one_plane(self):
        points = np.eye(3)
        coeffs = np.ones(4)
        assert_allclose(normalize(fit_plane(points)), normalize(coeffs),
                        rtol=self.rtol)

    def test_list_of_planes(self):
        points = [[[0, 1, 0],
                   [1, 0, 0],
                   [0, 1, 0],
                   [1, 0, 0],
                   [0, 0, 1]],
                  [[0, 0, 0],
                   [1, 0, 0],
                   [0, 1, 0],
                   [1, 0, 0],
                   [0, 1, 0]]]
        coeffs = [[1, 1, 1, 1],
                  [0, 0, 1, 0]]
        assert_allclose(normalize(fit_plane(points)), normalize(coeffs),
                        rtol=self.rtol)

    def test_grid_of_planes(self):
        points = [[[[1, 1, 2],
                    [12, 6, 2],
                    [-3, 2, 2]],
                   [[-5, 3, 12],
                    [-5, -4, 0],
                    [-5, 24, -432]]],
                  [[[0, 0, 0],
                    [12, 0, 4],
                    [23, 0, -13]],
                   [[-2, 0, 0],
                    [0, -8, 0],
                    [0, 0, -4]]]]
        coeffs = [[[0, 0, 1, 2],
                   [1, 0, 0, -5]],
                  [[0, 1, 0, 0],
                   [4, 1, 2, -8]]]
        assert_allclose(normalize(fit_plane(points)), normalize(coeffs),
                        rtol=self.rtol)
                  
                   
                  
